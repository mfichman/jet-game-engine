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
#include <Jet/Object.hpp>

namespace Jet {

//! Class to hold mesh geometry for rendering.  Meshes are always stored as
//! triangle lists (not triangle strips).
//! @class Mesh
//! @brief Class to hold mesh geometry for rendering.
class Mesh : public Object {
public:
    //! Sets a vertex that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the vertex
    //! @param vertex the vertex to add.
    virtual void vertex(size_t i, const Vertex& vertex)=0;

    //! Sets an index that is part of this mesh.  This method dynamically
    //! resizes the buffer as needed.
    //! @param i the index of the index.
    //! @param index the index to add
    virtual void index(size_t i, uint32_t index)=0;

	//! Returns the resource state of the mesh
	virtual void state(ResourceState state)=0;
	
	//! Sets the sync mode.
	virtual void sync_mode(SyncMode mode)=0;
    
    //! Returns a vertex that is part of this mesh
    //! @param i the index of the vertex in the vertex buffer
    virtual const Vertex& vertex(size_t i) const=0;

    //! Returns an index that is part of this mesh.
    //! @param i the index of the index in the index buffer
    virtual uint32_t index(size_t i) const=0;
	
	//! Sets the number of vertices in this mesh
	virtual void vertex_count(size_t size)=0;
	
	//! Sets the number of indices in this mesh
	virtual void index_count(size_t size)=0;
	
	//! Sets the state of the mesh.
	virtual ResourceState state() const=0;
	
	//! REturns the sync mode.
	virtual SyncMode sync_mode() const=0;
    
    //! Returns the name of the mesh.
    virtual const std::string& name() const=0;

    //! Returns a pointer to the beginning of the vertex buffer.
    virtual const Vertex* vertex_data() const=0;

    //! Returns a pointer to the beginning of the index buffer.
    virtual const uint32_t* index_data() const=0;

    //! Returns the number of vertices.
    virtual size_t vertex_count() const=0;

    //! Returns the number of indices.
    virtual size_t index_count() const=0;
};

}
