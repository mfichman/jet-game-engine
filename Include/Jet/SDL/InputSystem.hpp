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

#include <Jet/SDL/Types.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Input.hpp>

namespace Jet { namespace SDL {

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class InputSystem
//! @brief Rigid body physics engine
class InputSystem : public Input, public EngineListener {
public:
    //! Creates a new input system.
    InputSystem(Core::Engine* engine);
    
    //! Destructor.
    ~InputSystem();

private:
    void on_tick() {}
    void on_init();
    void on_update();
    void on_render() {}
    
	void on_key_pressed(const std::string& key);
    void on_key_released(const std::string& key);
    void on_mouse_pressed(int button, int x, int y);
    void on_mouse_released(int button, int x, int y);
	void on_mouse_moved(int x, int y);
    void on_joystick(int button, int x, int y, int z);
    
    Point normalized_mouse(int x, int y);
    
    Core::Engine* engine_;
};

}}