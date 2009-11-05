/*
 * Copyright (c) 2008 Matt Fichman
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

#include <Types.hpp>
#include <Interface.hpp>
#include <Anchor.hpp>
#include <iostream>

namespace Jet {
using namespace std;
using namespace std::tr1;

class Object : public InterfaceImpl<Object> {
public:
    typedef shared_ptr<Object> Ptr;
    
    inline Vector			position() const { return anchor_->position_; }
	inline void				position(const Vector& v) { modState(modified); anchor_->position_ = v; }
    inline Quaternion		rotation() const { return anchor_->rotation_; }
    inline void				rotation(const Quaternion& q) { modState(modified); anchor_->rotation_ = q; }
	inline Anchor::ConstPtr	anchor() const { return anchor_; }
    inline FrameID			frame() const { return frame_; }
    inline void				frame(FrameID f) { modState(unmodified); frame_ = f; } 
    
	static Object::Ptr		make() { return Object::Ptr(new Object()); }

protected:  
	Object() : anchor_(new Anchor()) {}

	Anchor::Ptr			anchor_;
    FrameID             frame_;
};

}
