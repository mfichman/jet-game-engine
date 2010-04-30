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

#include <Jet/Plane.hpp>

using namespace Jet;

Plane::Plane() :
    a(0.0f),
    b(1.0f),
    c(0.0f),
    d(0.0f) {
    
}

Plane::Plane(const Vector& normal, const Vector& point) :
    a(normal.x),
    b(normal.y),
    c(normal.z),
    d(-normal.dot(point)) {
    
}

Plane::Plane(const Vector& p1, const Vector& p2, const Vector& p3) {
    Vector normal = (p2 - p1).cross(p3 - p1);
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -normal.dot(p1);
}

Plane::Plane(real_t a, real_t b, real_t c, real_t d) :
    a(a),
    b(b),
    c(c),
    d(d) {
    
}

real_t Plane::distance(const Vector& vector) const {
    return (a*vector.x + b*vector.y + c*vector.z + d)/sqrtf(a*a + b*b + c*c);
}
