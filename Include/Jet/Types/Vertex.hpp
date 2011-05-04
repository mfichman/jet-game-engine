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
#include <Jet/Types/Texcoord.hpp>
#include <iostream>

namespace Jet {

//! Represents a vertex.
//! @class Vertex
//! @brief Vertex class.
class Vertex {
public:

    //! Comparison operator.
    inline bool operator<(const Vertex& other) const {
        if (position != other.position) return position < other.position;
        if (normal != other.normal) return normal < other.normal;
        return texcoord < other.texcoord;
    }
    
    inline bool operator==(const Vertex& other) const {
        return position == other.position && texcoord == other.texcoord && normal == other.normal;
    }

	inline bool operator!=(const Vertex& other) const {
		return !this->operator==(other);
	}

	inline Vertex operator+(const Vertex& other) const {
		Vertex out;
		out.position = position + other.position;
		out.normal = normal + other.normal;
		out.tangent = tangent + other.tangent;
		out.texcoord = texcoord + other.texcoord;
		return out;
	}

	inline Vertex operator*(float scale) const {
		Vertex out;
		out.position = position * scale;
		out.normal = normal * scale;
		out.tangent = tangent * scale;
		out.texcoord = texcoord * scale;
		return out;
	}
    
    Vector position;
    Vector normal;
    Vector tangent;
    Texcoord texcoord;
};

}
