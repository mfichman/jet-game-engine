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

#include <Jet/Quaternion.hpp>
#include <Jet/Vector.hpp>
#include <cmath>

using namespace Jet;
using namespace std;

Quaternion Quaternion::IDENTITY;

Quaternion::Quaternion(real_t w, real_t x, real_t y, real_t z) : 
    w(w),
    x(x),
    y(y),
    z(z) {
}

Quaternion::Quaternion(const Vector& axis, real_t angle) {
    real_t halfAngle = 0.5f*angle;
    real_t sin = sinf(halfAngle);
    w = cosf(halfAngle);
    x = sin*axis.x;
    y = sin*axis.y;
    z = sin*axis.z;
}

Quaternion::Quaternion(const Vector& x, const Vector& y, const Vector& z) {
    real_t rot[3][3];
    rot[0][0] = x.x;
    rot[1][0] = x.y;
    rot[2][0] = x.z;
    rot[0][1] = y.x;
    rot[1][1] = y.y;
    rot[2][1] = y.z;
    rot[0][2] = z.x;
    rot[1][2] = z.y;
    rot[2][2] = z.z;

    real_t trace = rot[0][0]+rot[1][1]+rot[2][2];
    if (trace > 0) {
        real_t root = sqrtf(trace + 1);
        this->w = 0.5f*root;
        this->x = (rot[2][1]-rot[1][2])*root;
        this->y = (rot[0][2]-rot[2][0])*root;
        this->z = (rot[1][0]-rot[0][1])*root;
    } else {
        size_t next[3] = { 1, 2, 0 };
        size_t i = 0;
        if (rot[1][1] > rot[0][0]) i = 1;
        if (rot[2][2] > rot[i][i]) i = 2;
        size_t j = next[i];
        size_t k = next[j];

        real_t root = sqrtf(rot[i][i]-rot[j][j]-rot[k][k]+1);
        real_t* quat[3] = { &this->x, &this->y, &this->z };
        *quat[i] = 0.5f*root;
        root = 0.5f/root;
        this->w = (rot[k][j]-rot[j][k])*root;
        *quat[j] = (rot[j][i]+rot[i][j])*root;
        *quat[k] = (rot[k][i]+rot[i][k])*root;
    }
}

Quaternion::Quaternion() :
    w(1),
    x(0),
    y(0),
    z(0) {
}

  
real_t 
Quaternion::length() const {
    return sqrtf(length2());
}

real_t 
Quaternion::length2() const {
    return w*w + x*x + y*y + z*z;
}

Quaternion 
Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(w+other.w, x+other.x, y+other.y, z+other.z);
}

Quaternion 
Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(w-other.w, x-other.x, y-other.y, z-other.z);
}

Quaternion 
Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w*other.w - x*other.x - y*other.y - z*other.z,
        w*other.x + x*other.w + y*other.z - z*other.y,
        w*other.y + y*other.w + z*other.x - x*other.z,
        w*other.z + z*other.w + x*other.y - y*other.x);
}

Quaternion 
Quaternion::inverse() const {
    real_t norm = length2();
    if (norm > 0) {
        return Quaternion(w/norm, -x/norm, -y/norm, -z/norm);
    } else {
        return Quaternion(0, 0, 0, 0);
    }
}

real_t 
Quaternion::dot(const Quaternion& other) const {
    return w*other.w + x*other.x + y*other.y + z*other.z;
}

Quaternion 
Quaternion::operator*(real_t s) const {
    return Quaternion(w*s, x*s, y*s, z*s);
}

Quaternion 
Quaternion::operator-() const {
    return Quaternion(-w, -x, -y, -z);
}

Quaternion
Quaternion::unit() const {
    real_t norm = length();
    return Quaternion(w/norm, x/norm, y/norm, z/norm);
}

Quaternion 
Quaternion::slerp(const Quaternion& other, real_t alpha) const {
    real_t cos = this->dot(other);
    Quaternion rkt;

    if (cos < 0) {
        cos = -cos;
        rkt = -other;
    } else {
        rkt = other;
    }

    if (abs(cos) < 1 - 1e-03) {
        real_t sin = sqrtf(1 - cos*cos);
        real_t angle = atan2f(sin, cos);
        real_t inv_sin = 1/sin;
        real_t coeff0 = sinf((1-alpha) * angle) * inv_sin;
        real_t coeff1 = sinf(alpha * angle) * inv_sin;
        return (*this)*coeff0 + rkt*coeff1;
    } else {
        rkt = (*this)*(1-alpha) + rkt*alpha;
        return rkt.unit();
    }
}

ostream& operator<<(ostream& out, const Quaternion& quaternion) {
    return out << quaternion.w << " " << quaternion.x << " " << quaternion.y << " " << quaternion.z;
}

istream& operator>>(istream& in, Quaternion& quaternion) {
    return in >> quaternion.w >> quaternion.x >> quaternion.y >> quaternion.z;
}
