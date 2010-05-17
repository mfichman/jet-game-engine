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
#include <Jet/Core/Overlay.hpp>
#include <luabind/luabind.hpp>

namespace Jet { namespace Core {
    
//! This class is used to register for events regarding an overlay.
//! @class OverlayListener
//! @brief Listens for overlay events.
class ScriptWidget : public OverlayListener {
public:
    
    //! Creates a new script widget for an overlay.
    //! @param self the lua object
    //! @param node the parent overlay for this widget
    //! @param name the name of the widget overlay
    inline ScriptWidget(const luabind::object& self, Jet::Overlay* overlay, const std::string& name) :
        overlay_(static_cast<Overlay*>(overlay->overlay(name))),
        self_(self) {
            
        overlay_->listener(this);
        self_["overlay"] = static_cast<Jet::Overlay*>(overlay_);
    }
    
    //! Destructor
    inline virtual ~ScriptWidget() {
        self_["overlay"] = luabind::nil;
    }
    
private:
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
    
    Overlay* overlay_;
    luabind::object self_;
};

}}