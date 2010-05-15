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
#include <Jet/Box.hpp>
#include <vector>
#include <map>
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
	inline Mesh(Engine* engine, const std::string& name) :
		engine_(engine),
		destroyed_(false),
		name_(name),
		state_(UNLOADED),
		vbuffer_(0),
		ibuffer_(0),
		nindices_(0),
		sync_mode_(STATIC_SYNC) {
	}
	
	inline Mesh(Engine* engine, const std::string& name, Mesh* parent) :
		engine_(engine),
		parent_(parent),
		destroyed_(false),
		name_(name),
		state_(UNLOADED),
		vbuffer_(0),
		ibuffer_(0),
		nindices_(0),
		sync_mode_(STATIC_SYNC) {
	}
	

	//! Destructor.
	virtual ~Mesh();

    //! Sets a vertex that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the vertex
    //! @param vertex the vertex to add.
    void vertex(size_t i, const Vertex& vertex);
	
    //! Sets an index that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the index.
    //! @param index the index to add
    void index(size_t i, uint32_t index);

    //! Returns a vertex that is part of this mesh
    //! @param i the index of the vertex in the vertex buffer
    const Vertex& vertex(size_t i) const;

	//! Returns a reference to a vertex that is part of this mesh.
	Vertex& vertex(size_t i);

    //! Returns an index that is part of this mesh.
    //! @param i the index of the index in the index buffer
    uint32_t index(size_t i) const;
	
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
    inline const std::string& name() const {
        return name_;
    }

    //! Returns a pointer to the beginning of the vertex buffer.
    const Vertex* vertex_data() const;

    //! Returns a pointer to the beginning of the index buffer.
    const uint32_t* index_data() const;

    //! Returns the number of vertices.
    inline size_t vertex_count() const {
		return parent_? parent_->vertex_count() : vertex_.size();
    }

    //! Returns the number of indices.
    inline size_t index_count() const {
        return index_.size();
    }
	
	//! Returns the collision shape
	inline btCollisionShape* shape() {
		return &shape_;
	}

	//! Sets the sync mode of this mesh.
	inline void sync_mode(SyncMode sync_mode) {
		sync_mode_ = sync_mode;
	}
	
	//! Sets the resource state
	void state(ResourceState state);
	
	//! Renders this mesh using an ibuffer subset.
	//! @param shader the shader to use
	void render(Shader* shader);
    
private:	
	void read_mesh_data();
	void init_hardware_buffers();
	void free_hardware_buffers();
	void update_collision_shape();
	void update_tangents();
    
    Engine* engine_;
	MeshPtr parent_;
	bool destroyed_;
	std::string name_;
	ResourceState state_;
    std::vector<Vertex> vertex_;
    std::vector<uint32_t> index_;
	uint32_t vbuffer_;
	uint32_t ibuffer_;
	uint32_t nindices_;
	SyncMode sync_mode_;
	btConvexHullShape shape_;
	Box bounding_box_;
};

}}
