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

#include <Jet/Core/Node.hpp>
#include <Jet/Core/AudioSource.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Core/Light.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Core/ParticleSystem.hpp>
#include <Jet/Core/QuadChain.hpp>
#include <Jet/Core/QuadSet.hpp>
#include <Jet/Core/RigidBody.hpp>
#include <Jet/Core/FractureObject.hpp>

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
    return const_cast<V&>(p.second); 
}

Core::Node::~Node() {
	if (!destroyed_) {
		destroyed_ = true;
		for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i < listener_.end(); i++) {
            (*i)->on_destroy();
        }
	}
	listener_.clear();
}

Object* Core::Node::object(const std::string& name)  {
	if (name.empty()) {
		return 0;
	}
	
    unordered_map<string, ObjectPtr>::const_iterator i = object_.find(name);
    if (i == object_.end()) {
        return 0;
    } else {
        return i->second.get();
    }
}

RigidBody* Core::Node::rigid_body() {
	if (!rigid_body_) {
		rigid_body_ = new RigidBody(engine_, this);
	}
	return rigid_body_.get();
}

AudioSource* Core::Node::audio_source() {
    if (!audio_source_) {
        audio_source_ = new AudioSource(engine_, this);
    }
    return audio_source_.get();
}

void Core::Node::add_object(const std::string& name, Object* object) {
	if (name.empty()) {
		string auto_name = "__" + lexical_cast<string>(auto_name_counter_++);
		object_.insert(make_pair(auto_name, object));
	} else {
		object_.insert(make_pair(name, object));
	}
}

void Core::Node::delete_object(Object* object) {
    ObjectPtr obj = object;
    for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
        if (i->second == obj) {
            object_.erase(i);
            return;
        }
    }
}

//! Returns the object with the given name.  If the typeid does not match,
//! the method throws an exception.  If the object does not exist, the
//! object will be created using the given typeid.
Object* Core::Node::object(const std::type_info& type, const std::string& name) {
	if (typeid(Jet::RigidBody) == type) {
		if (!rigid_body_) {
			rigid_body_ = new Core::RigidBody(engine_, this);
		}
		return rigid_body_.get();
	} else if (typeid(Jet::AudioSource) == type) {
		if (!audio_source_) {
			audio_source_ = new Core::AudioSource(engine_, this);
		}
		return audio_source_.get();
	}
	
	Object* obj = object(name);
	if (!obj) {
		if (typeid(Jet::Node) == type) {
			obj = new Core::Node(engine_, this);
		} else if (typeid(Jet::MeshObject) == type) {
			obj = new Core::MeshObject(engine_, this);
		} else if (typeid(Jet::FractureObject) == type) {
			obj = new Core::FractureObject(engine_, this);
		} else if (typeid(Jet::ParticleSystem) == type) {
			obj = new Core::ParticleSystem(engine_, this);
		} else if (typeid(Jet::QuadSet) == type) {
			obj = new Core::QuadSet(engine_, this);
		} else if (typeid(Jet::QuadChain) == type) {
			obj = new Core::QuadChain(engine_, this);
		} else if (typeid(Jet::Light) == type) {
			obj = new Core::Light(this);
		} else if (typeid(Jet::Camera) == type) {
			obj = new Core::Camera(engine_, this);
		}
		
		if (!obj) {
			throw runtime_error("Could not create object: " + name);
		} else {
			add_object(name, obj);
		}
	} else if (!type.before(typeid(obj))) {
		throw runtime_error("Invalid object type: " + name);
	}
	
	return obj;
}

Iterator<ObjectPtr> Core::Node::objects() const {
	typedef unordered_map<string, ObjectPtr> map_t;
	typedef boost::function<map_t::mapped_type& (const map_t::value_type&)> fun_t;
	typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
	typedef pair<map_t::iterator, map_t::iterator> pair_t;

	itr_t begin = make_transform_iterator(object_.begin(), &get_value<string, ObjectPtr>);
	itr_t end = make_transform_iterator(object_.end(), &get_value<string, ObjectPtr>);

	return Iterator<ObjectPtr>(begin, end);
}


void Core::Node::position(const Vector& position) {
	if (position_ == position) {
		return;
	} else {
		position_ = position;
		transform_dirty_ = true;
		if (rigid_body_ && rigid_body_->parent() == this) {
			RigidBody* rigid_body = static_cast<RigidBody*>(rigid_body_.get());
			btTransform transform = rigid_body->body_->getCenterOfMassTransform();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			rigid_body->body_->setCenterOfMassTransform(transform);
		}
	}
}

void Core::Node::rotation(const Quaternion& rotation) {
	if (rotation == rotation_) {
		return;
	} else {
		rotation_ = rotation;
		transform_dirty_ = true;
		if (rigid_body_ && rigid_body_->parent() == this) {
			RigidBody* rigid_body = static_cast<RigidBody*>(rigid_body_.get());
			btTransform transform = rigid_body->body_->getCenterOfMassTransform();
			transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
			rigid_body->body_->setCenterOfMassTransform(transform);
		}
	}
}

void Core::Node::look(const Vector& target, const Vector& up) {
    Vector zaxis = (target - position_).unit();
    Vector xaxis = (up.cross(zaxis)).unit();
    Vector yaxis = (zaxis.cross(xaxis)).unit();
    
    rotation_ = Quaternion(xaxis, yaxis, zaxis);   
}

void Core::Node::destroy() {
    if (destroyed_) {
        return;
    } else {
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
        for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
            (*i)->on_destroy();
        }
        listener_.clear(); // Break cycle between listeners and the node
    }
}

void Core::Node::render() {
    for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i < listener_.end(); i++) {
		(*i)->on_render();
	}
}

void Core::Node::collision(Jet::Node* node) {
    for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i < listener_.end(); i++) {
		(*i)->on_collision(node);
	}
}

void Core::Node::fracture(Jet::Node* node) {
    for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i < listener_.end(); i++) {
		(*i)->on_fracture(node);
	}
}

void Core::Node::update() {
	
	// Calculate the transform for this node if the transform is dirty.
	if (transform_dirty_) {
		
		if (parent()) {
			matrix_ = parent()->matrix() * Matrix(rotation_, position_);
		} else {
			matrix_ = Matrix(rotation_, position_);
		}
		world_position_ = matrix_.origin();
		world_rotation_ = matrix_.rotation();
	}
	
	// Update all child nodes, and their transforms
	for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
		const type_info& info = typeid(*i->second);
		if (typeid(Node) == info) {
			Node* node = static_cast<Node*>(i->second.get());
			if (transform_dirty_) {
				node->transform_dirty_ = true;
			}
			node->update();
		}
	}
	transform_dirty_ = false;
	
	for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
		(*i)->on_update();
	}
}

void Core::Node::update_transform() {
		
	// Calculate the transform for this node if the transform is dirty.
	if (transform_dirty_) {
		
		if (parent()) {
			matrix_ = parent()->matrix() * Matrix(rotation_, position_);
		} else {
			matrix_ = Matrix(rotation_, position_);
		}
		world_position_ = matrix_.origin();
		world_rotation_ = matrix_.rotation();
	}
	
	// Update all child nodes, and their transforms
	for (unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
		const type_info& info = typeid(*i->second);
		if (typeid(Node) == info) {
			Node* node = static_cast<Node*>(i->second.get());
			if (transform_dirty_) {
				node->transform_dirty_ = true;
			}
			node->update_transform();
		}
	}
	transform_dirty_ = false;
	
}
