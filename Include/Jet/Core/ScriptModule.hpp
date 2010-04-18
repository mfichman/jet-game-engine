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
#include <Jet/Module.hpp>
#include <luabind/luabind.hpp>
#include <vector>

namespace Jet { namespace Core {
    
//! A game ScriptModule (can be used to implement levels or zones)
//! @class ScriptModule
//! @brief A game ScriptModule or level
class ScriptModule : public Module {
public:
  
    //! Creates a new script module
    inline ScriptModule(const luabind::object& self) :
        self_(self) {
            
    }
    
private:
    //! Called when a key is pressed.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    void on_key_pressed(const std::string& key, real_t x, real_t y) {
        self_["on_key_pressed"](self_, key, x, y);
    }
    
    //! Called when the key is released.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    void on_key_released(const std::string& key, real_t x, real_t y) {
        self_["on_key_released"](self_, key, x, y);    
    }
    
    //! Called when the button is pressed.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    void on_button_pressed(uint32_t button, real_t x, real_t y) {
        self_["on_button_pressed"](self_, button, x, y);
    }
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    void on_button_released(uint32_t button, real_t x, real_t y) {
        self_["on_button_released"](self_, button, x, y);
    }
    
    //! Called when the mouse movies.
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    void on_mouse_motion(real_t x, real_t y) {
        self_["on_mouse_motion"](self_, x, y);
    }
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x-axis position of the joystick in the range (-1.0, 1.0)
    //! @param y the y-axis position of the joystick in the range (-1.0, 1.0)
    //! @param z the x-axis position of the joystick in the range (-1.0, 1.0)
    void on_joystick(uint32_t button, real_t x, real_t y, real_t z) {
        self_["on_joystick"](self_, button, x, y, z);
    }
    
    //! Called when the ScriptModule is created.
    void on_init() {
        self_["on_init"](self_);
    }
    
    //! Called when the ScriptModule is updated (during the physics update)
    void on_update() {
        self_["on_update"](self_);
    }
    
    //! Called when the ScriptModule is rendered.
    void on_render() {
        self_["on_render"](self_);
    }
    
    //! Called when the ScriptModule is destroyed.
    void on_destroy() {
        self_["on_destroy"](self_);
    }
    
    luabind::object self_;
};

}}