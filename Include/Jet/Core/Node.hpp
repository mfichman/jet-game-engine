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
#include <Jet/Node.hpp>
#include <Jet/RigidBody.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Matrix.hpp>
#include <Jet/Iterator.hpp>

#include <string>
#include <stdexcept>
#include <vector>
#ifdef WINDOWS
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>

namespace Jet { namespace Core {

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
class Node : public Jet::Node {
public:    
    //! Destructor.
    virtual ~Node();
	
	//! Creates a new node at the given index.  The position and rotation of
    //! the new node will be relative to this node.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::Node* node(const std::string& name);
    
    //! Creates a new model at the given index.  Models are used for rendering
    //! static meshes with a material.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::MeshObject* mesh_object(const std::string& name);
	
	//! Creates a new fracturable model.
	//! @param name the name of the new fracturable model
	Jet::FractureObject* fracture_object(const std::string& name);
    
    //! Creates a new particle system at the given index.  Particle systems are
    //! used for fire, water, and other affects.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::ParticleSystem* particle_system(const std::string& name);
    
    //! Creates a textured quad at the given index.  Textured quads can be used
    //! for billboards.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::QuadSet* quad_set(const std::string& name);
    
    //! Creates a quad chain at the given index.  Quad chains can be used for
    //! path effects, like tracers or condensation trails.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::QuadChain* quad_chain(const std::string& name);
    
    //! Creates a light and attaches it at the given index.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Jet::Light* light(const std::string& name);
    
    //! Returns the rigid body attached to this node.
    Jet::RigidBody* rigid_body();
    
    //! Returns the audio source attached to this node.
    Jet::AudioSource* audio_source();
	
	//! Returns the camera.
	Jet::Camera* camera();
    
    //! Returns a component that is attached to this node.
    //! @param name the name of the component
    Jet::Object* object(const std::string& name);
	
	//! Returns an iterator over the objects connected to this node.
	Iterator<ObjectPtr> objects() const;
	
	  //! Returns the parent scene node
    inline Node* parent() const { 
        return parent_;
    }
    
    //! Returns the node's current rotation.
    inline const Quaternion& rotation() const {
        return rotation_;
    }
    
    //! Returns the node's current position.
    inline const Vector& position() const {
        return position_;
    }
	
		
	//! Returns the world position of the node (as of the last time it moved).
	//! Note that this is updated once per frame, at the beginning of the
	//! frame, and changing the position attribute will not immediately
	//! affect the world position of the node.
	inline const Vector& world_position() const {
		return world_position_;
	}
	
	//! Returns the world rotation of the node (as of the last time it moved).
	//! Note that this is updated once per frame, at the beginning of the
	//! frame, and changing the rotation attribute will not immedately
	//! affect the world rotation of the node.
	inline const Quaternion& world_rotation() const {
		return world_rotation_;
	}
	
	//! Returns the matrix for this node.
	inline const Matrix& matrix() const {
		return matrix_;
	}

	//! Sets the position of the node.
    //! @param position the position of the node
	void position(const Vector& position);

	//! Sets the rotation of the node.
    //! @param rotation the rotation of the node
	void rotation(const Quaternion& rotation);
    
    //! Adds a listener to this node.
    //! @param listener the node listener
    inline void listener(NodeListener* listener) {
		if (destroyed_) {
			throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
		} else {
			listener_.push_back(listener);
		}
    }
    
    //! Orients this node to point at the given position.
    //! @param target the vector to look at
    //! @param up the up vector
    void look(const Vector& target, const Vector& up=Vector(0.0f, 1.0f, 0.0f));

	//! Marks this node for destruction.  The node is removed from the scene 
	//! graph immediately, but won't be garbage collected until all references
	//! to the node are destroyed.
	void destroy();

	//! Called when this node is rendered.  This function is only called if
	//! the node is visible.
	void render();

	//! Called during a physics update.  This function is only called if the
	//! node has a rigid body attached.
	void update();
	
	//! Called to update the transform of the node
	void update_transform();
	
private:
    //! Creates a new node with no parent.
    //! @param engine the engine object
	inline Node(Engine* engine) :
		engine_(engine),
		parent_(0),
		shape_transform_(btTransform::getIdentity()),
		destroyed_(false) {
		
	}
    
    //! Creates a new node with a parent.
    //! @param engine the engine object
    //! @param parent the parent
	inline Node(Engine* engine, Node* parent) :
		engine_(engine),
		parent_(parent),
		rigid_body_(parent->rigid_body_),
		shape_transform_(parent->shape_transform_),
		destroyed_(false),
		transform_dirty_(false) {
	}
    
    //! Removes an object from this node.
    //! @param object the object to remove
	void object_delete(Object* object);
    
    //! Addes a new object to the node.
    void object(const std::string& name, Object* object);
	
	//! Recursively attaches a rigid body to this node and to all child nodes.
	//! Note that if any child nodes have rigid bodies, they will be destroyed
	//! and the new rigid body will become the rigid body for that node.
	//! @param node the node to attach the body to
	void update_collision_shapes();
  
	Engine* engine_;
    Node* parent_;
    Vector position_;
    Quaternion rotation_;
	Vector world_position_;
	Quaternion world_rotation_;
	Matrix matrix_;
    
    Jet::RigidBodyPtr rigid_body_;
    Jet::AudioSourcePtr audio_source_;
	Jet::CameraPtr camera_;
    std::tr1::unordered_map<std::string, ObjectPtr> object_;
    std::vector<NodeListenerPtr> listener_;
	btTransform shape_transform_;
    bool destroyed_;
	bool transform_dirty_;

	friend class Engine;
    friend class MeshObject;
    friend class ParticleSystem;
    friend class QuadSet;
    friend class QuadChain;
    friend class Light;
    friend class RigidBody;
    friend class AudioSource;
};

}}
