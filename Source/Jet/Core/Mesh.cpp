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
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
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


Core::Mesh::Mesh(Engine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(UNLOADED),
		vbuffer_(0),
		ibuffer_(0),
		nindices_(0),
		sync_mode_(STATIC_SYNC),
		bounding_shape_(btVector3(0.0f, 0.0f, 0.0f)) {
}

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
		// Create index and vertex buffers
		glGenBuffers(1, &vbuffer_);
		glGenBuffers(1, &ibuffer_);

		// Choose the appropriate buffer mode.  TODO: Calling glBufferData
		// below may be destructive in terms of performance; investigate
		GLenum mode;
		if (STATIC_SYNC == sync_mode_) {
			mode = GL_STATIC_DRAW;
		} else {
			mode = GL_DYNAMIC_DRAW;
		}
		
		// Copy vertex data to graphics card
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
		glBufferData(GL_ARRAY_BUFFER, vertex_count()*sizeof(Vertex), vertex_data(), mode);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		// Copy index data to graphics card
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count()*sizeof(uint32_t), index_data(), mode);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
		nindices_ = index_count();

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
		
		// Generate the bounding shape
		bounding_box_ = BoundingBox();
		for (vector<Vertex>::iterator i = vertex_.begin(); i != vertex_.end(); i++) {
			bounding_box_.point(i->position);
		}
		btVector3 half_extents(bounding_box_.half_extents().x, bounding_box_.half_extents().y, bounding_box_.half_extents().z);
		bounding_shape_ = btBoxShape(half_extents);
	}

	// Leaving the SYNCED state
	if (SYNCED == state_) {
		// Free the vertex buffers
		glDeleteBuffers(1, &vbuffer_);
		glDeleteBuffers(1, &ibuffer_);
		vbuffer_ = 0;
		ibuffer_ = 0;
		nindices_ = 0;
	}
	
	// Entering the unloaded state
	if (UNLOADED == state) {
		vertex_.clear();
		index_.clear();
	}
	
	state_ = state;
}


void Core::Mesh::read_mesh_data() {
	static const string ext = ".obj";
    if ((name_.length() - name_.rfind(ext)) != ext.length()) {
        // This mesh has no associated data if it doesn't end with
		// the extension .obj.  It may be a custom user mesh.
		return;
    }

	string file = engine_->resource_path(name_);
    ifstream in(file.c_str());

    // Collection of vertices
    string command;
    uint32_t index = 0;
	vector<Vector> positions;
	vector<Vector> normals;
	vector<Texcoord> texcoords;

	typedef Vertex vkey_t;
	typedef pair<Vertex, uint32_t> vval_t;
	
	// This map is used to cache vertices that are already present so that we
	// can use the index buffer to get rid of duplicates
	map<vkey_t, vval_t> vertices;
	vector<uint32_t> indices;
    
	// Read in the whole file, one commmand at a time.  Each line starts
	// with a command word or "#" if the line is a comment.
    while (in.good() && !in.eof()) {
        in >> command;

		if (!in.good()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            string line;
            getline(in, line);
        } else if (command == "v") {
			Vector position;
			in >> position;
			positions.push_back(position);
		} else if (command == "vt") {
			Texcoord texcoord;
			in >> texcoord;
			texcoords.push_back(texcoord);
		} else if (command == "vn") {
			Vector normal;
			in >> normal;
			normals.push_back(normal);
		} else if (command == "f") {
			Vertex face[3];
			
			// Read in the face
			for (int i = 0; i < 3; i++) {
				string line, i1, i2, i3;
				in >> line;
				istringstream ss(line);
				getline(ss, i1, '/');
				getline(ss, i2, '/');
				getline(ss, i3, '/');
				
				if (!i1.empty()) {
					face[i].position = positions[lexical_cast<size_t>(i1)-1];
				}
				if (!i2.empty()) {
					face[i].texcoord = texcoords[lexical_cast<size_t>(i2)-1];
				}
				if (!i3.empty()) {
					face[i].normal = normals[lexical_cast<size_t>(i3)-1];
				}
			}
			
			// Add vertices to the buffer
			for (int i = 0; i < 3; i++) {
				// Calculate tangent
				compute_tangent(face, i);
				map<vkey_t, vval_t>::iterator j = vertices.find(face[i]);
				if (j == vertices.end()) {
					// Vertex was not found, so push a new index and vertex
					// into the list
					indices.push_back(index);
					vertices.insert(make_pair(face[i], make_pair(face[i], index)));
					index++;
				} else {
					// Vertex was found, so use the existing index.
					//j->second.first.tangent = j->second.first.tangent + face[i].tangent;
					indices.push_back(j->second.second);
				}
			}
		}
    }
	
	// Copy data over to the linear memory buffer
	
	Mesh::vertex_count(vertices.size());
	for (map<vkey_t, vval_t>::iterator i = vertices.begin(); i != vertices.end(); i++) {
		Vertex& vertex = i->second.first;
		uint32_t index = i->second.second;

		//Vector temp = vertex.tangent.unit().cross(vertex.normal);
		vertex.tangent = vertex.tangent.unit(); //temp.cross(vertex.normal);
		Mesh::vertex(index, vertex);
	}
	Mesh::index_count(indices.size());
	for (size_t i = 0; i < indices.size(); i++) {
		Mesh::index(i, indices[i]);
	}
	btVector3 half_extents(bounding_box_.half_extents().x, bounding_box_.half_extents().y, bounding_box_.half_extents().z);
	bounding_shape_ = btBoxShape(half_extents);
}

void Core::Mesh::compute_tangent(Vertex face[3], size_t j) {
	// Calculate binormals
	Vertex& p0 = face[j];
	Vertex& p1 = face[(j+1)%3];
	Vertex& p2 = face[(j+2)%3];
	
    Vector d1 = p1.position - p0.position;
    Vector d2 = p2.position - p1.position;
    const Texcoord& tex0 = p0.texcoord;
    const Texcoord& tex1 = p1.texcoord;
    const Texcoord& tex2 = p2.texcoord;
    real_t s1 = tex1.u - tex0.u;
    real_t t1 = tex1.v - tex0.v;
    real_t s2 = tex2.u - tex0.u;
    real_t t2 = tex2.v - tex0.v;

    real_t a = 1/(s1*t2 - s2*t1);
    p0.tangent = p0.tangent + ((d1*t2 - d2*t1)*a).unit();
	p1.tangent = p1.tangent + ((d1*t2 - d2*t1)*a).unit();
	p2.tangent = p2.tangent + ((d1*t2 - d2*t1)*a).unit();
}

void Core::Mesh::render(Core::Shader* shader) {
	state(SYNCED);
	
	// Bind and enable the vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (shader && engine_->option<bool>("shaders_enabled")) {
		// Enable tangent vectors
		glEnableVertexAttribArray(shader->tangent_attrib());
		glVertexAttribPointer(shader->tangent_attrib(), 3, GL_FLOAT, 1, sizeof(Vertex), (void*)(6*sizeof(GLfloat)));
	}

	// Set up the buffer offsets (equivalent of FVF in D3D9)
	// and then render the indexed buffers
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
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
		glDisableVertexAttribArray(shader->tangent_attrib());
	}
}

void Core::Mesh::index_count(size_t size) {
	if (size != index_.size()) {
		index_.resize(size);
		
		if (SYNCED == state_) {
			state(LOADED);
		}
	}
}

void Core::Mesh::vertex_count(size_t size) {
	if (size != vertex_.size()) {
		vertex_.resize(size);
		
		if (SYNCED == state_) {
			state(LOADED);
		}
	}
}
