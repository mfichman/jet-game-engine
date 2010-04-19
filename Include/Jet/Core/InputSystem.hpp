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
#include <Jet/Core/Engine.hpp>

namespace Jet { namespace Core {

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class InputSystem
//! @brief Rigid body physics engine
class InputSystem : public EngineListener {
public:
    //! Creates a new input system.
    InputSystem(Engine* engine);
    
    //! Destructor.
    ~InputSystem();

private:
    void on_init();
    void on_update();
    void on_post_update() {}
    void on_render() {}
    
    static void on_keyboard(unsigned char key, int x, int y);
    static void on_keyboard_up(unsigned char key, int x, int y);
    static void on_special(int key, int x, int y);
    static void on_special_up(int key, int x, int y);
    static void on_mouse(int button, int state, int x, int y);
    static void on_joystick(unsigned int button, int x, int y, int z);
    
    Engine* engine_;
    static InputSystem* system_;
};

}}