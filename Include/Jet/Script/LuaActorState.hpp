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

#include <Jet/Script/LuaTypes.hpp>
#include <Jet/Script/LuaScript.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Scene/Actor.hpp>
#include <iostream>

namespace Jet {

//! Base class for objects that are connected to a scene node, and control it.
//! This class is used to attach logic to a node.
//! @class ActorState
//! @brief Attaches script logic to a node
class LuaActorState : public ActorState {
public:
    //! Creates a new script controller with a new node.
	inline LuaActorState(CoreEngine* engine, int ref) :
        engine_(engine) {
            
        LuaScript* script = static_cast<LuaScript*>(engine_->script());
		lua_State* env = script->env();
		lua_getref(env, ref);
		lua_unref(env, ref);
        
        self_ = luabind::object(luabind::from_stack(env, -1));  
    
		lua_pop(env, 1);
		assert(!lua_gettop(env));
	}

    inline void on_update(float delta) {
        self_["on_update"](self_, delta);
	}

	inline void on_collision(Node* node, const Vector& position) {
		self_["on_collision"](self_, node, position);
	}

    inline void on_destroy() {
		self_["on_destroy"](self_);
	}

	inline void on_fracture(Node* node) {
		self_["on_fracture"](self_, node);
	}
    
    inline void on_tick() {
        self_["on_tick"](self_);
    }
    
    inline void on_state_enter() {
        self_["on_state_enter"](self_);
    }
    
    inline void on_state_exit() {
        self_["on_state_exit"](self_);
    }

private:
    CoreEngine* engine_;
    luabind::object self_;
};

}
