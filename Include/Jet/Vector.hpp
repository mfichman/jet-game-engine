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

JETAPI std::ostream& operator<<(std::ostream& out, const Jet::Vector& vector);
JETAPI std::istream& operator>>(std::istream& in, Jet::Vector& vector);

namespace Jet {

//! Represents a 3-component vector.
//! @class Vector
//! @brief 3-component vector.
class JETAPI Vector {
public:
    //! Creates a new vector with the given components.
    //! @param x x component
    //! @param y y component
    //! @param z z component
    Vector(real_t x, real_t y, real_t z);

    //! Creates a new vector.
    Vector();

    //! Returns the length of the vector.
    real_t length() const;

    //! Returns the squared length of the vector.
    real_t length2() const;

    //! Distance; distance to another vector
    //! @param other another vector
    real_t distance(const Vector& other) const;

    //! Distance squared; distance to another vector
    //! @param other another vector
    real_t distance2(const Vector& other) const;

    //! Dot product; returns a new vector.
    //! @param other another vector
    real_t dot(const Vector& other) const;

    //! Cross product; returns a new vector.
    //! @param other another vector
    Vector cross(const Vector& other) const;

    //! Adds the other vector; returns a new vector.
    //! @param other another vector
    Vector operator+(const Vector& other) const;

    //! Adds another vector; returns a new vector;
    //! @param other another vector
    Vector operator-(const Vector& other) const;

    //! Component-wise multiplication
    //! @param other another vector
    Vector operator*(const Vector& other) const;

    //! Component-wise division
    //! @param other another vector
    Vector operator/(const Vector& other) const;

    //! Negates this vector
    Vector operator-() const;

    //! Scales the vector.
    //! @param s the scaling factor
    Vector operator*(real_t s) const;

    //! Returns a unit vector.
    Vector unit() const;
    
    //! Returns an arbitrary vector perpindicular to this one.
    Vector orthogonal() const;
    
    //! Returns the type of avector.
    ValueType type() const {
        return VT_VECTOR; 
    }
    
    //! Returns a pointer to the vector data.
    operator const real_t*() const {
        return &x;
    }
    
    //! Returns a pointer to the vector data.
    operator real_t*() {
        return &x;
    }

    //! Stream operator.
    friend std::ostream& ::operator<<(std::ostream& out, const Vector& vector);

    //! Stream operator.
    friend std::istream& ::operator>>(std::istream& in, Vector& vector);

    //! Comparison operator.
    bool operator<(const Vector& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
    
    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector& other) const {
        return !this->operator==(other);
    }

    static Vector ZERO;
    static Vector UNIT_X;
    static Vector UNIT_Y;
    static Vector UNIT_Z;

    real_t x;
    real_t y;
    real_t z;
};

}
