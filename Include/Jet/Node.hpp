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
#include <Jet/Matrix.hpp>
#include <string>
#include <vector>
#include <unordered_map>

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
class JETAPI Node : public Object {
public:    
    //! Destructor.
    virtual ~Node() {}
    
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
    
    //! Returns the node's current transformation matrix.  Note that this is
    //! automatically updated from the position and rotation variables once
    //! per frame.
    inline const Matrix& matrix() const {
        return matrix_;
    }
    
    //! Creates a new node at the given index.  The position and rotation of
    //! the new node will be relative to this node.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Node* node(const std::string& name);
    
    //! Creates a new model at the given index.  Models are used for rendering
    //! static meshes with a material.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    MeshObject* mesh_object(const std::string& name);
    
    //! Creates a new particle system at the given index.  Particle systems are
    //! used for fire, water, and other affects.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    ParticleSystem* particle_system(const std::string& name);
    
    //! Creates a textured quad at the given index.  Textured quads can be used
    //! for billboards.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    QuadSet* quad_set(const std::string& name);
    
    //! Creates a quad chain at the given index.  Quad chains can be used for
    //! path effects, like tracers or condensation trails.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    QuadChain* quad_chain(const std::string& name);
    
    //! Creates a light and attaches it at the given index.
    //! @param index the index where the new object will be placed; if this
    //! parameter is zero then the object will be created at any free location
    Light* light(const std::string& name);
    
    //! Returns the rigid body attached to this node.
    RigidBody* rigid_body();
    
    //! Returns the audio source attached to this node.
    AudioSource* audio_source();
    
    //! Returns a component that is attached to this node.
    //! @param name the name of the component
    Object* object(const std::string& name) const;

    //! Returns a list of all objects attached to this node.  This function is
    //! used to walk over all objects in the tree using dynamic dispace.
    //! See the component class.
    Iterator<ObjectPtr> objects() const;

	//! Sets the position of the node.
    //! @param position the position of the node
	inline void position(const Vector& position) {
		position_ = position;
	}

	//! Sets the rotation of the node.
    //! @param rotation the rotation of the node
	inline void rotation(const Quaternion& rotation) {
		rotation_ = rotation;
	}
    
    //! Adds a listener to this node.
    //! @param listener the node listener
    inline void listener(NodeListener* listener) {
		if (destroyed_) {
			throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
		} else {
			listener_.push_back(listener);
		}
    }

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

	//! Clones this node.
	Node* clone() const;
    
private:
    //! Creates a new node with no parent.
    //! @param engine the engine object
	Node(Engine* engine);
    
    //! Creates a new node with a parent.
    //! @param engine the engine object
    //! @param node the parent
	Node(Engine* engine, Node* node);
    
    //! Removes an object from this node.
    //! @param object the object to remove
	void object_delete(Object* object);
    
    //! Addes a new object to the node.
    void object(const std::string& name, Object* object);
    
    //! Sets the transformation matrix for this node.
    //! @param matrix the transformation matrix
    inline void matrix(const Matrix& matrix) {
        matrix_ = matrix;
    }
    
	Engine* engine_;
    Node* parent_;
    Vector position_;
    Quaternion rotation_;
    Matrix matrix_;
    
#pragma warning(disable:4251)
    RigidBodyPtr rigid_body_;
    AudioSourcePtr audio_source_;
    std::tr1::unordered_map<std::string, ObjectPtr> object_;
    std::vector<NodeListenerPtr> listener_;
#pragma warning(default:4251)
    bool destroyed_;

	friend class Engine;
    friend class MeshObject;
    friend class ParticleSystem;
    friend class QuadSet;
    friend class QuadChain;
    friend class Light;
    friend class RigidBody;
    friend class AudioSource;
};

//! Listens for node events.  Examples include on_pre_render (called before
//! rendering), on_post_render (called after rendering), on_render (called
//! during rendering), and on_update (called with each tick of the physics
//! engine.
//! @class NodeListener
//! @brief Interface for handling node events.
class JETAPI NodeListener : public Object {
public: 
    //! Destructor.
    virtual ~NodeListener() {}

    //! Called for each physics update.
    virtual void on_update() {}
    
    //! Called during each frame if the node is visible.
    virtual void on_render() {}
    
    //! Called when a colllision is detected by the physics engine.
    virtual void on_collision() {}
    
    //! Called when the node is destroyed.
    virtual void on_destroy() {}
    
    //! Called when the position of the node changes.
    virtual void on_position() {}
    
    //! Called when the rotation of the node changes.
    virtual void on_rotation() {}
    
    //! Called when a new object is added to this node.
    virtual void on_object_created(Object* object) {}
    
    //! Called when an object is removed from this node.
    virtual void on_object_destroyed(Object* object) {}
};

}
