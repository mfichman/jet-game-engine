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

#include <Jet/Types/Quaternion.hpp>
#include <Jet/Types/Vector.hpp>
#include <cmath>

using namespace Jet;
using namespace std;

Quaternion Quaternion::IDENTITY;

Quaternion::Quaternion(float w, float x, float y, float z) : 
    w(w),
    x(x),
    y(y),
    z(z) {
}

Quaternion::Quaternion(const Vector& axis, float angle) {
    float halfAngle = 0.5f*angle;
    float sin = sinf(halfAngle);
    w = cosf(halfAngle);
    x = sin*axis.x;
    y = sin*axis.y;
    z = sin*axis.z;
}

Quaternion::Quaternion(const Vector& xaxis, const Vector& yaxis, const Vector& zaxis) {
    float kRot[3][3];
    kRot[0][0] = xaxis.x;
    kRot[1][0] = xaxis.y;
    kRot[2][0] = xaxis.z;
    
    kRot[0][1] = yaxis.x;
    kRot[1][1] = yaxis.y;
    kRot[2][1] = yaxis.z;
    
    kRot[0][2] = zaxis.x;
    kRot[1][2] = zaxis.y;
    kRot[2][2] = zaxis.z;

    float fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
    float fRoot;

    if (fTrace > 0.0) {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = sqrtf(fTrace + 1.0f);  // 2w
        w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        x = (kRot[2][1]-kRot[1][2])*fRoot;
        y = (kRot[0][2]-kRot[2][0])*fRoot;
        z = (kRot[1][0]-kRot[0][1])*fRoot;
    } else {
        // |w| <= 1/2
        static size_t s_iNext[3] = { 1, 2, 0 };
        size_t i = 0;
        if ( kRot[1][1] > kRot[0][0] )
            i = 1;
        if ( kRot[2][2] > kRot[i][i] )
            i = 2;
        size_t j = s_iNext[i];
        size_t k = s_iNext[j];

        fRoot = sqrtf(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
        float* apkQuat[3] = { &x, &y, &z };
        *apkQuat[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        w = (kRot[k][j]-kRot[j][k])*fRoot;
        *apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
        *apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
    }
}

Quaternion::Quaternion() :
    w(1),
    x(0),
    y(0),
    z(0) {
}

  
float Quaternion::length() const {
    return sqrtf(length2());
}

float Quaternion::length2() const {
    return w*w + x*x + y*y + z*z;
}

Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(w+other.w, x+other.x, y+other.y, z+other.z);
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(w-other.w, x-other.x, y-other.y, z-other.z);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w*other.w - x*other.x - y*other.y - z*other.z,
        w*other.x + x*other.w + y*other.z - z*other.y,
        w*other.y + y*other.w + z*other.x - x*other.z,
        w*other.z + z*other.w + x*other.y - y*other.x);
}

Quaternion Quaternion::inverse() const {
    float norm = length2();
    if (norm > 0) {
        return Quaternion(w/norm, -x/norm, -y/norm, -z/norm);
    } else {
        return Quaternion(0, 0, 0, 0);
    }
}

float Quaternion::dot(const Quaternion& other) const {
    return w*other.w + x*other.x + y*other.y + z*other.z;
}

Quaternion Quaternion::operator*(float s) const {
    return Quaternion(w*s, x*s, y*s, z*s);
}

Quaternion Quaternion::operator-() const {
    return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::unit() const {
    float norm = length();
    return Quaternion(w/norm, x/norm, y/norm, z/norm);
}

Quaternion Quaternion::slerp(const Quaternion& other, float alpha) const {
    float cos = this->dot(other);
    Quaternion rkt;

    if (cos < 0) {
        cos = -cos;
        rkt = -other;
    } else {
        rkt = other;
    }

    if (abs(cos) < 1 - 1e-03) {
        float sin = sqrtf(1 - cos*cos);
        float angle = atan2f(sin, cos);
        float inv_sin = 1/sin;
        float coeff0 = sinf((1-alpha) * angle) * inv_sin;
        float coeff1 = sinf(alpha * angle) * inv_sin;
        return (*this)*coeff0 + rkt*coeff1;
    } else {
        rkt = (*this)*(1-alpha) + rkt*alpha;
        return rkt.unit();
    }
}

bool Quaternion::operator==(const Quaternion other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Quaternion::operator!=(const Quaternion& other) const {
    return !operator==(other);
}   

ostream& operator<<(ostream& out, const Quaternion& quaternion) {
    return out << quaternion.w << " " << quaternion.x << " " << quaternion.y << " " << quaternion.z;
}

istream& operator>>(istream& in, Quaternion& quaternion) {
    return in >> quaternion.w >> quaternion.x >> quaternion.y >> quaternion.z;
}
