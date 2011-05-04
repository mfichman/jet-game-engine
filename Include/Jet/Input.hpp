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
#include <Jet/Object.hpp>

namespace Jet {

//! Interface for input engine subsystems
//! @class Input
//! @brief Input subsystem
class Input : public virtual Object {
public:
    
	//! Returns the current input state for the local player.
	virtual const InputState& input_state() const=0;

    //! Offers an input state for the given player.  This allows the
	//! network system to add an input state for processing.
	virtual void input_state(const InputState& state)=0;

	//! Returns true if the key is down for the given player UUID
	virtual bool key_down(uint32_t uuid, const std::string& key)=0;

	//! Returns true if the mouse button is down for the given player UUID
	virtual bool mouse_down(uint32_t uuid, int button)=0;

	//! Returns the current mouse position
	virtual const Point& mouse_position(uint32_t uuid)=0;
};

}
