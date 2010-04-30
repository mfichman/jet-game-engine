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
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Core/Material.hpp>
#include <Jet/Core/Mesh.hpp>
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/RigidBody.hpp>
#include <Jet/MeshObject.hpp>
#include <map>

namespace Jet { namespace Core {

//! This class is used to display an instanced mesh on the screen.
//! @class MeshObject
//! @brief Displays an instanced mesh.
class MeshObject : public Jet::MeshObject {
public:
    //! Destructor.
    virtual ~MeshObject() {}
    
    //! Returns the parent of this object.
    inline Node* parent() const {
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
    inline const boost::any& shader_param(const std::string& name) {
        return shader_param_[name];
    }
    
    //! Sets the material used to render this object.
    //! @param material a pointer to the material
    inline void material(Jet::Material* material) {
        material_ = static_cast<Material*>(material);
    }
    
    //! Sets the mesh used to render this object.
    //! @param mesh the mesh
	inline void mesh(Jet::Mesh* mesh) {
		mesh_ = static_cast<Mesh*>(mesh);
	}
    
    //! Sets whether or not this object casts shadows.
    //! @param shadows true if the object should cast shadows
    inline void cast_shadows(bool shadows) {
        cast_shadows_ = shadows;
    }
    
    //! Sets the material used to render this object by name.
    //! @param name the name of the material
    inline void material(const std::string& name) {
		material(static_cast<Material*>(engine_->material(name)));
	}
    
    //! Sets the mesh used to render this object by name.
    //! @param name the name of the mesh
    inline void mesh(const std::string& name) {
		mesh(static_cast<Mesh*>(engine_->mesh(name)));
	}
    
    //! Sets the shader parameter at the given location.
    //! @param name the param name
    //! @param param the parameter
    inline void shader_param(const std::string& name, const boost::any& param) {
        shader_param_[name] = param;
    }
    
private:
    inline MeshObject(Engine* engine, Node* parent) :
		engine_(engine),
		parent_(parent),
		cast_shadows_(true) {
	}
    
    Engine* engine_;
    Node* parent_;
	MaterialPtr material_;
    MeshPtr mesh_;
    std::map<std::string, boost::any> shader_param_;
    bool cast_shadows_;

    friend class Engine;
	friend class Node;
};

}}