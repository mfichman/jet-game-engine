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

namespace Jet { namespace Impl { namespace ODE {
using namespace std;
using namespace std::tr1;

//------------------------------------------------------------------------------
inline const dReal*
fromQuaternion(const Quaternion& q) {
    static dReal out[4];
    out[0] = q.w();
    out[1] = q.x();
    out[2] = q.y();
    out[3] = q.z();
    return out;
}

//------------------------------------------------------------------------------
inline const dReal* 
fromVector(const Vector& v) {
    static dReal out[3];
    out[0] = v.x();
    out[1] = v.y();
    out[2] = v.z();
    return out;
}

//------------------------------------------------------------------------------
inline const Quaternion&
toQuaternion(const dReal* q) {
    static Quaternion out;
    out[0] = q[1];
    out[1] = q[2];
    out[2] = q[3];
    out[3] = q[0];
    return out;
}

//------------------------------------------------------------------------------
inline const Vector&
toVector(const dReal* v) {
    static Vector out;
    out[0] = v[0];
    out[1] = v[2];
    out[2] = v[3];
    return out;
}

}}}
