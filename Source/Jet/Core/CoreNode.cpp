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

#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreParticleSystem.hpp>
#include <Jet/Core/CoreCamera.hpp>
#include <Jet/Core/CoreLight.hpp>
#include <Jet/Core/CoreMeshObject.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreQuadChain.hpp>
#include <Jet/Core/CoreQuadSet.hpp>
#include <Jet/Core/CoreFractureObject.hpp>
#include <stdexcept>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace boost;

template <typename K, typename V>
V& get_value(const std::pair<K, V>& p) {
	// Small transform function used for transforming the object
	// itrator.
    return const_cast<V&>(p.second); 
}

CoreNode::~CoreNode() {
	if (!destroyed_) {
		// If the node hasn't been destroyed, then mark it as destroyed
		// and notify all the listeners to the node.  This case can
		// happen if the parent node is destroyed.
		destroyed_ = true;
		if (listener_) {
			listener_->on_destroy();
		}
	}
}

Object* CoreNode::object(const std::string& name)  {
	// Find the given object.  Empty names are invalid, so return
	// null for the empty string.
	if (name.empty()) {
		return 0;
	}
	
	// Search for the object, and return 0 if it wasn't found.
    unordered_map<string, ObjectPtr>::const_iterator i = object_.find(name);
    if (i == object_.end()) {
        return 0;
    } else {
        return i->second.get();
    }
}

RigidBody* CoreNode::rigid_body() {
	// Create the rigid body if it hasn't been loaded yet.  Note that when the
	// rigid body is created, shape transforms are also updated for collision
	// detection.  Thus, all geometry must be attached BEFORE the rigid body
	// is created.
	if (!rigid_body_) {
		rigid_body_ = engine_->physics()->rigid_body(this);
	}
	return rigid_body_.get();
}

AudioSource* CoreNode::audio_source() {
	// Create the audio source if it hasn't been loaded yet.
    if (!audio_source_) {
        audio_source_ = engine_->audio()->audio_source(this);
    }
    return audio_source_.get();
}

NetworkMonitor* CoreNode::network_monitor() {
	return 0;
}

void CoreNode::add_object(const std::string& name, Object* object) {
	// Add the given object to the node.  If the name is the empty string,
	// then auto-generate a name using the automatic name counter.
	if (name.empty()) {
		string auto_name = "__" + lexical_cast<string>(auto_name_counter_++);
		object_.insert(make_pair(auto_name, object));
	} else {
		object_.insert(make_pair(name, object));
	}
}

void CoreNode::delete_object(Object* object) {
	// Search through the map linearly and find the object to delete it.
	// Linear performance is acceptable given that nodes don't generally have
	// many children, and deletes are infrequent.
    for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
        if (i->second == object) {
            object_.erase(i);
            return;
        }
    }
}

CoreNode* CoreNode::node(const std::string& name) {
	return get_object<CoreNode>(name);
}

MeshObject* CoreNode::mesh_object(const std::string& name) {
	return get_object<CoreMeshObject>(name);
}

FractureObject* CoreNode::fracture_object(const std::string& name) {
	return get_object<CoreFractureObject>(name);
}

ParticleSystem* CoreNode::particle_system(const std::string& name) {
	return get_object<CoreParticleSystem>(name);
}

QuadSet* CoreNode::quad_set(const std::string& name) {
	return get_object<CoreQuadSet>(name);
}

QuadChain* CoreNode::quad_chain(const std::string& name) {
	return get_object<CoreQuadChain>(name);
}

Light* CoreNode::light(const std::string& name) {
	return get_object<CoreLight>(name);
}

Camera* CoreNode::camera(const std::string& name) {
	return get_object<CoreCamera>(name);
}

Iterator<ObjectPtr> CoreNode::objects() const {
	// Transform the unordered map iterator to a generic iterator over
	// all objects attached to this node.  This hides implementation details
	// about the node class.
	typedef unordered_map<string, ObjectPtr> map_t;
	typedef boost::function<map_t::mapped_type& (const map_t::value_type&)> fun_t;
	typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
	typedef pair<map_t::iterator, map_t::iterator> pair_t;

	itr_t begin = make_transform_iterator(object_.begin(), &get_value<string, ObjectPtr>);
	itr_t end = make_transform_iterator(object_.end(), &get_value<string, ObjectPtr>);

	return Iterator<ObjectPtr>(begin, end);
}

void CoreNode::listener(NodeListener* listener) {
	// Add the listener if the node hasn't been destroyed already.  This keeps
	// listeners from being added while the node is being reclaimed.
	if (destroyed_) {
		throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
	} else {
		listener_ = listener;
	}
}

Vector CoreNode::linear_velocity() const {
	// Return the linear velocity of the rigid body.  Return a zero vector
	// if this mesh does not have a rigid body.
	if (rigid_body_) {
		return rigid_body_->linear_velocity();
	} else {
		return Vector();
	}
}

void CoreNode::position(const Vector& position) {
	// Set the new position and mark the transform as dirty
	position_ = position;
	transform_modified_count_++;
		
	// If the rigid body exists, and this node is the parent of the
	// rigid body, then set the transform for the rigid body.
	if (rigid_body_ && rigid_body_->parent() == this) {
		rigid_body_->position(position);
	}
}

void CoreNode::rotation(const Quaternion& rotation) {
	// Set the new rotation and mark the transform as dirty
	rotation_ = rotation;
	transform_modified_count_++;
		
		// If the rigid body exists, and this node is the parent of the
		// rigid body, then set the transform for the rigid body.
	if (rigid_body_ && rigid_body_->parent() == this) {
		rigid_body_->rotation(rotation);
	}
}

void CoreNode::visible(bool visible) {
	if (visible_ != visible) {
		visible_ = visible;

		// If the node is the parent of the rigid body, and the rigid body
		// exists, then making an object invisible should also disable
		// physics calculations on the rigid body.
		if (rigid_body_ && rigid_body_->parent() == this) {
			static_cast<RigidBody*>(rigid_body_.get())->active(visible_);
		}
	}
}

void CoreNode::look(const Vector& target, const Vector& up) {
    Vector zaxis = (target - position_).unit();
    Vector xaxis = (up.cross(zaxis)).unit();
    Vector yaxis = (zaxis.cross(xaxis)).unit();
    
    rotation_ = Quaternion(xaxis, yaxis, zaxis);   
}

void CoreNode::signal(const Signal& signal) {
	if (true /*!network_monitor*/) {
		// Send signal immediately to the object, or else delay the object.
		if (listener_) {
			listener_->on_signal(signal);
		}
	}
}

void CoreNode::destroy() {
    if (!destroyed_) {
		destroyed_ = true;
        
        // Keep a reference to this node until the end of this function,
        // because all references to this node may be released before
        // finalization happens.
        NodePtr self(this);
        
        // Remove this node from the parent
        if (parent_) {
            parent_->delete_object(this);
            parent_ = 0;
        }
		
		// Notify all listeners that this node will be deleted.
        if (listener_) {
            listener_->on_destroy();
        }
		
		// Break cycle between listeners and the node.  If the listeners have
		// a strong reference to this node, they will still be reclaimed
		// properly.
        listener_.reset(); 
    }
}

void CoreNode::render() {
	// Handle a render event by notifying all listeners
    if (listener_) {
		listener_->on_render();
	}
}

void CoreNode::collision(Node* node) {
	// Handle a collision event by notifying all listeners
    if (listener_) {
		listener_->on_collision(node);
	}
}

void CoreNode::fracture(Node* node) {
	// Handle a fracture event (i.e., a new node is created using this
	// node as a template of some kind)
    if (listener_) {
		listener_->on_fracture(node);
	}
}

void CoreNode::update() {	
	// Calculate the transform for this node if the transform is dirty.
	update_transform();
	
	// Update all child nodes, and their transforms
	for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
		const type_info& info = typeid(*i->second);
		if (typeid(CoreNode) == info) {
			CoreNode* node = static_cast<CoreNode*>(i->second.get());
			node->update();
		}
	}
	
	// Notify all listeners that a tick is happening
    if (listener_) {
		listener_->on_update(engine_->frame_delta());
	}
}

void CoreNode::tick() {
	// Calculate the transform for this node if the transform is dirty.
	update_transform();
	
	// Update all child nodes, and their transforms
	for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
		const type_info& info = typeid(*i->second);
		if (typeid(CoreNode) == info) {
			CoreNode* node = static_cast<CoreNode*>(i->second.get());
			node->tick();
		}
	}
	
	// Notify listeners that an update is happening
    if (listener_) {
		listener_->on_tick();
	}
}

void CoreNode::update_transform() {
	bool needs_update = false;
	
	if (parent_ && parent_->transform_modified_count_ != transform_update_count_) {
		// If the parent transform has been recalculated more times than
		// the node's data hs been modified, then we know that our
		// transform is out of date.  Therefore, we set the transform counts
		// equal to the parent node count and recalculate our
		transform_update_count_ = parent_->transform_modified_count_;
		//transform_modified_count_ = parent_->transform_modified_count_;
		needs_update = true;
	} else if (transform_modified_count_ != transform_update_count_) {
		// If the node has been modified since the last update, then we
		// need to update the transform.
		transform_update_count_ = transform_modified_count_;
		needs_update = true;
	}
	
	// Calculate the absolute transform for this node in world space (i.e., not
	// relative to the parent)
	if (needs_update) {
		if (parent()) {
			matrix_ = parent()->matrix() * Matrix(rotation_, position_);
		} else {
			matrix_ = Matrix(rotation_, position_);
		}
		world_position_ = matrix_.origin();
		world_rotation_ = matrix_.rotation();
	}
}
