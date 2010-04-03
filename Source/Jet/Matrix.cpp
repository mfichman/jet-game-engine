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
#include <cstring>

using namespace Jet;

Matrix::Matrix(const real_t data[16]) {
    memcpy((void*)data, (void*)this->data, sizeof(this->data));
}

Matrix::Matrix() {
    memset((void*)this->data, 0, sizeof(this->data));
}

Matrix Matrix::operator*(const Matrix& other) const {
    Matrix out;
    
    /*
    0  1  2  3
    4  5  6  7
    8  9  10 11
    12 13 14 15
    */
    
    out.data[0] = data[0]*other.data[0] + data[1]*other.data[4] + data[2]*other.data[8] + data[3]*other.data[12];
    out.data[1] = data[0]*other.data[1] + data[1]*other.data[5] + data[2]*other.data[9] + data[3]*other.data[13];
    out.data[2] = data[0]*other.data[2] + data[1]*other.data[6] + data[2]*other.data[10] + data[3]*other.data[14];
    out.data[3] = data[0]*other.data[3] + data[1]*other.data[7] + data[2]*other.data[11] + data[3]*other.data[15];
    
    out.data[4] = data[4]*other.data[0] + data[5]*other.data[4] + data[6]*other.data[8] + data[7]*other.data[12];
    out.data[5] = data[4]*other.data[1] + data[5]*other.data[5] + data[6]*other.data[9] + data[7]*other.data[13];
    out.data[6] = data[4]*other.data[2] + data[5]*other.data[6] + data[6]*other.data[10] + data[7]*other.data[14];
    out.data[7] = data[4]*other.data[3] + data[5]*other.data[7] + data[6]*other.data[11] + data[7]*other.data[15];
    
    out.data[8] = data[8]*other.data[0] + data[9]*other.data[4] + data[10]*other.data[8] + data[11]*other.data[12];
    out.data[9] = data[8]*other.data[1] + data[9]*other.data[5] + data[10]*other.data[9] + data[11]*other.data[13];
    out.data[10] = data[8]*other.data[2] + data[9]*other.data[6] + data[10]*other.data[10] + data[11]*other.data[14];
    out.data[11] = data[8]*other.data[3] + data[9]*other.data[7] + data[10]*other.data[11] + data[11]*other.data[15];
    
    out.data[12] = data[12]*other.data[0] + data[13]*other.data[4] + data[14]*other.data[8] + data[15]*other.data[12];
    out.data[13] = data[12]*other.data[1] + data[13]*other.data[5] + data[14]*other.data[9] + data[15]*other.data[13];
    out.data[14] = data[12]*other.data[2] + data[13]*other.data[6] + data[14]*other.data[10] + data[15]*other.data[14];
    out.data[15] = data[12]*other.data[3] + data[13]*other.data[7] + data[14]*other.data[11] + data[15]*other.data[15];

    return out;
}

Matrix Matrix::inverse() const {
    // This inversion routine is taken from Ogre3D version 1.7.
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
    out.data[1] = d01;
    out.data[2] = d02;
    out.data[3] = d03;
    out.data[4] = d10;
    out.data[5] = d11;
    out.data[6] = d12;
    out.data[7] = d13;
    out.data[8] = d20;
    out.data[9] = d21;
    out.data[10] = d22;
    out.data[11] = d23;
    out.data[12] = d30;
    out.data[13] = d31;
    out.data[14] = d32;
    out.data[15] = d33;
    
    return out;
}