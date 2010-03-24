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
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Iterator.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/NetworkMonitor.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/RigidBody.hpp>
#include <Jet/ControlScript.hpp>
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

    //! Returns the object with the given name that is attached to this scene
    //! node.  The object may be of any type.
    //! @param 
    virtual Object* object(const std::string& name)=0;

    //! Creates an object of the given type and attaches it to this scene node.
    //! @param name the name of the new object
    //! @param type the type of the new object
    //! @param file the template file for the new object
    virtual Object* object(const std::string& name, const std::string& type, const std::string& file)=0;

    //! Returns an iterator to all objects connected to this scene node.
    virtual Iterator<Object> objects() const=0;

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

    inline MeshObject* mesh_object(const std::string& name) {
        return object_cast<MeshObject>(name);
    }

    inline AudioSource* audio_source(const std::string& name) {
        return object_cast<AudioSource>(name);
    }

    inline ControlScript* control_script(const std::string& name) {
        return object_cast<ControlScript>(name);
    }

    inline NetworkMonitor* network_monitor(const std::string& name) {
        return object_cast<NetworkMonitor>(name);
    }

    inline ParticleSystem* particle_system(const std::string& name) {
        return object_cast<ParticleSystem>(name);
    }

    inline RigidBody* rigid_body(const std::string& name) {
        return object_cast<RigidBody>(name);
    }

    inline SceneNode* scene_node(const std::string& name) {
        return object_cast<SceneNode>(name);
    }

protected:
    //! Clones this scene node
    virtual SceneNode* clone() const=0;

private:
    template <typename T>
    T* object_cast(const std::string& name) {
        return dynamic_cast<T*>(object(name));
    }
};

}
