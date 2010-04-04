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

#include <Jet/Component.hpp>

using namespace Jet;
using namespace std;

Component::Component(const std::string& type) : 
    type_(type) {
}

const Value& Component::value(const std::string& name) const {
    map<string, Value>::const_iterator i = value_.find(name);
    if (i == value_.end()) {
        throw runtime_error(string("Could not find value '") + name + "'");
    } else {
        return i->second;
    }
}

const Value& Component::value(const std::string& name) {
    return value_[name];
}

Component* Component::component(const std::string& name) const {
    map<string, ComponentPtr>::const_iterator i = component_.find(name);
    if (i == component_.end()) {
        throw runtime_error(string("Could not find value '") + name + "'");
    } else {
        return i->second.get();
    }
}

void Component::value(const std::string& name, const Value& value) {
    value_[name] = value;
}

void Component::component(const std::string& name, Component* component) {
    component_[name] = component;
}

Iterator<ValueEntry> Component::values() {
    return Iterator<ValueEntry>(value_.begin(), value_.end());
}

Component* Component::clone() const {
    Component* clone = new Component(type_);
    
    // Copy values stored in this component
    for (map<string, Value>::const_iterator i = value_.begin(); i != value_.end(); i++) {
        clone->value_[i->first] = i->second;
    }
    
    // Copy sub-components
    for (map<string, ComponentPtr>::const_iterator i = component_.begin(); i != component_.end(); i++) {
        clone->component_[i->first] = i->second->clone();
    }
    
    clone->type_ = type_;
    return clone;   
}
