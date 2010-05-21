/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  

#include <Jet/Graphics/OpenGLMesh.hpp>
#include <Jet/Graphics/OpenGLShader.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreMeshLoader.hpp>
#include <stdexcept>

using namespace Jet;
using namespace std;
using namespace boost;


OpenGLMesh::~OpenGLMesh() {
	
	// Free the buffer from memory
	state(RS_UNLOADED);
}

void OpenGLMesh::state(ResourceState state) {
	if (state == state_) {
		return; // No change
	}
	
	// Leaving the RS_UNLOADED state
	if (RS_UNLOADED == state_) {
		// Load the data from the file
		read_mesh_data();
	}
	
	// Entering the RS_LOADED state
	if (RS_LOADED == state) {
		update_tangents();
		init_hardware_buffers();
	}

	// Leaving the RS_LOADED state
	if (RS_LOADED == state_) {
		free_hardware_buffers();
	}
	
	// Entering the unloaded state
	if (RS_UNLOADED == state) {
		vertex_.clear();
		index_.clear();
	}
	
	// Update the geometry
	geometry_->state(state);
	state_ = state;
}

void OpenGLMesh::free_hardware_buffers() {
	assert(ibuffer_ && vbuffer_);
	
	// Free the vertex buffers
	if (!parent_) {
		// If the mesh has no parent, then the vertex buffer is owned by
		// this mesh.
		glDeleteBuffers(1, &vbuffer_);
	}
	glDeleteBuffers(1, &ibuffer_);
	ibuffer_ = 0;
	vbuffer_ = 0;
	nindices_ = 0;
}

void OpenGLMesh::init_hardware_buffers() {
	assert(!ibuffer_ && !vbuffer_);	

	// Choose the appropriate buffer mode.  TODO: Calling glBufferData
	// below may be destructive in terms of performance; investigate
	GLenum mode;
	if (SM_STATIC == sync_mode_) {
		mode = GL_STATIC_DRAW;
	} else {
		mode = GL_DYNAMIC_DRAW;
	}
	
	if (!parent_) {
		// Copy vertex data to graphics card
		glGenBuffers(1, &vbuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
		glBufferData(GL_ARRAY_BUFFER, vertex_count()*sizeof(Vertex), vertex_data(), mode);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		parent_->state(RS_LOADED);
		vbuffer_ = parent_->vbuffer_;
	}

	// Copy index data to graphics card
	glGenBuffers(1, &ibuffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count()*sizeof(uint32_t), index_data(), mode);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	nindices_ = index_count();
}

//! Updates tangent vectors for the mesh
void OpenGLMesh::update_tangents() {
	// Only update tangents if the node owns the vertex buffer,
	// i.e., if it doesn't have a parent.
	if (!parent_) {
		// Initialize all tangents to zero for the whole mesh
		for (size_t i = 0; i < vertex_.size(); i++) {
			vertex_[i].tangent = Vector();
		}
		
		// Iterate through faces and add each face's contribution to
		// the tangents of its vertices
		for (size_t i = 2; i < index_.size(); i += 3) {
			Vertex& p0 = vertex_[index_[i-2]];
			Vertex& p1 = vertex_[index_[i-1]];
			Vertex& p2 = vertex_[index_[i-0]];
			
			// Tangent calculation
			Vector d1 = p1.position - p0.position;
			Vector d2 = p2.position - p1.position;
			const Texcoord& tex0 = p0.texcoord;
			const Texcoord& tex1 = p1.texcoord;
			const Texcoord& tex2 = p2.texcoord;
			float s1 = tex1.u - tex0.u;
			float t1 = tex1.v - tex0.v;
			float s2 = tex2.u - tex0.u;
			float t2 = tex2.v - tex0.v;
			float a = 1/(s1*t2 - s2*t1);
			
			// Add tangent contribution
			p0.tangent += ((d1*t2 - d2*t1)*a).unit();
			p1.tangent += ((d1*t2 - d2*t1)*a).unit();
			p2.tangent += ((d1*t2 - d2*t1)*a).unit();
		}
		
		// Normalize all the tangents
		for (size_t i = 0; i < vertex_.size(); i++) {
			vertex_[i].tangent = vertex_[i].tangent.unit();
		}
	}
}

void OpenGLMesh::read_mesh_data() {
	static const string ext = ".obj";
	size_t pos = name_.rfind(ext);
	if (pos == string::npos || (name_.length() - pos) != ext.length()) {
        // This mesh has no associated data if it doesn't end with
		// the extension .obj.  It may be a custom user mesh.
		return;
    }

	// Get the path to the file and read it in
	string file = engine_->resource_path(name_);
	CoreMeshLoader(this, file);
}

void OpenGLMesh::render(OpenGLShader* shader) {
	
	// Make sure that all vertex data is synchronized
	state(RS_LOADED);
	if (parent_) {
		parent_->state(RS_LOADED);
	}
	
	// Bind and enable the vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (shader && engine_->option<bool>("shaders_enabled")) {
		// Enable tangent vectors
		GLint tangent_attrib = shader->attrib_location("tangent");
		glEnableVertexAttribArray(tangent_attrib);
		glVertexAttribPointer(tangent_attrib, 3, GL_FLOAT, 1, sizeof(Vertex), (void*)(6*sizeof(GLfloat)));
	}

	// Set up the buffer offsets (equivalent of FVF in D3D9)
	// and then render the indexed buffers
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(GLfloat)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(9*sizeof(GLfloat)));
    glDrawElements(GL_TRIANGLES, nindices_, GL_UNSIGNED_INT, (void*)0);

	// Disable index and vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (shader && engine_->option<bool>("shaders_enabled")) {
		// Disable tangent vectors
		GLint tangent_attrib = shader->attrib_location("tangent");
		glDisableVertexAttribArray(tangent_attrib);
	}
}

void OpenGLMesh::vertex(size_t i, const Vertex& vertex) {
	if (parent_) {
		throw std::runtime_error("Vertex data is read-only");
	} else {
		if (i >= vertex_.size()) {
			vertex_count(i + 1);
		}
		vertex_[i] = vertex;
	}
}

void OpenGLMesh::index(size_t i, uint32_t index) {
	if (i >= index_.size()) {
		index_count(i + 1);
	}
	index_[i] = index;
}

void OpenGLMesh::index_count(size_t size) {
	if (size != index_.size()) {
		index_.resize(size);
	}
}

void OpenGLMesh::vertex_count(size_t size) {
	if (parent_) {
		throw runtime_error("Vertex data is read-only");
	}
	
	if (size != vertex_.size()) {
		vertex_.resize(size);
	}
}

const Vertex& OpenGLMesh::vertex(size_t i) const {
	if (parent_) {
		return parent_->vertex(i);
	} else {
		return vertex_[i];
	}
}

Vertex& OpenGLMesh::vertex(size_t i) {
	if (parent_) {
		return parent_->vertex(i);
	} else {
		return vertex_[i];
	}
}

uint32_t OpenGLMesh::index(size_t i) const {
	return index_[i];
}

const Vertex* OpenGLMesh::vertex_data() const {
	if (parent_) {
		return parent_->vertex_data();
	} else {
		return vertex_.size() ? &vertex_.front() : 0;
	}
}

const uint32_t* OpenGLMesh::index_data() const {
	return index_.size() ? &index_.front() : 0;
}
