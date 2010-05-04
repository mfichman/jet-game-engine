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
#include <Jet/Core/Node.hpp>
#include <Jet/Core/Mesh.hpp>
#include <Jet/Core/Material.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/FractureObject.hpp>
#include <vector>

namespace Jet { namespace Core {

//! This class is used to display an instanced mesh on the screen.  The mesh
//! can be fractured, so it has its own index buffer and physics geometry
//! cache.
//! @class FractureObject
//! @brief Displays an fracturable instanced mesh.
class FractureObject : public Jet::FractureObject {
public:
    
    //! Destroys this fracture object
    ~FractureObject();
    
    //! Returns the parent of this object.
    inline Node* parent() const {
        return parent_;
    }
    
    //! Returns the material used to render this object.
    inline Material* material() const {
        return mesh_object_->material();
    }
    
    //! Returns the mesh used to render this object.
    inline Mesh* mesh() const {
        return mesh_object_->mesh();
    }
    
    //! Returns true if this object casts shadows.
    inline bool cast_shadows() const {
        return mesh_object_->cast_shadows();
    }
    
    //! Returns whether or not fractures to this object will be sealed or
    //! left open.
    inline bool seal_fractures() const {
        return seal_fractures_;
    }
    
    //! Returns the fracture count of this object.
    inline size_t fracture_count() const {
        return fracture_count_;
    }

    
    //! Sets the material used to render this object.
    //! @param material a pointer to the material
    inline void material(Jet::Material* material) {
        mesh_object_->material(material);
    }
    
    //! Sets the mesh used to render this object.
    //! @param mesh the mesh
    inline void mesh(Jet::Mesh* mesh) {
        mesh_object_->mesh(mesh);
    }

    //! Sets the material used to render this object by name.
    //! @param name the name of the material
    inline void material(const std::string& name) {
        mesh_object_->material(name);
    }
    
    //! Sets the mesh used to render this object by name.
    //! @param name the name of the mesh
    inline void mesh(const std::string& name) {
        mesh_object_->mesh(name);
    }
    
    //! Sets whether or not this object casts shadows.
    //! @param shadows true if the object should cast shadows
    inline void cast_shadows(bool shadows) {
        mesh_object_->cast_shadows(shadows);
    }
    
    //! Returns whether or not fractures to this object will be sealed or
    //! left open.  Sealing the object requires extra processing during a
    //! fracture, and disables double-sided rendering for the fractured
    //! object.
    //! @param seal true if fractures should be sealed to keep the object
    //! solid-looking
    inline void seal_fractures(bool seal) {
        seal_fractures_ = seal;
    }
    
    //! Sets the fracture count.  This is how many times a fracture object
    //! and its children can be split before it is considered atomic.  Every
    //! time fracture is called on a fracture object, the fracture count will
    //! be reduced by one.  When it reaches zero, subsequent calls to fracture
    //! will be ignored.
    inline void fracture_count(size_t count) {
        fracture_count_ = count;
    }
    
    //! Adds a fracture to this fracture object.  The object will effectively
    //! be split in half along the given plane.  The smaller half of the
    //! object will be spun off into a new node with a parent name derived
    //! from the name of the current parent.  It will be a peer to the parent
    //! node in the scene graph.  The larger half will bestored in this
    //! fracture object and remain with the original parent node.
    //! @param plane the plane to split the object along
    void fracture(const Plane& plane);
    
private:
    inline FractureObject(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent),
        seal_fractures_(false),
        fracture_count_(0) {
            
            
        mesh_object_ = static_cast<MeshObject*>(parent_->mesh_object());
    }
    
    FractureObject* create_clone();
    void fracture_indices(const Plane& plane);
    
    Engine* engine_;
    Node* parent_;
    MeshObjectPtr mesh_object_;
    bool seal_fractures_;
    size_t fracture_count_;
    
    friend class Node;
};

}}