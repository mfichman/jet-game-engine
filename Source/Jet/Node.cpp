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
#include <Jet/Controller.hpp>
#include <stdexcept>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>

using namespace Jet;
using namespace std;
using namespace boost;

template <typename K, typename V>
V& get_value(const std::pair<K, V>& p) { 
    return const_cast<V&>(p.second); 
}

Node::Node(Engine* engine, Node* parent) :
    engine_(engine),
    parent_(parent) {

}

Node::Node(Engine* engine) :
    engine_(engine),
    parent_(0) {
}

Node* Node::node(const std::string& name, const std::string& blueprint) {
    NodePtr node(engine_->node(blueprint)->clone());
    node->parent_ = this;
    node->name_ = name;
    node_.insert(make_pair(name, node));
    return node.get();
}

Component* Node::component(const std::string& name, const std::string& blueprint) {
    ComponentPtr component(engine_->component(blueprint)->clone());
    component_.insert(make_pair(name, component));

    // Add the component to a second map so that it can be looked up by type
    // easily
    component_type_.insert(make_pair(component->type(), component));
    return component.get();
}

void Node::controller(const std::string& type) {
    ControllerPtr controller = dynamic_cast<Controller*>(engine_->object(type));
    if (!controller) {
        throw runtime_error("Expected an instance of Jet::Controller");
    } else {
        controller_.push_back(controller);
    }
}

void Node::controller(Controller* controller) {
    controller_.push_back(controller);
}

Iterator<NodePtr> Node::nodes() {
    typedef map<string, NodePtr> map_t;
    typedef function<map_t::mapped_type& (const map_t::value_type&)> fun_t;
    typedef transform_iterator<fun_t, map_t::iterator> itr_t;
    typedef pair<map_t::iterator, map_t::iterator> pair_t;

    itr_t begin = make_transform_iterator(node_.begin(), &get_value<string, NodePtr>);
    itr_t end = make_transform_iterator(node_.end(), &get_value<string, NodePtr>);

    return Iterator<NodePtr>(begin, end);
}

Iterator<const NodePtr> Node::nodes() const {
    typedef map<string, NodePtr> map_t;
    typedef function<const map_t::mapped_type& (const map_t::value_type&)> fun_t;
    typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
    typedef pair<map_t::iterator, map_t::const_iterator> pair_t;

    itr_t begin = make_transform_iterator(node_.begin(), &get_value<string, NodePtr>);
    itr_t end = make_transform_iterator(node_.end(), &get_value<string, NodePtr>);

    return Iterator<const NodePtr>(begin, end);
}

Iterator<const ComponentPtr> Node::components() const {
    typedef map<string, ComponentPtr> map_t;
    typedef function<const map_t::mapped_type& (const map_t::value_type&)> fun_t;
    typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
    typedef pair<map_t::const_iterator, map_t::const_iterator> pair_t;

    itr_t begin = make_transform_iterator(component_.begin(), &get_value<string, const ComponentPtr>);
    itr_t end = make_transform_iterator(component_.end(), &get_value<string, const ComponentPtr>);

    return Iterator<const ComponentPtr>(begin, end); 
}

Iterator<ComponentPtr> Node::components(const string& type) {
    typedef multimap<string, ComponentPtr> map_t;
    typedef function<map_t::mapped_type& (const map_t::value_type&)> fun_t;
    typedef transform_iterator<fun_t, map_t::iterator> itr_t;
    typedef pair<map_t::iterator, map_t::iterator> pair_t;

    pair_t range = component_type_.equal_range(type);
    itr_t begin = make_transform_iterator(range.first, &get_value<string, ComponentPtr>);
    itr_t end = make_transform_iterator(range.second, &get_value<string, ComponentPtr>);

    return Iterator<ComponentPtr>(begin, end);
}

Iterator<const ComponentPtr> Node::components(const string& type) const {
    typedef multimap<string, ComponentPtr> map_t;
    typedef function<const map_t::mapped_type& (const map_t::value_type&)> fun_t;
    typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
    typedef pair<map_t::const_iterator, map_t::const_iterator> pair_t;

    pair_t range = component_type_.equal_range(type);
    itr_t begin = make_transform_iterator(range.first, &get_value<string, const ComponentPtr>);
    itr_t end = make_transform_iterator(range.second, &get_value<string, const ComponentPtr>);

    return Iterator<const ComponentPtr>(begin, end);
}

void Node::destroy() {
    if (parent_) {
        parent_->remove_child(this);
        parent_ = 0;
    }
}

void Node::event(const std::string& name, const Params& params) {
     for (list<ControllerPtr>::iterator i = controller_.begin(); i != controller_.end(); i++) {
        (*i)->on_event(this, name, params);  
     }
}

void Node::update() {
    for (list<ControllerPtr>::iterator i = controller_.begin(); i != controller_.end(); i++) {
         (*i)->on_update(this);
    }
}

void Node::render() {
    for (list<ControllerPtr>::iterator i = controller_.begin(); i != controller_.end(); i++) {
        (*i)->on_render(this);
    }
}

void Node::remove_child(Node* node) {
    node_.erase(node->name_);
}

Node* Node::clone() const {
    Node* clone = new Node(engine_, parent_);

    // Clone all nodes attached to this node, recursively
    for (map<string, NodePtr>::const_iterator i = node_.begin(); i != node_.end(); i++) {
        NodePtr node(i->second->clone());
        node->parent_ = parent_;
        node->node_.insert(make_pair(i->first, node));
    }
    // Clone all components attached to this node
    for (map<string, ComponentPtr>::const_iterator i = component_.begin(); i != component_.end(); i++) {
        ComponentPtr component(i->second->clone());
        clone->component_.insert(make_pair(i->first, component));
        clone->component_type_.insert(make_pair(component->type(), component));
    }
    // TODO: Clone all controllers attached to this node
    for (list<ControllerPtr>::const_iterator i = controller_.begin(); i != controller_.end(); i++) {
        ControllerPtr controller = (*i)->clone();
        clone->controller_.push_back(controller);        
    }

    return clone;
}
