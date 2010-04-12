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
#pragma once

#include <Jet/Types.hpp>

namespace Jet {

//! This class represents a 4x4 matrix for transforming homogeneous
//! coordinates.
//! @class Matrix
//! @brief 4x4 matrix representation
class JETAPI Matrix {
public:
    
    //! Creates a new matrix using the given array of floats in row-major
    //! order (same as OpenGL).
    //! @param data the array
    Matrix(const real_t data[16]);
    
    //! Creates a new matrix from a list of floats.
    //! @param floats
    Matrix(real_t m00, real_t m01, real_t m02, real_t m03,
           real_t m10, real_t m11, real_t m12, real_t m13,
           real_t m20, real_t m21, real_t m22, real_t m23,
           real_t m30, real_t m31, real_t m32, real_t m33);
    
    //! Creates a new combined rotation and translation matrix from
    //! a quaternion and a vector.
    //! @param rotation the rotation of the matrix
    //! @param trans the translation of the matrix
    Matrix(const Quaternion& rotation, const Vector& trans);
    
    //! Creates a rotation matrix from a quaternion.
    //! @param rotation the rotation of the matrix
    Matrix(const Quaternion& rotation);
    
    //! Creates a translation matrix from a vector.
    //! @param trans the translation of the matrix
    Matrix(const Vector& trans);
    
    //! Creates a new empty matrix, with all entries initialized to zero.
    Matrix();
    
    //! Returns the inverse of this matrix.
    Matrix inverse() const;
    
    //! Returns a pointer to the matrix data.
    inline operator const real_t*() const {
        return data;
    }
    
    //! Returns a pointer to the matrix data.
    inline operator real_t*() {
        return data;
    }
    
    //! Returns an element of the data array
    inline real_t& operator[](const size_t& i) {
        return data[i];
    }
    
    //! Multiplies two matrices.
    Matrix operator*(const Matrix& other) const;
    
    real_t data[16];
};
    
}