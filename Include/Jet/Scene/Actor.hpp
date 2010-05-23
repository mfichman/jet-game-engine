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

namespace Jet {

//! The actor class is an implementation of a finite state machine (FSM) for a
//! node.  Actors can be useful for many things, including listening for events
//! from a node and implementing AI.
//! @class Actor
//! @brief Interface for handling node events.
class Actor : public Object {
public:
    
    //! Returns the parent node.
    virtual Node* parent() const=0;
    
    //! Returns the current state.  If the object currently has no state, then
    //! the empty state will be returned.
    virtual const std::string& state() const=0;
    
    //! Sets the current state.  This will switch the acive state to the
    //! state given by name.
    virtual void state(const std::string& name)=0;
    
    //! Adds a new state object to this actor.  A state listens for node
    //! events, and switches when the actor state changes.
    //! @param name the name of the state
    //! @param state the state object
    virtual void actor_state(const std::string& name, ActorState* state)=0;
};

//! This interface can be overriden by the client to provide new states for
//! the actor.
//! @param ActorState
//! @brief Interface for handling node events and state updates.
class ActorState : public virtual Object {
public:
    //! Called once per frame.  The value passed in is the game time elapsed
    //! since the last frame.  Note that this will not be a realtime value if
    //! the game is running in slow motion.  This function should be used only
    //! when necessary, because running an event every frame can slow down the
    //! application.
    virtual void on_update(float delta)=0;
    
    //! Called during each frame if the node is visible.
    virtual void on_render()=0;
    
    //! Called when a colllision is detected by the physics engine.
    virtual void on_collision(Node* node, const Vector& position)=0;
    
    //! Called when the node is destroyed.
    virtual void on_destroy()=0;

	//! Called when a node is fractured from a child of this node.
	//! @param node the node that fractured off of this node
	virtual void on_fracture(Node* node)=0;
	
	// Called once for each physics update.
	virtual void on_tick()=0;
    
    //! Called when the state is entered.
    virtual void on_state_enter()=0;
    
    //! Called when the state is left.
    virtual void on_state_exit()=0;
};

}