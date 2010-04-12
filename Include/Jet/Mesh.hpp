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

#include <Jet/Types.hpp>
#include <Jet/Vertex.hpp> 
#include <Jet/Object.hpp>
#include <vector>

namespace Jet {

//! Class to hold mesh geometry for rendering.  Meshes are always stored as
//! triangle lists (not triangle strips).
//! @class Mesh
//! @brief Class to hold mesh geometry for rendering.
class JETAPI Mesh : public Object {
public:
    //! Destructor.
    virtual ~Mesh() {}

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
    
    //! Marks the mesh as loaded.
    void loaded(bool loaded);
    
    //! Sets the implementation object
    inline void impl(Object* impl) {
        impl_ = impl;
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
    
    //! Returns true if the mesh data is loaded
    inline bool loaded() const {
        return loaded_;
    }
    
    //! Returns a pointer to the implementation object.
    Object* impl() const {
        return impl_.get();
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
    
private:
    Mesh(Engine* engine, const std::string& name);
    
    Engine* engine_;
	std::string name_;
    bool loaded_;
#pragma warning(disable:4251)
    ObjectPtr impl_;
    std::vector<Vertex> vertex_;
    std::vector<uint32_t> index_;
#pragma warning(default:4251)

    friend class Engine;
};

}
