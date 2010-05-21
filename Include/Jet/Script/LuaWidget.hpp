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
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreOverlay.hpp>

namespace Jet {
    
//! This class is used to register for events regarding an overlay.
//! @class OverlayListener
//! @brief Listens for overlay events.
class LuaWidget : public OverlayListener {
public:
    
    //! Creates a new script widget for an overlay.
    //! @param self the lua object
    //! @param node the parent overlay for this widget
    //! @param name the name of the widget overlay
    inline LuaWidget(CoreEngine* engine, CoreOverlay* overlay, int ref, const std::string& name) :
        engine_(engine),
		overlay_(overlay->overlay(name)) {
            
		LuaScript* script = static_cast<LuaScript*>(engine_->script());
		lua_State* env = script->env();
		lua_getref(env, ref);
		lua_unref(env, ref);

        overlay_->listener(this);
		self_ = luabind::object(luabind::from_stack(env, -1));
        self_["overlay"] = static_cast<Overlay*>(overlay_);
    }
    
    //! Destructor
    inline virtual ~LuaWidget() {
        self_["overlay"] = luabind::nil;
    }
    
private:
    //! Called when a key is released
    inline void on_key_pressed(const std::string& key) {
        self_["on_key_pressed"](self_, key);
    }
    
    //! Called when a key is released
    inline void on_key_released(const std::string& key) {
        self_["on_key_released"](self_, key);
    }
    
    //! Called when the mouse is pressed while over the overlay.
    inline void on_mouse_pressed(int button) {
        self_["on_mouse_pressed"](self_, button);
    }
    
    //! Called when the mouse is released over the overlay.
    inline void on_mouse_released(int button) {
        self_["on_mouse_released"](self_, button);
    }
    
    //! Called when the mouse enters the region of the overlay.
    inline void on_mouse_enter() {
        self_["on_mouse_enter"](self_);
    }
    
    //! Called when the mouse exits the region of the overlay.
    inline void on_mouse_exit() {
        self_["on_mouse_exit"](self_);
    }
    
    //! Called during an update
    inline void on_update(float delta) {
        self_["on_update"](self_, delta);
    }
    
    //! Called when the overlay is destroyed.
    inline void on_destroy() {
        self_["on_destroy"](self_);
        overlay_ = 0;
    }
    
	CoreEngine* engine_;
    CoreOverlay* overlay_;
    luabind::object self_;
};

}