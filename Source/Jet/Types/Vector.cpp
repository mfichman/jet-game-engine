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

#include <Jet/Types/Vector.hpp>
#include <cmath>

using namespace Jet;
using namespace std;

Vector::Vector(float x, float y, float z) : 
    x(x),
    y(y),
    z(z) {
}

Vector::Vector() :
    x(0),
    y(0),
    z(0) {
}

float Vector::length() const {
    return sqrtf(length2());
}

float Vector::length2() const {
    return x*x + y*y + z*z;
}

float Vector::distance(const Vector& other) const {
    return sqrtf(distance2(other));
}

float Vector::distance2(const Vector& other) const {
    Vector sub = other - (*this);
    return sub.length2();
}

float Vector::dot(const Vector& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vector Vector::cross(const Vector& other) const {
    return Vector(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x);
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x+other.x, y+other.y, z+other.z);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(x-other.x, y-other.y, z-other.z);
}

Vector Vector::operator*(const Vector& other) const {
    return Vector(x*other.x, y*other.y, z*other.z);
}

Vector Vector::operator/(const Vector& other) const {
    return Vector(x/other.x, y/other.y, z/other.z);
}

Vector Vector::operator-() const {
    return Vector(-x, -y, -z);
}

Vector Vector::operator/(float s) const {
	return Vector(x/s, y/s, z/s);
}

Vector Vector::operator*(float s) const {
    return Vector(s*x, s*y, s*z);
}

Vector& Vector::operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}


Vector Vector::project(const Vector& other) const {
    Vector u = unit();
    return u * other.dot(u);
}

Vector Vector::unit() const {
    float norm = length();
    return Vector(x/norm, y/norm, z/norm);
}

Vector Vector::orthogonal() const {
    
    Vector ortho = cross(Vector(1.0f, 0.0f, 0.0f));
    if (ortho.length2() < 1e-12) {
        ortho = cross(Vector(0.0f, 1.0f, 0.0f));
    }
    
    return ortho.unit();    
}

ostream& operator<<(ostream& out, const Vector& vector) {
    return out << vector.x << " " << vector.y << " " << vector.z;
}

istream& operator>>(istream& in, Vector& vector) {
    return in >> vector.x >> vector.y >> vector.z;
}
