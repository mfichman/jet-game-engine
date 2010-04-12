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

#include <Jet/Node.hpp>
#include <Jet/Engine.hpp>
#include <Jet/MeshObject.hpp>
#include <stdexcept>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace boost;

template <typename K, typename V>
V& get_value(const std::pair<K, V>& p) { 
    return const_cast<V&>(p.second); 
}

Node::Node(Engine* engine, Node* parent) :
    engine_(engine),
    parent_(parent),
    destroyed_(false) {

}

Node::Node(Engine* engine) :
    engine_(engine),
    parent_(0),
    destroyed_(false) {
}

Node* Node::node(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<Node*>(obj);
    } else {
        NodePtr node(new Node(engine_, this));
        object(name, node.get());
        return node.get();
    }
}

Light* Node::light(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<Light*>(obj);
    } else {
        LightPtr light(new Light(this));
        object(name, light.get());
        return light.get();
    }
}

MeshObject* Node::mesh_object(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<MeshObject*>(obj);
    } else {
        MeshObjectPtr mesh_object(new MeshObject(engine_, this));
        object(name, mesh_object.get());
        return mesh_object.get();
    }
}

ParticleSystem* Node::particle_system(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<ParticleSystem*>(obj);
    } else {
        ParticleSystemPtr psys(new ParticleSystem(engine_, this));
        object(name, psys.get());
        return psys.get();
    }
}

QuadSet* Node::quad_set(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<QuadSet*>(obj);
    } else {
        QuadSetPtr quad_set(new QuadSet(engine_, this));
        object(name, quad_set.get());
        return quad_set.get();
    }
}

QuadChain* Node::quad_chain(const std::string& name) {
    Object* obj = object(name);
    if (obj) {
        return dynamic_cast<QuadChain*>(obj);
    } else {
        QuadChainPtr quad_chain(new QuadChain(engine_, this));
        object(name, quad_chain.get());
        return quad_chain.get();
    }
}

Object* Node::object(const std::string& name) const {
    unordered_map<string, ObjectPtr>::const_iterator i = object_.find(name);
    if (i == object_.end()) {
        return 0;
    } else {
        return i->second.get();
    }
}

void Node::object(const std::string& name, Object* object) {
    for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        object_.insert(make_pair(name, object));
        (*i)->on_object_created(object);
    }
}

void Node::object_delete(Object* object) {
    ObjectPtr obj = object;
    for (tr1::unordered_map<string, ObjectPtr>::iterator i = object_.begin(); i != object_.end(); i++) {
        if (i->second == obj) {
            object_.erase(i);
            return;
        }
    }
}


RigidBody* Node::rigid_body() {
    if (!rigid_body_) {
        rigid_body_ = new RigidBody(engine_, this);
    }
    return rigid_body_.get();
}

AudioSource* Node::audio_source() {
    if (!audio_source_) {
        audio_source_ = new AudioSource(engine_, this);
    }
    return audio_source_.get();
}

Iterator<ObjectPtr> Node::objects() const {
	typedef unordered_map<string, ObjectPtr> map_t;
	typedef boost::function<map_t::mapped_type& (const map_t::value_type&)> fun_t;
	typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
	typedef pair<map_t::iterator, map_t::iterator> pair_t;

	itr_t begin = make_transform_iterator(object_.begin(), &get_value<string, ObjectPtr>);
	itr_t end = make_transform_iterator(object_.end(), &get_value<string, ObjectPtr>);

	return Iterator<ObjectPtr>(begin, end);
}

void Node::destroy() {
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
            parent_->object_delete(this);
            parent_ = 0;
        }
        for (vector<NodeListenerPtr>::iterator i = listener_.begin(); i < listener_.end(); i++) {
            (*i)->on_destroy();
        }
        listener_.clear(); // Break cycle between listeners and the node
    }
}

Node* Node::clone() const {
    Node* clone = new Node(engine_, parent_);
	throw runtime_error("Not implemented");

    return clone;
}
