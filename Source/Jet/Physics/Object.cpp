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
 
#include <Jet/Object.hpp>

using namespace Jet;
    
void
Object::frame(FrameID f) {
    if (frame_ != f) {
        frame_ = f;
    }
}

void
Object::listener(Listener::Ptr l) {
    listener_ = l;
}

void
Object::position(const Vector& v) {
   if (position_ != v) {
        position_ = v;
        if (listener_) {
            listener_->onPosition(Ptr(this));
        }
   }
}

void
Object::rotation(const Quaternion& q) {
    if (rotation_ != q) {
        rotation_ = q;
        if (listener_) {
            listener_->onRotation(Ptr(this));
        }
    }
}

void 
Object::body(Body::Ptr p) {
    if (body_ != p) {
        if (body_) body_->object(NULL);
        body_ = p;
        if (body_) body_->object(this);
        if (listener_) {
            listener_->onBody(Ptr(this));
        }
    }
}

void 
Object::object(Object::Ptr p) {
    if (object_ != p) {
        object_ = p;
        if (listener_) {
            listener_->onObject(Ptr(this));
        }
    }
}
