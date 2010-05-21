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

#include <Jet/Core/CoreTypes.hpp>
#include <Jet/Scene/Node.hpp>
#include <Jet/Scene/RigidBody.hpp>
#include <Jet/Scene/AudioSource.hpp>
#include <Jet/Scene/MeshObject.hpp>
#include <Jet/Scene/ParticleSystem.hpp>
#include <Jet/Scene/Light.hpp>
#include <Jet/Scene/QuadSet.hpp>
#include <Jet/Scene/QuadChain.hpp>
#include <Jet/Scene/FractureObject.hpp>
#include <Jet/Scene/Camera.hpp>
#include <Jet/Types/Quaternion.hpp>
#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Matrix.hpp>
#include <Jet/Types/Signal.hpp>
#include <Jet/Types/Iterator.hpp>

#include <string>
#include <stdexcept>
#include <queue>
#include <vector>
#ifdef WINDOWS
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

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
class CoreNode : public Node {
public:
	//! Creates a new node with no parent.
    //! @param engine the engine object
	inline CoreNode(CoreEngine* engine) :
		engine_(engine),
		parent_(0),
		visible_(true),
		destroyed_(false),
		transform_modified_count_(1),
		transform_update_count_(0),
		auto_name_counter_(0) {
	}
    
    //! Creates a new node with a parent.
    //! @param engine the engine object
    //! @param parent the parent
	inline CoreNode(CoreEngine* engine, CoreNode* parent) :
		engine_(engine),
		parent_(parent),
		visible_(true),
		rigid_body_(parent->rigid_body_),
		destroyed_(false),
		transform_modified_count_(1),
		transform_update_count_(0),
		auto_name_counter_(0) {
	}
	
    //! Destructor.
    virtual ~CoreNode();
	
	//! Creates a new node at the given index.  The position and rotation of
    //! the new node will be relative to this node.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    CoreNode* node(const std::string& name="");
    
    //! Creates a new model at the given index.  Models are used for rendering
    //! static meshes with a material.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    MeshObject* mesh_object(const std::string& name="");
	
	//! Creates a new fracturable model.
	//! @param name the name of the new fracturable model
	FractureObject* fracture_object(const std::string& name="");

    //! Creates a new particle system at the given index.  Particle systems are
    //! used for fire, water, and other affects.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    ParticleSystem* particle_system(const std::string& name="");
    
    //! Creates a textured quad at the given index.  Textured quads can be used
    //! for billboards.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    QuadSet* quad_set(const std::string& name="");
    
    //! Creates a quad chain at the given index.  Quad chains can be used for
    //! path effects, like tracers or condensation trails.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    QuadChain* quad_chain(const std::string& name="");
    
    //! Creates a light and attaches it at the given index.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Light* light(const std::string& name="");
	
	//! Returns the camera.
	Camera* camera(const std::string& name="");
    
    //! Returns the rigid body attached to this node.
    RigidBody* rigid_body();
    
    //! Returns the audio source attached to this node.
    AudioSource* audio_source();
	
	//! Returns the network monitor attached to this node.
	NetworkMonitor* network_monitor();
    
	//! Creates and returns an extension object, if it doesn't already exist.
	//! @param name the name of the extension object
	//! @param type the type of the object
	Object* extension(const std::string& type, const std::string& name);
	
    //! Returns a component that is attached to this node.
    //! @param name the name of the component
    Object* object(const std::string& name);
	
	//! Returns an iterator over the objects connected to this node.
	Iterator<ObjectPtr> objects() const;
	
	  //! Returns the parent scene node
    inline CoreNode* parent() const { 
        return parent_;
    }
    
	//! Returns the visibility of this node.
	inline bool visible() const {
		return visible_;
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
	
	//! Returns the linear velocity.
	Vector linear_velocity() const;
	
	//! Sets the raw position of this node
	void raw_position(const Vector& position) {
		position_ = position;
		transform_modified_count_++;
	}
	
	//! Sets the raw position of the node
	void raw_rotation(const Quaternion& rotation) {
		rotation_ = rotation;
		transform_modified_count_++;
	}
	
	//! Sets the rigid body of this node.
	void rigid_body(RigidBody* rigid_body) {
		rigid_body_ = rigid_body;
	}
	
	//! Sets the visibility of this node.
	void visible(bool visible);

	//! Sets the position of the node.
    //! @param position the position of the node
	void position(const Vector& position);

	//! Sets the rotation of the node.
    //! @param rotation the rotation of the node
	void rotation(const Quaternion& rotation);
    
    //! Adds a listener to this node.
    //! @param listener the node listener
    void listener(NodeListener* listener);
    
    //! Orients this node to point at the given position.
    //! @param target the vector to look at
    //! @param up the up vector
    void look(const Vector& target, const Vector& up=Vector(0.0f, 1.0f, 0.0f));

	//! Sends a signal to this node.  Note that if the node is owned by
	//! a remote machine, then the signal will be sent by RPC.
	void signal(const Signal& signal);

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

	//! Called to notify of a fracture event.
	void fracture(Node* node);

	//! Called to notify of a collision event.
	void collision(Node* node);
	
	//! Called once per game loop.  This is called before the
	//! render event, which also happens once per game loop.
	void tick();
	
private:
	//! Returns an object of the given type.
	template <typename T>
	T* get_object(const std::string& name) {
		T* obj = dynamic_cast<T*>(object(name));
		if (!obj) {
			obj = new T(engine_, this);
			add_object(name, obj);
		}
		return obj;
	}
	
    //! Removes an object from this node.
    //! @param object the object to remove
	void delete_object(Object* object);
    
    //! Addes a new object to the node.
    void add_object(const std::string& name, Object* object);
	
	//! Updates this node's internal matrix transform.
	void update_transform();
  
	CoreEngine* engine_;
    CoreNode* parent_;
	bool visible_;
    Vector position_;
    Quaternion rotation_;
	Vector world_position_;
	Quaternion world_rotation_;
	Matrix matrix_;
    
    RigidBodyPtr rigid_body_;
    AudioSourcePtr audio_source_;
    NodeListenerPtr listener_;
    std::tr1::unordered_map<std::string, ObjectPtr> object_;
	std::queue<std::vector<Signal> > signal_;
    bool destroyed_;
	size_t transform_modified_count_;
	size_t transform_update_count_;
	size_t auto_name_counter_;
};

}
