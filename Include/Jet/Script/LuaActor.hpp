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

namespace Jet {

//! Base class for objects that are connected to a scene node, and control it.
//! This class is used to attach logic to a node.
//! @class Actor
//! @brief Attaches script logic to a node
class LuaActor : public NodeListener {
public:
    //! Creates a new script controller with a new node.
	inline  LuaActor(CoreEngine* engine, CoreNode* node, int ref, const std::string& name) :
		engine_(engine),
		node_(node->node(name)) {
        
        LuaScript* script = static_cast<LuaScript*>(engine_->script());
		lua_State* env = script->env();
		lua_getref(env, ref);
		lua_unref(env, ref);

		node_->listener(this);
		self_ = luabind::object(luabind::from_stack(env, -1));
        self_["node"] = static_cast<Node*>(node_);
    }
    
    //! Destructor
    inline virtual ~ LuaActor() {
        self_["node"] = luabind::nil;
    }

private:
    inline void on_update(float delta) {
		//self_["on_update"](self_, delta);
	}

    inline void on_render() {
		self_["on_render"](self_);
	}

	inline void on_collision(Node* node) {
		self_["on_collision"](self_, node);
	}

    inline void on_destroy() {
		self_["on_destroy"](self_);
		node_ = 0;
	}

	inline void on_fracture(Node* node) {
		self_["on_fracture"](self_, node);
	}
    
    inline void on_tick() {
        self_["on_tick"](self_);
    }
    
    inline void on_signal(const Signal& signal) {
        self_["on_" + signal.name](self_, signal.first, signal.second);
    }
    
	CoreEngine* engine_;
    CoreNode* node_;
    luabind::object self_;
    
};

}
