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
#pragma once

#include <Jet/Core/Types.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Vertex.hpp>
#include <Jet/BoundingBox.hpp>
#include <vector>
#include <iostream>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

namespace Jet { namespace Core {

//! Class to hold mesh geometry for rendering.  Meshes are always stored as
//! triangle lists (not triangle strips).
//! @class Mesh
//! @brief Class to hold mesh geometry for rendering.
class Mesh : public Jet::Mesh {
public:

	//! Destructor.
	virtual ~Mesh();

    //! Sets a vertex that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the vertex
    //! @param vertex the vertex to add.
    inline void vertex(size_t i, const Vertex& vertex) {
		if (i >= vertex_.size()) {
			vertex_count(i + 1);
		}
		vertex_[i] = vertex;
		if (SYNCED == state_) {
			state(LOADED);
		}
	}

    //! Sets an index that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the index.
    //! @param index the index to add
    inline void index(size_t i, uint32_t index) {
		if (i >= index_.size()) {
			index_count(i + 1);
		}
		index_[i] = index;
		if (SYNCED == state_) {
			state(LOADED);
		}
	}

    //! Returns a vertex that is part of this mesh
    //! @param i the index of the vertex in the vertex buffer
    inline const Vertex& vertex(size_t i) const {
        return vertex_[i];
    }

    //! Returns an index that is part of this mesh.
    //! @param i the index of the index in the index buffer
    inline uint32_t index(size_t i) const {
        return index_[i];
    }
	
	//! Sets the number of vertices in this mesh
	void vertex_count(size_t size);
	
	//! Sets the number of indices in this mesh
	void index_count(size_t size);
	
	//! Returns the state of this resource.
	inline ResourceState state() const {
		return state_;
	}
	
	//! Returns the sync mode of this mesh.
	inline SyncMode sync_mode() const {
		return sync_mode_;
	}
  
    //! Returns the name of the mesh.
    const std::string& name() const {
        return name_;
    }

    //! Returns a pointer to the beginning of the vertex buffer.
    inline const Vertex* vertex_data() const {
        return vertex_.size() ? &vertex_.front() : 0;
    }

    //! Returns a pointer to the beginning of the index buffer.
    inline const uint32_t* index_data() const {
        return index_.size() ? &index_.front() : 0;
    }

    //! Returns the number of vertices.
    inline size_t vertex_count() const {
        return vertex_.size();
    }

    //! Returns the number of indices.
    inline size_t index_count() const {
        return index_.size();
    }
	
	//! Returns the collision shape
	inline btCollisionShape* shape() {
		return &shape_;
	}
	
	//! Returns the bounding box shape
	inline btCollisionShape* bounding_shape() {
		return &bounding_shape_;
	}
	
	//! Returns the origin of the mesh
	inline Vector origin() const {
		return bounding_box_.origin();
	}

	//! Sets the sync mode of this mesh.
	void sync_mode(SyncMode sync_mode) {
		sync_mode_ = sync_mode;
	}
	
	//! Sets the resource state
	void state(ResourceState state);
	
	//! Renders this mesh
	void render(Shader* shader);
    
private:
    Mesh(Engine* engine, const std::string& name);
	
	void read_mesh_data();
	void compute_tangent(Vertex face[3], size_t j);
    
    Engine* engine_;
	std::string name_;
	ResourceState state_;
    std::vector<Vertex> vertex_;
    std::vector<uint32_t> index_;
	uint32_t vbuffer_;
	uint32_t ibuffer_;
	uint32_t nindices_;
	SyncMode sync_mode_;
	btTriangleIndexVertexArray vertex_array_;
	btConvexHullShape shape_;
	btBoxShape bounding_shape_;
	BoundingBox bounding_box_;
	

    friend class Engine;
};

}}