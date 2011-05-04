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

#include <Jet/Graphics/OpenGLTypes.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Resources/Geometry.hpp>
#include <Jet/Resources/Mesh.hpp>
#include <Jet/Types/Vertex.hpp>
#include <vector>

namespace Jet {

//! Class to hold mesh geometry for rendering.  Meshes are always stored as
//! triangle lists (not triangle strips).
//! @class Mesh
//! @brief Class to hold mesh geometry for rendering.
class OpenGLMesh : public Mesh {
public:
	//! Creates a new mesh.
	inline OpenGLMesh(CoreEngine* engine, const std::string& name) :
		engine_(engine),
		geometry_(engine->geometry(name)),
        name_(name),
		state_(RS_UNLOADED),
		vbuffer_(0),
		ibuffer_(0),
		sync_mode_(SM_STATIC) {
	}
	
	//! Creates a new mesh.
	inline OpenGLMesh(CoreEngine* engine, const std::string& name, OpenGLMesh* parent) :
		engine_(engine),
		parent_(parent),
		geometry_(engine->geometry(name)),
		name_(name),
		state_(RS_UNLOADED),
		vbuffer_(0),
		ibuffer_(0),
		sync_mode_(SM_STATIC) {
	}

	//! Destructor.
	virtual ~OpenGLMesh();

    //! Sets a vertex that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the vertex
    //! @param vertex the vertex to add.
    void vertex(size_t i, const Vertex& vertex);
	
    //! Sets an index that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the index.
    //! @param index the index to add
    void index(size_t group, size_t i, uint32_t index);

    //! Returns a vertex that is part of this mesh
    //! @param i the index of the vertex in the vertex buffer
    const Vertex& vertex(size_t i) const;

	//! Returns a reference to a vertex that is part of this mesh.
	Vertex& vertex(size_t i);

    //! Returns an index that is part of this mesh.
    //! @param i the index of the index in the index buffer
    uint32_t index(size_t group, size_t i) const;
	
	//! Sets the number of vertices in this mesh
	void vertex_count(size_t size);
	
	//! Sets the number of indices in this mesh
	void index_count(size_t group, size_t size);

	//! Sets the number of groups
	void group_count(size_t group);
	
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
    const uint32_t* index_data(size_t group) const;

    //! Returns the number of vertices.
    inline size_t vertex_count() const {
		return parent_? parent_->vertex_count() : vertex_.size();
    }

    //! Returns the number of indices.
    inline size_t index_count(size_t group) const {
		if (group >= index_.size()) {
			throw std::runtime_error("Group does not exist");
		}
        return index_[group].size();
    }

	//! Returns the number of groups
	inline size_t group_count() const {
		return index_.size();
	}

	//! Returns the group at the given index.
	inline const std::string& group(size_t index) const {
		if (index >= group_.size()) {
			throw std::runtime_error("Group does not exist");
		}
		return group_[index];
	}

	//! Returns the group id of the group with the given name.
	inline size_t group(const std::string& name) const {
		typedef std::vector<std::string>::const_iterator itr_t;
	    itr_t i = std::find(group_.begin(), group_.end(), name);
		if (i == group_.end()) {
			throw std::runtime_error("Could not find group");
		}
		return group_.begin() - i;
	}

	//! Sets the group name for the given group.
	inline void group(size_t index, const std::string& name) {
		if (index >= group_count()) {
			group_count(index + 1);
		}
		group_[index] = name;
	}

	//! Sets the sync mode of this mesh.
	inline void sync_mode(SyncMode sync_mode) {
		sync_mode_ = sync_mode;
	}
	
	//! Returns the geometry object associated with this mesh
	inline Geometry* geometry() const {
		return geometry_.get();
	}
	
	//! Sets the resource state
	void state(ResourceState state);
	
	//! Renders this mesh using an ibuffer subset.
	//! @param shader the shader to use
	void render(OpenGLShader* shader);
    
private:	
	void read_mesh_data();
	void init_hardware_buffers();
	void free_hardware_buffers();
	void update_collision_shape();
	void update_tangents();
    
    CoreEngine* engine_;
	OpenGLMeshPtr parent_;
	GeometryPtr geometry_;
	std::string name_;
	ResourceState state_;
    std::vector<Vertex> vertex_;
	std::vector<std::vector<uint32_t>> index_;
	std::vector<std::string> group_;
	GLuint vbuffer_;
	std::vector<GLuint> ibuffer_;
	SyncMode sync_mode_;
};

}
