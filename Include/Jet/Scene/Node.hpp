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

//! This class manages dynamic objects in the scene.  Many different objects
//! may be created through this class in a hierarchical fashion.  The positions
//! and rotations of all objects are relative to their parent scene node.
//! Note that objects other than child scene nodes cannot be created directly;
//! they are loaded from the property table for the scene node.  Objects other
//! than scene cannot be destroyed unless their parent scene node is destroyed.
//! However, some objects can be disabled.  All subcomponents of a scene node
//! must have a name unique to this scene node, regardless of type.
//! @class Node
//! @brief Represents a node in the scene graph.
class Node : public Object {
public:        
    //! Returns the parent scene node
    virtual Node* parent() const=0;
	
	//! Returns true if the node is visible.
	virtual bool visible() const=0;

	//! Returns true if the node is marked for deletion
	virtual bool destroyed() const=0;
    
    //! Returns the node's current rotation.
    virtual const Quaternion& rotation() const=0;
    
    //! Returns the node's current position.
    virtual const Vector& position() const=0;
	
	//! Returns the world position of the node (as of the last time it moved).
	//! Note that this is updated once per frame, at the beginning of the
	//! frame, and changing the position attribute will not immediately
	//! affect the world position of the node.
	virtual const Vector& world_position() const=0;
	
	//! Returns the world rotation of the node (as of the last time it moved).
	//! Note that this is updated once per frame, at the beginning of the
	//! frame, and changing the rotation attribute will not immedately
	//! affect the world rotation of the node.
	virtual const Quaternion& world_rotation() const=0;
    
    //! Returns the node's current absolute transform
    virtual const Matrix& matrix() const=0;
    
    //! Creates a new node at the given index.  The position and rotation of
    //! the new node will be relative to this node.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    virtual Node* node(const std::string& name="")=0;
    
    //! Creates a new model.  Models are used for rendering
    //! static meshes with a material.
	//! @param name the name of the new object
    virtual MeshObject* mesh_object(const std::string& name="")=0;
	
	//! Creates a new fracturable model.
	//! @param name the name of the new fracturable model
	virtual FractureObject* fracture_object(const std::string& name="")=0;
    
    //! Creates a new particle system at the given index.  Particle systems are
    //! used for fire, water, and other affects.
	//! @param name the name of the new object
    virtual ParticleSystem* particle_system(const std::string& name="")=0;
    
    //! Creates a textured quad.  Textured quads can be used
    //! for billboards.
	//! @param name the name of the new object
    virtual QuadSet* quad_set(const std::string& name="")=0;
    
    //! Creates a quad chain.  Quad chains can be used for
    //! path effects, like tracers or condensation trails.
	//! @param name the name of the new object
    virtual QuadChain* quad_chain(const std::string& name="")=0;
    
    //! Creates a light.
	//! @param name the name of the new object
    virtual Light* light(const std::string& name="")=0;
	
	//! Returns the camera attached to this node.
	virtual Camera* camera(const std::string& name="")=0;
    
    //! Creates a new collision sphere
    virtual CollisionSphere* collision_sphere(const std::string& name="")=0;
    
    //! Returns the rigid body attached to this node.
    virtual RigidBody* rigid_body()=0;
    
    //! Returns the audio source attached to this node.
    virtual AudioSource* audio_source()=0;
	
	//! Returns the network monitor attached to this node.
	virtual NetworkMonitor* network_monitor()=0;
    
    //! Returns the actor attached to this node, or creates it.
    virtual Actor* actor()=0;

    //! Returns a component that is attached to this node.  If the object
	//! does not exist, this function returns null.
    //! @param name the name of the component
    virtual Object* object(const std::string& name)=0;
	
	//! Sets whether or not the node is visible.
	virtual void visible(bool visible)=0;

	//! Sets the position of the node.
    //! @param position the position of the node
	virtual void position(const Vector& position)=0;

	//! Sets the rotation of the node.
    //! @param rotation the rotation of the node
	virtual void rotation(const Quaternion& rotation)=0;
    
    //! Orients this node to point at the given position.
    //! @param target the vector to look at
    //! @param up the up vector
    virtual void look(const Vector& target, const Vector& up)=0;

	//! Marks this node for destruction.  The node is removed from the scene 
	//! graph immediately, but won't be garbage collected until all references
	//! to the node are destroyed.
	virtual void destroy()=0;
};
}
