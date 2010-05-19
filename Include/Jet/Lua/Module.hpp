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

#include <Jet/Lua/Types.hpp>
#include <Jet/Lua/ScriptSystem.hpp>
#include <Jet/Module.hpp>

namespace Jet { namespace Lua {
    
//! A game Module (can be used to implement levels or zones)
//! @class Module
//! @brief A game Module or level
class Module : public Jet::Module {
public:
  
    //! Creates a new script module
    inline Module(Core::Engine* engine, int ref) :
        engine_(engine) {
           
		ScriptSystem* script = static_cast<ScriptSystem*>(engine_->script());
		lua_State* env = script->env();
		lua_getref(env, ref);
		lua_unref(env, ref);

		self_ = luabind::object(luabind::from_stack(env, -1));
    }
    
private:
    //! Called when a key is pressed.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    inline void on_key_pressed(const std::string& key, const Point& point) {
        self_["on_key_pressed"](self_, key, point);
    }
    
    //! Called when the key is released.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    inline void on_key_released(const std::string& key, const Point& point) {
        self_["on_key_released"](self_, key, point);    
    }
    
    //! Called when the button is pressed.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    inline void on_mouse_pressed(uint32_t button, const Point& point) {
        self_["on_mouse_pressed"](self_, button, point);
    }
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    inline void on_mouse_released(uint32_t button, const Point& point) {
        self_["on_mouse_released"](self_, button, point);
    }
    
    //! Called when the mouse movies.
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    inline void on_mouse_motion(const Point& point) {
        self_["on_mouse_motion"](self_, point);
    }
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x-axis position of the joystick in the range (-1.0, 1.0)
    //! @param y the y-axis position of the joystick in the range (-1.0, 1.0)
    //! @param z the x-axis position of the joystick in the range (-1.0, 1.0)
    inline void on_joystick(uint32_t button, const Vector& axes) {
        self_["on_joystick"](self_, button, axes);
    }
    
    //! Called when the Module is created.
    inline void on_init() {
        self_["on_init"](self_);
    }
    
    //! Called when the Module is updated (during the physics update)
    inline void on_update(float delta) {
        self_["on_update"](self_, delta);
    }
    
    //! Called when the Module is rendered.
    inline void on_render() {
        self_["on_render"](self_);
    }
    
    //! Called when the Module is destroyed.
    inline void on_destroy() {
        self_["on_destroy"](self_);
    }
    
    //! Called once per frame.
    inline void on_tick() {
        self_["on_tick"](self_);
    }
    
	Core::Engine* engine_;
    luabind::object self_;
};

}}