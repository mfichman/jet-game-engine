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
#include <boost/any.hpp>

namespace Jet {

//! This class is used to display an instanced mesh on the screen.
//! @class MeshObject
//! @brief Displays an instanced mesh.
class MeshObject : public Object {
public:    
    //! Returns the parent of this object.
    virtual Node* parent() const=0;
    
    //! Returns the material used to render this object.
    virtual Material* material() const=0;
    
    //! Returns the mesh used to render this object.
    virtual Mesh* mesh() const=0;
    
    //! Returns true if this object casts shadows.
    virtual bool cast_shadows() const=0;
    
    //! Returns the shader parameter at the given location.
    //! @param name the param name
    virtual const boost::any& shader_param(const std::string& name)=0;
    
    //! Sets the material used to render this object.
    //! @param material a pointer to the material
    virtual void material(Material* material)=0;
    
    //! Sets the mesh used to render this object.
    //! @param mesh the mesh
    virtual void mesh(Mesh* mesh)=0;
    
    //! Sets whether or not this object casts shadows.
    //! @param shadows true if the object should cast shadows
    virtual void cast_shadows(bool shadows)=0;
    
    //! Sets the material used to render this object by name.
    //! @param name the name of the material
    virtual void material(const std::string& name)=0;
    
    //! Sets the mesh used to render this object by name.
    //! @param name the name of the mesh
    virtual void mesh(const std::string& name)=0;
    
    //! Sets the shader parameter at the given location.
    //! @param name the param name
    //! @param param the parameter
    virtual void shader_param(const std::string& name, const boost::any& param)=0;
};

}