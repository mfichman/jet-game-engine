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
#include <Jet/Object.hpp>
#include <Jet/Interface.hpp>

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;

class Camera : public Interface {
public:
    typedef shared_ptr<Camera> Ptr;
    typedef RangedOrdinal<float, 0, 1> Tightness;

    inline Object::Ptr      target() const { return target_; }
    inline void             target(Object::Ptr t) { target_ = t; }
    inline Vector           up() const { return up_; }
    inline void             up(const Vector& u) { up_ = u; }
    inline Object::Ptr      parent() const { return parent_; }
    inline void             parent(Object::Ptr p) { parent_ = p; }
    inline Tightness        tightness() { return tightness_; }
    inline void             tightness(Tightness t) { tightness_ = t; }

protected:
    Vector                  up_;
    Object::Ptr             target_;
    Object::Ptr             parent_;
    Tightness               tightness_;
};

}}
