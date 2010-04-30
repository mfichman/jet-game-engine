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

//! This class is used to display an instanced mesh on the screen.  The mesh
//! can be fractured, so it has its own index buffer and physics geometry
//! cache.
//! @class FractureObject
//! @brief Displays an fracturable instanced mesh.
class FractureObject : public Object {
public:    
    //! Returns the parent of this object.
    virtual Node* parent() const=0;
    
    //! Returns the material used to render this object.
    virtual Material* material() const=0;
    
    //! Returns the mesh used to render this object.
    virtual Mesh* mesh() const=0;
    
    //! Returns true if this object casts shadows.
    virtual bool cast_shadows() const=0;
    
    //! Returns whether or not fractures to this object will be sealed or
    //! left open.
    virtual bool seal_fractures() const=0;
    
    //! Returns the fracture count of this object.
    virtual size_t fracture_count() const=0;
    
    //! Sets the material used to render this object.
    //! @param material a pointer to the material
    virtual void material(Material* material)=0;
    
    //! Sets the mesh used to render this object.
    //! @param mesh the mesh
    virtual void mesh(Mesh* mesh)=0;

    //! Sets the material used to render this object by name.
    //! @param name the name of the material
    virtual void material(const std::string& name)=0;
    
    //! Sets the mesh used to render this object by name.
    //! @param name the name of the mesh
    virtual void mesh(const std::string& name)=0;
	    
    //! Sets whether or not this object casts shadows.
    //! @param shadows true if the object should cast shadows
    virtual void cast_shadows(bool shadows)=0;
    
    //! Returns whether or not fractures to this object will be sealed or
    //! left open.  Sealing the object requires extra processing during a
    //! fracture, and disables double-sided rendering for the fractured
    //! object.
    //! @param seal true if fractures should be sealed to keep the object
    //! solid-looking
    virtual void seal_fractures(bool seal)=0;
    
    //! Sets the fracture count.  This is how many times a fracture object
    //! and its children can be split before it is considered atomic.  Every
    //! time fracture is called on a fracture object, the fracture count will
    //! be reduced by one.  When it reaches zero, subsequent calls to fracture
    //! will be ignored.
    virtual void fracture_count(size_t count)=0;
    
    //! Adds a fracture to this fracture object.  The object will effectively
    //! be split in half along the given plane.  The smaller half of the
    //! object will be spun off into a new node with a parent name derived
    //! from the name of the current parent.  It will be a peer to the parent
    //! node in the scene graph.  The larger half will bestored in this
    //! fracture object and remain with the original parent node.
    //! @param plane the plane to split the object along
    virtual void fracture(const Plane& plane)=0;
};

}