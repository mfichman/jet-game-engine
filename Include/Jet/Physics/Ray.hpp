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

#include <Jet/Types.hpp>
#include <Jet/Physics/Object.hpp>

namespace Jet { namespace Physics {
using namespace std;
using namespace std::tr1;
using namespace boost;

class Ray : public Interface {
public:
    typedef intrusive_ptr<Ray> Ptr;
    
    inline float        length() const { return length_; }
    inline void         length(float l) { length_ = l; }
    inline Vector       direction() const { return direction_; }
    inline void         direction(const Vector& v) { direction_ = v; }
    inline Object::Ptr  object() const { return object_; }
    inline void         object(Object::Ptr p) { object_ = p; }
    
protected:
    float           length_;
    Vector          direction_;
    Object::Ptr		object_;
};

}}
