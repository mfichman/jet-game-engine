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
#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Quaternion.hpp>
#include <Jet/Types/Frustum.hpp>

namespace Jet {

//! This class represents a 4x4 matrix for transforming homogeneous
//! coordinates.
//! @class Matrix
//! @brief 4x4 matrix representation
class Matrix {
public:
	
	//! Creates a perspective projection matrix
	static Matrix frustum(float l, float r, float b, float t, float n, float f);

	//! Creates an orthographic projection matrix
	static Matrix ortho(float l, float r, float b, float t, float n, float f);

	//! Creates a perspective projection matrix using field-of-view
	static Matrix perspective(float fovy, float aspect, float near, float far);
    
    //! Creates a new matrix using the given array of floats in row-major
    //! order (same as OpenGL).
    //! @param data the array
    Matrix(const float data[16]);
    
    //! Creates a new matrix from a list of floats.
    //! @param floats
    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33);
    
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
    
    //! Creates a matrix from 3 orthonormal vectors
    Matrix(const Vector& x, const Vector& y, const Vector& z);
    
    //! Creates a new empty matrix, with all entries initialized to zero.
    Matrix();
    
    //! Returns the inverse of this matrix.
    Matrix inverse() const;
    
    //! Multiplies two matrices.
    Matrix operator*(const Matrix& other) const;
    
    //! Transforms a vector.
    Vector operator*(const Vector& other) const;
    
    //! Transforms the view frustum.
    Frustum operator*(const Frustum& other) const;
    
    //! Rotates a vector using the rotation part of this matrix.
    Vector rotate(const Vector& other) const;
    
    //! Returns a pointer to the matrix data.
    inline operator const float*() const {
        return data;
    }
    
    //! Returns a pointer to the matrix data.
    inline operator float*() {
        return data;
    }
    
    //! Returns an element of the data array
    inline float& operator[](const size_t& i) {
        return data[i];
    }

    
    //! Returns the forward vector (i.e., along the positive z-axis)
    inline Vector forward() const {
        return Vector(data[8], data[9], data[10]);
    }
    
    //! Returns the right vector (i.e., along the positive x-axis)
    inline Vector right() const {
        return Vector(data[0], data[1], data[2]);
    }
    
    //! Returns the left vector (i.e., along the positive y-axis)
    inline Vector up() const {
        return Vector(data[4], data[5], data[6]);
    }
    
    //! Returns the origin of the matrix translation
    inline Vector origin() const {
        return Vector(data[12], data[13], data[14]);
    }
    
    //! Returns the rotation part of the matrix
    inline Quaternion rotation() const {
        return Quaternion(right(), up(), forward());
    }
    
    /* Matrix layout:
     * 0  4  8  12
     * 1  5  9  13
     * 2  6  10 14
     * 3  7  11 15
    */
    float data[16];
};
    
}