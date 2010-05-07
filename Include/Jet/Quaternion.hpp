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
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Jet::Quaternion& quaternion);
std::istream& operator>>(std::istream& in, Jet::Quaternion& quaternion);

namespace Jet {

//! Represents a rotation in 3D space.
//! @class Quaternion
//! @brief Quaternion rotation class
class Quaternion {
public:
    //! Creates a new quaternion.
    //! @param w scalar component
    //! @param x x component
    //! @param y y component
    //! @param z z component
    Quaternion(float w, float x, float y, float z);

    //! Creates a new quaternion from an angle and axis.
    //! @param axis the quaternion axis
    //! @param angle the rotation angle about the axis
    Quaternion(const Vector& axis, float angle);

    //! Creates a new quaternion from an orthonormal basis.
    //! @param x the x axis
    //! @param y the y axis
    //! @param z the z axis
    Quaternion(const Vector& x, const Vector& y, const Vector& z);
    
    //! Creates a new quaternion.
    Quaternion(const Matrix& matrix);

    //! Creates a new identity quaternion
    Quaternion();

    //! Returns the length of the quaternion.
    float length() const;

    //! Returns the squared length of the quaternion.
    float length2() const;

    //! Dot product.
    //! @param other another quaternion
    float dot(const Quaternion& other) const;

    //! Scalar multiplication.
    //! @param s the scalar
    Quaternion operator*(float s) const;

    //! Adds the other vector; returns a new quaternion.
    //! @param other another quaternion
    Quaternion operator+(const Quaternion& other) const;

    //! Adds another quaternion; returns a new quaternion.
    //! @param other another quaternion
    Quaternion operator-(const Quaternion& other) const;

    //! Multiplies this quaternion with another
    //! @param other another quaternion
    Quaternion operator*(const Quaternion& other) const;

    //! Negates this quaternion
    Quaternion operator-() const;

    //! Interpolates between this quaternion and another.
    //! @param other another quaternion
    Quaternion slerp(const Quaternion& other, float alpha) const;

    //! Returns the inverse of this quaternion
    Quaternion inverse() const;

    //! Returns the normalized copy of this quaternion
    Quaternion unit() const;
    
    //! Returns true if the quaternions are equal
    bool operator==(const Quaternion other) const;
    
    //! Returns true if the quaternions are not equal
    bool operator!=(const Quaternion& other) const;

    //! Stream operator
    friend std::ostream& ::operator<<(std::ostream& out, const Quaternion& quaternion);

    //! Stream operator
    friend std::istream& ::operator>>(std::istream& in, Quaternion& quaternion);

    static Quaternion IDENTITY;

    float w;
    float x;
    float y;
    float z;
};

}
