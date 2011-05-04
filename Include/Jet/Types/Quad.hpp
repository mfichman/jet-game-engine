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
#include <iostream>

namespace Jet {

//! Represents a quad.
//! @class Quad
//! @brief Quadclass.
class Quad {
public:
    
    //! Creates a new quad with the given normal, up vector, width, and height.
    //! @param normal the direction the quad faces.
    //! @param up the direction the quad is oriented along the y-axis
    //! @param width the width of the quad
    //! @param height the height of the quad
    Quad(const Vector& normal, const Vector& up, float width, float height);

	//! Creates a default quad.
	inline Quad() :
		width(0.0f),
		height(0.0f) {
	}

    //! Comparison operator.
    inline bool operator<(const Quad& other) const {
        if (up != other.up) return up < other.up;
        if (normal != other.normal) return normal < other.normal;
        if (width != other.width) return width < other.width;
        return height < other.height;
    }
    
    inline bool operator==(const Quad& other) const {
        return up == other.up && normal == other.normal && width == other.width && height == other.height;
    }

	inline bool operator!=(const Quad& other) const {
		return !this->operator==(other);
	}
    
    Vector normal;
    Vector up;
    float width;
    float height;
};

}
