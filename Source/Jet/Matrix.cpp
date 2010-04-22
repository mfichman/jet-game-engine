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

#include <Jet/Matrix.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Vector.hpp>
#include <cstring>
#include <algorithm>

using namespace Jet;

Matrix::Matrix(const real_t data[16]) {
    std::copy(data, data+16, this->data);
}

Matrix::Matrix(real_t m00, real_t m01, real_t m02, real_t m03,
    real_t m10, real_t m11, real_t m12, real_t m13,
    real_t m20, real_t m21, real_t m22, real_t m23,
    real_t m30, real_t m31, real_t m32, real_t m33) {
    
    data[0] = m00; data[4] = m01; data[8] = m02; data[12] = m03;
    data[1] = m10; data[5] = m11; data[9] = m12; data[13] = m13;
    data[2] = m20; data[6] = m21; data[10] = m22; data[14] = m23;
    data[3] = m30; data[7] = m31; data[11] = m32; data[15] = m33;
}

Matrix::Matrix(const Vector& x, const Vector& y, const Vector& z) {
    
    // Right
    data[0] = x.x;
    data[1] = x.y;
    data[2] = x.z;
    data[3] = 0.0f;
    
    // Up
    data[4] = y.x;
    data[5] = y.y;
    data[6] = y.z;
    data[7] = 0.0f;
    
    // Forward
    data[8] = z.x;
    data[9] = z.y;
    data[10] = z.z;
    data[11] = 0.0f;

    
    data[12] = 0.0f;
    data[13] = 0.0f;
    data[14] = 0.0f;
    data[15] = 1.0f;
}

Matrix::Matrix(const Quaternion& rotation, const Vector& trans) {
    // This routine is borrowed from Ogre 3D
    real_t fTx  = 2.0f*rotation.x;
    real_t fTy  = 2.0f*rotation.y;
    real_t fTz  = 2.0f*rotation.z;
    real_t fTwx = fTx*rotation.w;
    real_t fTwy = fTy*rotation.w;
    real_t fTwz = fTz*rotation.w;
    real_t fTxx = fTx*rotation.x;
    real_t fTxy = fTy*rotation.x;
    real_t fTxz = fTz*rotation.x;
    real_t fTyy = fTy*rotation.y;
    real_t fTyz = fTz*rotation.y;
    real_t fTzz = fTz*rotation.z;

    data[0] = 1.0f-(fTyy+fTzz);
    data[4] = fTxy-fTwz;
    data[8] = fTxz+fTwy;
    data[12] = trans.x;
    
    data[1] = fTxy+fTwz;
    data[5] = 1.0f-(fTxx+fTzz);
    data[9] = fTyz-fTwx;
    data[13] = trans.y;
    
    data[2] = fTxz-fTwy;
    data[6] = fTyz+fTwx;
    data[10] = 1.0f-(fTxx+fTyy);
    data[14] = trans.z;
    
    data[3] = 0.0f;
    data[7] = 0.0f;
    data[11] = 0.0f;
    data[15] = 1.0f;
}

Matrix::Matrix(const Quaternion& rotation) {
    // This routine is borrowed from Ogre 3D
    real_t fTx  = 2.0f*rotation.x;
    real_t fTy  = 2.0f*rotation.y;
    real_t fTz  = 2.0f*rotation.z;
    real_t fTwx = fTx*rotation.w;
    real_t fTwy = fTy*rotation.w;
    real_t fTwz = fTz*rotation.w;
    real_t fTxx = fTx*rotation.x;
    real_t fTxy = fTy*rotation.x;
    real_t fTxz = fTz*rotation.x;
    real_t fTyy = fTy*rotation.y;
    real_t fTyz = fTz*rotation.y;
    real_t fTzz = fTz*rotation.z;

    data[0] = 1.0f-(fTyy+fTzz);
    data[4] = fTxy-fTwz;
    data[8] = fTxz+fTwy;
    data[12] = 0.0f;
    
    data[1] = fTxy+fTwz;
    data[5] = 1.0f-(fTxx+fTzz);
    data[9] = fTyz-fTwx;
    data[13] = 0.0f;
    
    data[2] = fTxz-fTwy;
    data[6] = fTyz+fTwx;
    data[10] = 1.0f-(fTxx+fTyy);
    data[14] = 0.0f;
    
    data[3] = 0.0f;
    data[7] = 0.0f;
    data[11] = 0.0f;
    data[15] = 1.0f;
}

Matrix::Matrix(const Vector& trans) {
    std::fill_n(data, 16, 0.0f);
    
    data[12] = trans.x;
    data[13] = trans.y;
    data[14] = trans.z;
    data[15] = 1.0f;
}

Matrix::Matrix() {
    std::fill_n(data, 16, 0.0f);
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix out;
    
    /*
    0  1  2  3
    4  5  6  7
    8  9  10 11
    12 13 14 15
    */
    
    /*
    0  4  8  12
    1  5  9  13
    2  6  10 14
    3  7  11 15
    */
    
    out.data[0] = other.data[0]*data[0] + other.data[1]*data[4] + other.data[2]*data[8] + other.data[3]*data[12];
    out.data[1] = other.data[0]*data[1] + other.data[1]*data[5] + other.data[2]*data[9] + other.data[3]*data[13];
    out.data[2] = other.data[0]*data[2] + other.data[1]*data[6] + other.data[2]*data[10] + other.data[3]*data[14];
    out.data[3] = other.data[0]*data[3] + other.data[1]*data[7] + other.data[2]*data[11] + other.data[3]*data[15];
    
    out.data[4] = other.data[4]*data[0] + other.data[5]*data[4] + other.data[6]*data[8] + other.data[7]*data[12];
    out.data[5] = other.data[4]*data[1] + other.data[5]*data[5] + other.data[6]*data[9] + other.data[7]*data[13];
    out.data[6] = other.data[4]*data[2] + other.data[5]*data[6] + other.data[6]*data[10] + other.data[7]*data[14];
    out.data[7] = other.data[4]*data[3] + other.data[5]*data[7] + other.data[6]*data[11] + other.data[7]*data[15];
    
    out.data[8] = other.data[8]*data[0] + other.data[9]*data[4] + other.data[10]*data[8] + other.data[11]*data[12];
    out.data[9] = other.data[8]*data[1] + other.data[9]*data[5] + other.data[10]*data[9] + other.data[11]*data[13];
    out.data[10] = other.data[8]*data[2] + other.data[9]*data[6] + other.data[10]*data[10] + other.data[11]*data[14];
    out.data[11] = other.data[8]*data[3] + other.data[9]*data[7] + other.data[10]*data[11] + other.data[11]*data[15];
    
    out.data[12] = other.data[12]*data[0] + other.data[13]*data[4] + other.data[14]*data[8] + other.data[15]*data[12];
    out.data[13] = other.data[12]*data[1] + other.data[13]*data[5] + other.data[14]*data[9] + other.data[15]*data[13];
    out.data[14] = other.data[12]*data[2] + other.data[13]*data[6] + other.data[14]*data[10] + other.data[15]*data[14];
    out.data[15] = other.data[12]*data[3] + other.data[13]*data[7] + other.data[14]*data[11] + other.data[15]*data[15];

    return out;
}

Vector Matrix::operator*(const Vector& v) const {
    Vector out;
    
    real_t invw = 1.0f / (data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15]);
    
    out.x = (data[0]*v.x + data[4]*v.y + data[8]*v.z + data[12])*invw;
    out.y = (data[1]*v.x + data[5]*v.y + data[9]*v.z + data[13])*invw;
    out.z = (data[2]*v.x + data[6]*v.y + data[10]*v.z + data[14])*invw;
    
    return out;
}

Frustum Matrix::operator*(const Frustum& f) const {
    Frustum out;
    
    out.near_top_left = (*this) * f.near_top_left;
    out.near_top_right = (*this) * f.near_top_right;
    out.near_bottom_left = (*this) * f.near_bottom_left;
    out.near_bottom_right = (*this) * f.near_bottom_right;
    
    out.far_top_left = (*this) * f.far_top_left;
    out.far_top_right = (*this) * f.far_top_right;
    out.far_bottom_left = (*this) * f.far_bottom_left;
    out.far_bottom_right = (*this) * f.far_bottom_right;
    
    return out;
}

Matrix Matrix::inverse() const {
    // This inversion routine is taken from Ogre3D, version 1.7.
    Matrix out;
    
    real_t m00 = data[0], m01 = data[1], m02 = data[2], m03 = data[3];
    real_t m10 = data[4], m11 = data[5], m12 = data[6], m13 = data[7];
    real_t m20 = data[8], m21 = data[9], m22 = data[10], m23 = data[11];
    real_t m30 = data[12], m31 = data[13], m32 = data[14], m33 = data[15];
    
    real_t v0 = m20 * m31 - m21 * m30;
    real_t v1 = m20 * m32 - m22 * m30;
    real_t v2 = m20 * m33 - m23 * m30;
    real_t v3 = m21 * m32 - m22 * m31;
    real_t v4 = m21 * m33 - m23 * m31;
    real_t v5 = m22 * m33 - m23 * m32;
           
    real_t t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    real_t t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    real_t t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    real_t t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
    
    real_t invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    real_t d00 = t00 * invDet;
    real_t d10 = t10 * invDet;
    real_t d20 = t20 * invDet;
    real_t d30 = t30 * invDet;

    real_t d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    real_t d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    real_t d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    real_t d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    real_t d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    real_t d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    real_t d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    real_t d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    real_t d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    real_t d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    real_t d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    real_t d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
    
    out.data[0] = d00;
    out.data[4] = d01;
    out.data[8] = d02;
    out.data[12] = d03;
    out.data[1] = d10;
    out.data[5] = d11;
    out.data[9] = d12;
    out.data[13] = d13;
    out.data[2] = d20;
    out.data[6] = d21;
    out.data[10] = d22;
    out.data[14] = d23;
    out.data[3] = d30;
    out.data[7] = d31;
    out.data[11] = d32;
    out.data[15] = d33;
    
    return out;
}