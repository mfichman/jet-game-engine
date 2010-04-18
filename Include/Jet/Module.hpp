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

namespace Jet {
    
//! A game module (can be used to implement levels or zones)
//! @class Module
//! @brief A game module or level
class Module : public Object {
public:    
    
    //! Called when a key is pressed.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    virtual void on_key_pressed(const std::string& key, real_t x, real_t y)=0;
    
    //! Called when the key is released.
    //! @param key the key
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    virtual void on_key_released(const std::string& key, real_t x, real_t y)=0;
       
     //! Called when the button is pressed.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    virtual void on_button_pressed(uint32_t button, real_t x, real_t y)=0;
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    virtual void on_button_released(uint32_t button, real_t x, real_t y)=0;
    
    //! Called when the mouse movies.
    //! @param x the x value of the mouse position in the range (-1.0, 1.0)
    //! @param y the y value of the mouse position in the range (-1.0, 1.0)
    virtual void on_mouse_motion(real_t x, real_t y)=0;
    
    //! Called when the button is released.
    //! @param button the button
    //! @param x the x-axis position of the joystick in the range (-1.0, 1.0)
    //! @param y the y-axis position of the joystick in the range (-1.0, 1.0)
    //! @param z the x-axis position of the joystick in the range (-1.0, 1.0)
    virtual void on_joystick(uint32_t button, real_t x, real_t y, real_t z)=0;
    
    //! Called when the module is created.
    virtual void on_init()=0;
    
    //! Called when the module is updated (during the physics update)
    virtual void on_update()=0;
    
    //! Called when the module is rendered.
    virtual void on_render()=0;
    
    //! Called when the module is destroyed.
    virtual void on_destroy()=0;
};

}