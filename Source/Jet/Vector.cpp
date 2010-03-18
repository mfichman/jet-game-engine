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

#include <Jet/Vector.hpp>
#include <cmath>

using namespace Jet;
using namespace std;

Vector::Vector(real_t x, real_t y, real_t z) : 
    x(x),
    y(y),
    z(z) {
}

Vector::Vector() :
    x(0),
    y(0),
    z(0) {
}

real_t 
Vector::length() const {
    return sqrtf(length2());
}

real_t 
Vector::length2() const {
    return x*x + y*y + z*z;
}

real_t 
Vector::distance(const Vector& other) const {
    return sqrtf(distance2(other));
}

real_t 
Vector::distance2(const Vector& other) const {
    Vector sub = other - (*this);
    return sub.length2();
}

real_t 
Vector::dot(const Vector& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vector 
Vector::cross(const Vector& other) const {
    return Vector(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x);
}

Vector 
Vector::operator+(const Vector& other) const {
    return Vector(x+other.x, y+other.y, z+other.z);
}

Vector 
Vector::operator-(const Vector& other) const {
    return Vector(x-other.x, y-other.y, z-other.z);
}

Vector 
Vector::operator*(const Vector& other) const {
    return Vector(x*other.x, y*other.y, z*other.z);
}

Vector 
Vector::operator/(const Vector& other) const {
    return Vector(x/other.x, y/other.y, z/other.z);
}

Vector 
Vector::operator-() const {
    return Vector(-x, -y, -z);
}

Vector 
Vector::operator*(real_t s) const {
    return Vector(s*x, s*y, s*z);
}

Vector 
Vector::unit() const {
    real_t norm = length();
    return Vector(x/norm, y/norm, z/norm);
}

ostream& operator<<(ostream& out, const Vector& vector) {
    return out << vector.x << " " << vector.y << " " << vector.z;
}

istream& operator>>(istream& in, Vector& vector) {
    return in >> vector.x >> vector.y >> vector.z;
}
