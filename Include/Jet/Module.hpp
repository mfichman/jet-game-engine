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
    
    //! Called when a network game is found
    virtual void on_player_update(int number, const std::string& name)=0;
    
    //! Called when a network game is found
    virtual void on_game_found(const std::string& game)=0;
    
    //! Called when a game is lost.
    virtual void on_game_lost(const std::string& game)=0;
    
    //! Called when a key is pressed.
    //! @param key the key
    //! @param point the x and y values of the mouse position in the normalized
    //! range (-1.0, 1.0)
    virtual void on_key_pressed(const std::string& key, const Point& point)=0;
    
    //! Called when the key is released.
    //! @param key the key
    //! @param point the x and y values of the mouse position in the normalized
    //! range (-1.0, 1.0)
    virtual void on_key_released(const std::string& key, const Point& point)=0;
       
     //! Called when the button is pressed.
    //! @param button the button
    //! @param point the x and y values of the mouse position in the normalized
    //! range (-1.0, 1.0)
    virtual void on_mouse_pressed(uint32_t button, const Point& point)=0;
    
    //! Called when the button is released.
    //! @param button the button
    //! @param point the x and y values of the mouse position in the normalized
    //! range (-1.0, 1.0)
    virtual void on_mouse_released(uint32_t button, const Point& point)=0;
    
    //! Called when the mouse movies.
    //! @param point the x and y values of the mouse position in the normalized
    //! range (-1.0, 1.0)
    virtual void on_mouse_motion(const Point& point)=0;
    
    //! Called when the button is released.
    //! @param button the button
    //! @param axes the x, y and z-axis positions of the joystick in the
    //! range (-1.0, 1.0)
    virtual void on_joystick(uint32_t button, const Vector& axes)=0;
    
    //! Called when the module is created.
    virtual void on_init()=0;
    
    //! Called when the module is updated (during the physics update)
    virtual void on_update(float delta)=0;
    
    //! Called when the module is rendered.
    virtual void on_render()=0;
    
    //! Called when the module is destroyed.
    virtual void on_destroy()=0;
    
    //! Called once per frame.
    virtual void on_tick()=0;
};

}