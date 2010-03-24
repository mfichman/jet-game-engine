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
#include <Jet/Object.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Iterator.hpp>
#include <string>

namespace Jet {

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
class SceneNode : public Object {
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
    virtual ControlScript* control_script(const std::string& name)=0;
    
    //! Returns the rigid body that is a child of this scene node.
    virtual RigidBody* rigid_body()=0;

    //! Returns the network monitor that is a child of this scene node.
    virtual NetworkMonitor* network_monitor()=0;
    
    //! Returns all particle systems attached to this scene node.
    virtual Iterator<ParticleSystem> particle_systems() const=0;
    
    //! Returns all mesh objects attached to this scene node.
    virtual Iterator<MeshObject> mesh_objects() const=0;
    
    //! Returns all quads attached to this scene node.
    virtual Iterator<Quad> quads() const=0;
    
    //! Returns all audio sources attached to this scene node.
    virtual Iterator<AudioSource> audio_sources() const=0;

    //! Returns the set of all scene nodes attached to this scene node.
    virtual Iterator<SceneNode> scene_nodes() const=0;

    //! Returns the set of all controllers attached to this scene node.
    virtual Iterator<ControlScript> control_scripts() const=0;
    
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
    virtual ControlScript* add_control_script(const std::string& name, const std::string& type)=0;

    //! Adds a controller to this node.
    //! @param name the name of the controller
    //! @param controller a pointer to the controler object
    virtual ControlScript* control_script(const std::string& name, ControlScript* script)=0;

    //! Returns the current position of this scene node relative to its parent.
    virtual const Vector& position() const=0;

    //! Returns the current rotation of this scene node relative to its parent.
    virtual const Quaternion& rotation() const=0;

    //! Gets the coordinate space for this object.
    virtual CoordinateSpace coordinate_space() const=0;

    //! Sets the current position of this scene node relative to its parent.
    //! @param v the new position
    virtual void position(const Vector& v)=0;

    //! Sets the current rotation of this scene node relative to its parent.
    //! @param v the new rotation
    virtual void rotation(const Quaternion& v)=0;
    
    //! Sets the coordinate space for this object.  This can be either 
    //! CS_EYE for the coordinates to be interpreted in eye space, 
    //! CS_WORLD for the coordinates to be interpreted in world space, or
    //! CS_LOCAL for the coordinates to be interpreted in world space, 
    //! but relative to the parent scene node.  The default is CS_LOCAL.
    virtual void coordinate_space(CoordinateSpace cs)=0;
};

}
