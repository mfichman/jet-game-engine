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

#include <Jet/Object.hpp>
#include <algorithm>

using namespace Jet;
using namespace std;

//------------------------------------------------------------------------------
void
Object::position(const Vector& v) {
    if (position_ != v) {
        position_ = v;
        publisher_.notify(&Observer::onPosition);
    }
}

//------------------------------------------------------------------------------
void
Object::rotation(const Quaternion& q) {
    if (rotation_ != q) {
        rotation_ = q;
        publisher_.notify(&Observer::onRotation);
    }
}

//------------------------------------------------------------------------------
void
Object::networkSync(NetworkSync s) {
    if (networkSync_ != s) {
        networkSync_ = s;
        publisher_.notify(&Observer::onNetworkSync);
    }
}
