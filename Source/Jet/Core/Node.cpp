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

Node* Core::Node::node(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<Node*>(obj);
    } else {
        NodePtr node(new Core::Node(engine_, this));
        add_object(name, node.get());
        return node.get();
    }
}

Light* Core::Node::light(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<Light*>(obj);
    } else {
        LightPtr light(new Core::Light(this));
        add_object(name, light.get());
        return light.get();
    }
}

MeshObject* Core::Node::mesh_object(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<MeshObject*>(obj);
    } else {
        MeshObjectPtr mesh_object(new Core::MeshObject(engine_, this));
        add_object(name, mesh_object.get());		
        return mesh_object.get();
    }
}

FractureObject* Core::Node::fracture_object(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<FractureObject*>(obj);
    } else {
        FractureObjectPtr fracture_object(new Core::FractureObject(engine_, this));
        add_object(name, fracture_object.get());		
        return fracture_object.get();
    }
}

ParticleSystem* Core::Node::particle_system(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<ParticleSystem*>(obj);
    } else {
        ParticleSystemPtr psys(new Core::ParticleSystem(engine_, this));
        add_object(name, psys.get());
        return psys.get();
    }
}

QuadSet* Core::Node::quad_set(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<QuadSet*>(obj);
    } else {
        QuadSetPtr quad_set(new Core::QuadSet(engine_, this));
        add_object(name, quad_set.get());
        return quad_set.get();
    }
}

QuadChain* Core::Node::quad_chain(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<QuadChain*>(obj);
    } else {
        QuadChainPtr quad_chain(new Core::QuadChain(engine_, this));
        add_object(name, quad_chain.get());
        return quad_chain.get();
    }
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

Camera* Core::Node::camera() {
	if (!camera_) {
		camera_ = new Camera(engine_, this);
	}
	return camera_.get();
}

Object* Core::Node::extension(const std::string& name, const std::string& type) {
	Object* obj = object(name);
    if (obj) {
		return obj;
    } else {
		ObjectPtr object = engine_->constructor(name)(engine_, this);
        add_object(name, object.get());
        return object.get();
    }
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
