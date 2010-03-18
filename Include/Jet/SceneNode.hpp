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

#include <Jet/Jet.hpp>
#include <Jet/SceneComponent.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <any_iterator/any_iterator.hpp>
#include <string>
#include <memory>
#include <map>

namespace Jet {
    using namespace IteratorTypeErasure;

//! This class manages dynamic objects in the scene.  Many different objects
//! may be created through this class in a hierarchical fashion.  The positions
//! and rotations of all objects are relative to their parent scene node.
//! Note that objects other than child scene nodes cannot be created directly;
//! they are loaded from the property table for the scene node.  Objects other
//! than scene cannot be destroyed unless their parent scene node is destroyed.
//! However, some objects can be disabled.  All subcomponents of a scene node
//! must have a name unique to this scene node, regardless of type.
//! @class SceneNode
//! @brief Represents a node in the scene graph.
class JETAPI SceneNode : public SceneComponent {
public:
    
    //! Destructor.
    virtual ~SceneNode() {}
    
    //! Returns the parent scene node
    virtual SceneNode* parent() const=0;
    //! Returns a particle system that is a child of this scene node
    //! @param the name of the particle system
    virtual ParticleSystem* particle_system(const std::string& name)=0;

    //! Returns a mesh object that is a child of this scene node.
    //! @param the name of the mesh object
    virtual MeshObject* mesh_object(const std::string& name)=0;

    //! Returns a quad that is a child of this scene node.
    //! @param the name of the quad
    virtual Quad* quad(const std::string& name)=0;

    //! Returns the audio speaker that is a child of this scene node.
    virtual AudioSource* audio_source(const std::string& name)=0;

    //! Gets the scene node that is a child of this scene node.
    //! @param name the name of the scene node
    virtual SceneNode* scene_node(const std::string& name)=0;

    //! Returns the controller with the given name.
    //! @param name the name of the controller
    virtual Controller* controller(const std::string& name)=0;
    
    //! Returns the rigid body that is a child of this scene node.
    virtual RigidBody* rigid_body(const std::string& type="")=0;

    //! Returns the network monitor that is a child of this scene node.
    virtual NetworkMonitor* network_monitor(const std::string& type="")=0;
    
    //! Returns all particle systems attached to this scene node.
    virtual any_iterator<ParticleSystem*> particle_systems() const=0;
    
    //! Returns all mesh objects attached to this scene node.
    virtual any_iterator<MeshObject*> mesh_objects() const=0;
    
    //! Returns all quads attached to this scene node.
    virtual any_iterator<Quad*> quads() const=0;
    
    //! Returns all audio sources attached to this scene node.
    virtual any_iterator<AudioSource*> audio_sources() const=0;

    //! Returns the set of all scene nodes attached to this scene node.
    virtual any_iterator<SceneNode*> scene_nodes() const=0;

    //! Returns the set of all controllers attached to this scene node.
    virtual any_iterator<Controller*> controllers() const=0;
    
    //! Adds a particle system to this scene node.
    //! @param name the name of the system
    //! @param type the type of the system
    virtual ParticleSystem* add_particle_system(const std::string& name, const std::string& type)=0;
    
    //! Adds a mesh object to this scene node.
    //! @param name the name
    //! @param type the type
    virtual MeshObject* add_mesh_object(const std::string& name, const std::string& type)=0;
    
    //! Adds a quad to this scene node.
    //! @param name the name
    //! @param type the type
    virtual Quad* add_quad(const std::string& name, const std::string& type)=0;
    
    //! Adds an audio source to this node.
    //! @param name the name
    //! @param type the type
    virtual AudioSource* add_audio_source(const std::string& name, const std::string& type)=0; 

    //! Returns a scene node that is a child of this scene node.
    //! @param the name of the child scene node
    virtual SceneNode* add_scene_node(const std::string& name, const std::string& type)=0;

    //! Adds a controller to this node.
    //! @param name the name the controller will have
    //! @param type the type of controller to add
    virtual Controller* add_controller(const std::string& name, const std::string& type)=0;

    //! Returns the current position of this scene node relative to its parent.
    virtual const Vector& position() const=0;

    //! Returns the current rotation of this scene node relative to its parent.
    virtual const Quaternion& rotation() const=0;

    //! Sets the current position of this scene node relative to its parent.
    //! @param v the new position
    virtual void position(const Vector& v)=0;

    //! Sets the current rotation of this scene node relative to its parent.
    //! @param v the new rotation
    virtual void rotation(const Quaternion& v)=0;
};


}
