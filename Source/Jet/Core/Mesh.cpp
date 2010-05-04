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

#include <Jet/Core/Mesh.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/Shader.hpp>
#include <Jet/Core/MeshLoader.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdexcept>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

using namespace Jet;
using namespace std;
using namespace boost;


Core::Mesh::~Mesh() {
	
	// Free the buffer from memory
	state(UNLOADED);
}

void Core::Mesh::state(ResourceState state) {
	if (state == state_) {
		return; // No change
	}
	
	// Leaving the UNLOADED state
	if (UNLOADED == state_) {
		// Load the data from the file
		read_mesh_data();
	}
	
	// Entering the SYNCED state
	if (SYNCED == state) {		

		init_hardware_buffers();
		update_collision_shape();
	}

	// Leaving the SYNCED state
	if (SYNCED == state_) {
		free_hardware_buffers();
	}
	
	// Entering the unloaded state
	if (UNLOADED == state) {
		vertex_.clear();
		index_.clear();
	}
	
	state_ = state;
}

void Core::Mesh::free_hardware_buffers() {
		// Free the vertex buffers
	if (!parent_) {
		glDeleteBuffers(1, &vbuffer_);
		vbuffer_ = 0;
	}
	glDeleteBuffers(1, &ibuffer_);
	ibuffer_ = 0;
	nindices_ = 0;
}

void Core::Mesh::init_hardware_buffers() {
	assert(!ibuffer_);
	
	// Create index and vertex buffers
	if (!parent_) {
		glGenBuffers(1, &vbuffer_);
	}
	glGenBuffers(1, &ibuffer_);

	// Choose the appropriate buffer mode.  TODO: Calling glBufferData
	// below may be destructive in terms of performance; investigate
	GLenum mode;
	if (STATIC_SYNC == sync_mode_) {
		mode = GL_STATIC_DRAW;
	} else {
		mode = GL_DYNAMIC_DRAW;
	}
	
	if (!parent_) {
		// Copy vertex data to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
		glBufferData(GL_ARRAY_BUFFER, vertex_count()*sizeof(Vertex), vertex_data(), mode);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Copy index data to graphics card
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count()*sizeof(uint32_t), index_data(), mode);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	nindices_ = index_count();
}

void Core::Mesh::update_collision_shape() {
	// Create a triangle array using the data loaded from the disk
	btIndexedMesh mesh;
	mesh.m_numTriangles = index_count()/3;
	mesh.m_triangleIndexBase = (uint8_t*)index_data();
	mesh.m_triangleIndexStride = 3*sizeof(uint32_t);
	mesh.m_numVertices = vertex_count();
	mesh.m_vertexBase = (uint8_t*)vertex_data();
	mesh.m_vertexStride = sizeof(Vertex);
	btTriangleIndexVertexArray vertex_array;
	vertex_array.addIndexedMesh(mesh);
	
	//! Create a temporary shape to hold the vertice;
	btConvexTriangleMeshShape temp_shape(&vertex_array);
	btShapeHull shape_hull(&temp_shape);
	btScalar margin = temp_shape.getMargin();
	shape_hull.buildHull(margin);
	shape_ = btConvexHullShape();
	temp_shape.setUserPointer(&shape_hull);

	// Awesome! Create a hull using the hull vertices.
	for (int i = 0; i < shape_hull.numVertices(); i++) {
		shape_.addPoint(shape_hull.getVertexPointer()[i]);
	}
}

void Core::Mesh::read_mesh_data() {
	static const string ext = ".obj";
    if ((name_.length() - name_.rfind(ext)) != ext.length()) {
        // This mesh has no associated data if it doesn't end with
		// the extension .obj.  It may be a custom user mesh.
		return;
    }

	// Get the path to the file and read it in
	string file = engine_->resource_path(name_);
	MeshLoader(this, file);
}


void Core::Mesh::render(Core::Shader* shader) {
	// Make sure that all vertex data is synchronized
	state(SYNCED);
	if (parent_) {
		parent_->state(SYNCED);
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

 void Core::Mesh::vertex(size_t i, const Vertex& vertex) {
	if (parent_) {
		throw std::runtime_error("Vertex data is read-only");
	} else {
		if (i >= vertex_.size()) {
			vertex_count(i + 1);
		}
		vertex_[i] = vertex;
	}
}

//! Sets an index that is part of this mesh.  This method dynamically
//! resizes the buffer as needed.
//! @param i the index of the index.
//! @param index the index to add
void Core::Mesh::index(size_t i, uint32_t index) {
	if (i >= index_.size()) {
		index_count(i + 1);
	}
	index_[i] = index;
}

void Core::Mesh::index_count(size_t size) {
	if (size != index_.size()) {
		index_.resize(size);
	}
}

void Core::Mesh::vertex_count(size_t size) {
	if (parent_) {
		throw runtime_error("Vertex data is read-only");
	}
	
	if (size != vertex_.size()) {
		vertex_.resize(size);
	}
}
