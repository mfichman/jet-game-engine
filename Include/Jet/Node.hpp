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
#include <Jet/Component.hpp>
#include <Jet/Controller.hpp>
#include <string>
#include <map>
#include <list>

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
    class Listener;

    //! Destructor.
    virtual ~Node() {}
    
    //! Returns the parent scene node
    inline Node* parent() const { 
        return parent_;
    }

    //! Returns the node with the given name.
    //! @param name the name of the node attached to this node
    inline Node* node(const std::string& name) const {
        std::map<std::string, NodePtr>::const_iterator i = node_.find(name);
        if (i == node_.end()) {
            Node* self = const_cast<Node*>(this);
            NodePtr node(new Node(engine_, self));
            self->node_.insert(make_pair(name, node));
            return node.get();
        } else {
            return i->second.get();
        }
    }

    //! Returns the object with the given name that is attached to this scene
    //! node.  The object may be of any type.
    //! @param name the name of the component
    inline Component* component(const std::string& name) const {
        std::map<std::string, ComponentPtr>::const_iterator i = component_.find(name);
        return (i == component_.end()) ? 0 : i->second.get();
    }

    //! Returns an iterator to all nodes connected to this scene node.
    Iterator<NodePtr> nodes();

    //! Returns an iterator to all nodes connected to this scene node.
    Iterator<const NodePtr> nodes() const;
    
    //! Returns an iterator to all components connected to this scene node that
    //! have the given type
    Iterator<const ComponentPtr> components() const;

    //! Returns an iterator to all components connected to this scene node that
    //! have the given type
    //! @param type the type of components to retrieve
    Iterator<ComponentPtr> components(const std::string& type);

    //! Returns an iterator to all components connected to this scene node that
    //! have the given type
    //! @param type the type of components to retrieve
    Iterator<const ComponentPtr> components(const std::string& type) const;

    //! Returns the current position of this scene node relative to its parent.
    inline const Vector& position() const {
        return position_;
    }

    //! Returns the current rotation of this scene node relative to its parent.
    inline const Quaternion& rotation() const {
        return rotation_;
    }

    //! Attaches a new node using the given blueprint node.
    //! @param name the name of the new node
    //! @param blueprint the name of the blueprint node
    Node* node(const std::string& name, const std::string& blueprint);

    //! Attaches a component using the given blueprint component.
    //! @param name the name of the new component
    //! @param blueprint the name of the blueprint
    Component* component(const std::string& name, const std::string& blueprint);

    //! Attaches a new component to this node.
    //! @param name the name of the new component
    //! @param component the new component
    void component(const std::string& name, Component* component);

    //! Attaches a controller using the given blueprint component.  Note that 
    //! each controller may have only one parent, so adding a controller that
    //! already has a parent is illegal.
    //! @param type the type of controller to load, e.g., Spaceship.lua,
    //! Spaceship.py, or Spaceship.cpp.
    void controller(const std::string& type);
    
    //! Adds a new controller to this node.
    //! @param controller the new controller
    void controller(Controller* controller);
    
    //! Sets the current position of this scene node relative to its parent.
    //! @param position the new position
    inline void position(const Vector& position) {
        position_ = position;
    }

    //! Sets the current rotation of this scene node relative to its parent.
    //! @param v the new rotation
    inline void rotation(const Quaternion& rotation) {
        rotation_ = rotation;
    }

    //! Destroys this node and removes it from the parent.  The memory will not 
    //! be freed until all references to this node are broken.
    void destroy();
    
    //! Called to send an event to all controllers attached to this node.
    //! @param name the name of the event
    //! @param params list of parameters for the event
    void event(const std::string& name, const Params& params);
    
private:
    //! Private onstructor
    Node(Engine* engine, Node* parent);
    
    //! Private constructor.
    Node(Engine* engine);
    
    //! Clones this scene node
    virtual Node* clone() const;

    //! Removes the child from the parent
    //! @param child the child node
    void remove_child(Node* node);
 
    //! Called to send an update event to all controllers attached to this
    //! node.  This is called by the engine class once per physics update.
    void update();
    
    //! Called to send a render event to all controllers attached to this
    //! node.  Called by the engine class when the node is sent to the
    //! renderer.
    void render();
    
    Engine* const engine_;
    Node* parent_;
#pragma warning(disable:4251)
    std::string name_;
    std::map<std::string, NodePtr> node_;
    std::map<std::string, ComponentPtr> component_;
    std::list<ControllerPtr> controller_;
    std::multimap<std::string, ComponentPtr> component_type_;
#pragma warning(default:4251)
    Vector position_;
    Quaternion rotation_;

    friend class Engine;
};

/*
class Node::Listener : pubic Object {
public:
    //! Destructor.
    virtual ~Node() {}

    //! Called when a component is added to the node.
    //! @param node the affected node
    //! @param name the name of the added component
    virtual void on_component(Node* node, const std::string& name)=0;

    //! Called when the position is updated.
    //! @param node the affected node
    virtual void on_position(Node* node)=0;

    //! Called when the rotation is updated.
    //! @param node the affected node
    virtual void on_rotation(Node* node)=0;

    //! Called when the watched node is destroyed.
    //! @param node the affected node
    virtual void on_destroy(Node* node)=0;
};
*/

}
