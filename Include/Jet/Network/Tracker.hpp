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

#include <Jet/Types.hpp>

using namespace Jet { using namespace Network {

class Body
{
public:
    typedef std::tr1::shared_ptr<Body> Ptr;

    inline Object:Ptr   object() const { return object_; ]
    inline void         object(Object::Ptr o) { object_ = o; }
    inline Vector       velocity() const { return velocity_; }
    inline void         velocity(const Vector& v) { velocity_ = v; }
    inline Vector       angularVelocity() const { return angularVelocity_; }
    inline void         angularVelocity(const Vector& v) { angularVelocity_ = v; }
    
protected:
    Object::Ptr         object_;
    Vector              velocity_;
    Vector              angularVelocity_;
};

}}
