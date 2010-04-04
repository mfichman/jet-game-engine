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
#include <Jet/Params.hpp>
#include <map>

namespace Jet {

//! Component that can be attached to a scene node.
//! @class Component
//! @brief Component that can be attached to a scene node.
class JETAPI Component : public Object {
public:
    class Listener;
    
    //! Destructor.
    virtual ~Component() {}
    
    //! Returns the type of this component.
    inline const std::string& type() const {
        return type_;
    }
    
    inline void type(const std::string& type) {
        type_ = type;
    }

    //! Method for reading a value from the component.
    //! @param name the name of the value
    const Value& value(const std::string& name) const;
    
    //! Method for reading a value from the component.
    //! @param name the name of the value
    const Value& value(const std::string& name);
    
    //! Returns the component with the given name.
    //! @param name the name of the component
    Component* component(const std::string& name) const;

    //! Method for writing a value to the component.
    //! @param name the name of the value
    void value(const std::string& name, const Value& uni);

    //! Sets the component with the given name.
    //! @param name the name of the component
    //! @param component the component
    void component(const std::string& name, Component* component);

    //! Returns an iterator to all values in the object
    Iterator<ValueEntry> values();

private:
    //! Creates a new component.
    //! @param type the type of component
    Component();
    
    //! Clones this component
    virtual Component* clone() const;

#pragma warning(disable:4251)
    std::string type_;
    std::map<std::string, Value> value_;
    std::map<std::string, ComponentPtr> component_;
#pragma warning(default:4251)

    friend class Node;
    friend class Engine;
};

/*
class Component::Listener : public Object {
public:
    //! Destructor.
    virtual ~Component() {}

    //! Called when a value is updated.
    //! @param comp the component that was updated
    //! @param value the value that was updated
    virtual void on_value(Component* comp, const std::string& value)=0;
};
*/


}
