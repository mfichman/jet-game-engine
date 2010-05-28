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

#include <Jet/Core/CoreTypes.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Scene/Actor.hpp>
#include <Jet/Utility.hpp>
#include <map>

namespace Jet {

//! The actor class is an implementation of a finite state machine (FSM) for a
//! node.  Actors can be useful for many things, including listening for events
//! from a node and implementing AI.
//! @class CoreActor
//! @brief Interface for handling node events.
class CoreActor : public Actor {
public:
    inline CoreActor(CoreEngine* engine, CoreNode* node) :
        engine_(engine),
        parent_(node),
        current_state_(0) {
        
    }
    
    //! Returns the parent node.
    inline CoreNode* parent() const {
        return parent_;
    }
    
    
    //! Returns the current state.  If the object currently has no state, then
    //! the empty state will be returned.
    inline const std::string& state() const {
        return state_;
    }

	//! Returns a hash of the current state name.
	inline uint32_t state_hash() const {
		return Jet::hash(state_);
	}
    
    //! Sets the current state.  This will switch the acive state to the
    //! state given by name.
    void state(const std::string& name);

	//! Sets the current state by hash.
	void state_hash(uint32_t hash);
    
    //! Adds a new state object to this actor.  A state listens for node
    //! events, and switches when the actor state changes.
    //! @param name the name of the state
    //! @param state the state object
    inline void actor_state(const std::string& name, ActorState* state) {
        actor_state_[name] = state;
    }
    
private:    
    
    CoreEngine* engine_;
    CoreNode* parent_;
    ActorState* current_state_;
    std::string state_;
    std::map<std::string, ActorStatePtr> actor_state_;
    
    friend class CoreNode;
};


}