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

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Node.hpp>
#include <luabind/luabind.hpp>
#include <vector>

namespace Jet { namespace Core {

//! Base class for objects that are connected to a scene node, and control it.
//! This class is used to attach logic to a node.
//! @class ScriptController
//! @brief Attaches script logic to a node
class ScriptController : public Jet::NodeListener, public luabind::wrap_base {
public:
    //! Creates a new script controller with a new node.
	inline ScriptController(Jet::Node* node, const std::string& name) {
        node_ = static_cast<Node*>(node->node(name));
		node_->listener(this);
        std::cout << "Initialized in C++" << std::endl;
    }
    
    inline virtual ~ScriptController() {
		std::cout << "Destroyed" << std::endl;
		if (node_) {
			node_->destroy();
		}
    }
    
    //! Returns the node attached to this script controller.
    inline Jet::Node* node() const {
        return node_;
    }

private:
    inline void on_update() {
		call<void>("on_update");
	}

    inline void on_render() {
		call<void>("on_render");
	}

    inline void on_collision() {
		call<void>("on_collision");
	}

    inline void on_destroy() {
		//call<void>("on_destroy");
		node_ = 0;
	}
    
    Node* node_;
    
};

}}