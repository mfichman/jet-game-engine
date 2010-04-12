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
#include <Jet/Material.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Mesh.hpp>
#include <boost/any.hpp>
#include <map>

namespace Jet {

//! This class is used to display an instanced mesh on the screen.
//! @class MeshObject
//! @brief Displays an instanced mesh.
class JETAPI MeshObject : public Object {
public:
    //! Destructor.
    virtual ~MeshObject() {}
    
    //! Returns the parent of this object.
    Node* parent() const {
        return parent_;
    }
    
    //! Returns the material used to render this object.
    inline Material* material() const {
        return material_.get();
    }
    
    //! Returns the mesh used to render this object.
    inline Mesh* mesh() const {
        return mesh_.get();
    }
    
    //! Returns true if this object casts shadows.
    inline bool cast_shadows() const {
        return cast_shadows_;
    }
    
    //! Returns the shader parameter at the given location.
    //! @param name the param name
    const boost::any& shader_param(const std::string& name) {
        return shader_param_[name];
    }
    
    //! Sets the material used to render this object.
    //! @param material a pointer to the material
    inline void material(Material* material) {
        material_ = material;
    }
    
    //! Sets the mesh used to render this object.
    //! @param mesh the mesh
    inline void mesh(Mesh* mesh) {
        mesh_ = mesh;
    }
    
    //! Sets whether or not this object casts shadows.
    //! @param shadows true if the object should cast shadows
    inline void cast_shadows(bool shadows) {
        cast_shadows_ = shadows;
    }
    
    //! Sets the material used to render this object by name.
    //! @param name the name of the material
    void material(const std::string& name);
    
    //! Sets the mesh used to render this object by name.
    //! @param name the name of the mesh
    void mesh(const std::string& name);
    
    //! Sets the shader parameter at the given location.
    //! @param name the param name
    //! @param param the parameter
    inline void shader_param(const std::string& name, const boost::any& param) {
        shader_param_[name] = param;
    }
    
private:
    MeshObject(Engine* engine, Node* parent);
    
    Engine* engine_;
    Node* parent_;
#pragma warning(disable:4251)
    MaterialPtr material_;
    MeshPtr mesh_;
    std::map<std::string, boost::any> shader_param_;
#pragma warning(default:4251)
    bool cast_shadows_;

    friend class Engine;
	friend class Node;
};

}