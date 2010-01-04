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
#pragma once

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;
class Root;

class JETAPI Camera : public Object {
public:  
    friend class Root; 
    typedef intrusive_ptr<Camera> Ptr;
    typedef RangedOrdinal<float, 0, 1> Tightness;

    // Attributes
    inline const Vector&        target() const { return target_; }
    void                        target(const Vector& t);
    inline const Vector&        up() const { return up_; }
    void                        up(const Vector& u);
    inline Tightness            tightness() const { return tightness_; }
    void                        tightness(Tightness t);
    inline void                 operator()(Object::Functor& f) { f(this); }

protected:
    Camera() {}

    Vector target_;
    Vector up_;
    Tightness tightness_;
};

}
