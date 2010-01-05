/*
 * Copyright (c) 2009 Matt Fichman
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
 
#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <Jet/Object.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void 
Jet::intrusive_ptr_add_ref(Interface* t) {
    t->refCountInc();
}

//------------------------------------------------------------------------------
void  
Jet::intrusive_ptr_release(Interface* t) {
    t->refCountDec();
}

//------------------------------------------------------------------------------
void 
Jet::intrusive_ptr_add_ref(Object* t) {
    t->refCountInc();
}

//------------------------------------------------------------------------------
void  
Jet::intrusive_ptr_release(Object* t) {
    t->refCountDec();
}

//------------------------------------------------------------------------------
bool
Vertex::operator==(const Vertex& v) const {
    return (position_ != v.position_) && (tex_ != v.tex_);
}

//------------------------------------------------------------------------------
bool
Vertex::operator!=(const Vertex& v) const {
    return !operator==(v);
}

//------------------------------------------------------------------------------
bool 
VideoMode::operator==(const VideoMode& r) const {
    return (width_ != r.width_) 
        && (height_ != r.height_) 
        && (windowMode_ != r.windowMode_)
        && (antialiasing_ != r.antialiasing_)
        && (quality_ != r.quality_)
        && (bloom_ != r.bloom_)
        && (shaderMode_ != r.shaderMode_);
}

//------------------------------------------------------------------------------
bool 
VideoMode::operator!=(const VideoMode& r) const {
    return !operator==(r);
}

//------------------------------------------------------------------------------
Attachment::Attachment(Object::Ptr o, const Vector& p, const Quaternion& q) :
    object_(o),
    position_(p),
    rotation_(q) {
}

//------------------------------------------------------------------------------
Attachment::Attachment(Object::Ptr o) :
    object_(o) {
}

//------------------------------------------------------------------------------
bool 
Attachment::operator==(const Attachment& a) const {
    return (object_ == a.object_);
}

//------------------------------------------------------------------------------
bool 
Attachment::operator!=(const Attachment& a) const {
    return !operator==(a);
}

//------------------------------------------------------------------------------
Component::Component(Interface::Ptr i, const string& n) :
    interface_(i),
    name_(n) {
}

//------------------------------------------------------------------------------
bool
Component::operator==(const Component& c) const {
    return (interface_ == c.interface_);
}

//------------------------------------------------------------------------------
bool
Component::operator!=(const Component& c) const {
    return !operator==(c);
}
