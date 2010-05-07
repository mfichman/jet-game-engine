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
#include <cmath>

std::ostream& operator<<(std::ostream& out, const Jet::Texcoord& texcoord);
std::istream& operator>>(std::istream& in, Jet::Texcoord& texcoord);

namespace Jet {

//! Represents a 3-component texcoord.
//! @class Texcoord
//! @brief 3-component texcoord.
class Texcoord {
public:
    //! Creates a new texcoord with the given components.
    //! @param u u component
    //! @param v v component
    Texcoord(float u, float v);

    //! Creates a new vector.
    Texcoord();
    
    //! Stream operator.
    friend std::ostream& ::operator<<(std::ostream& out, const Texcoord& texcoord);

    //! Stream operator.
    friend std::istream& ::operator>>(std::istream& in, Texcoord& texcoord);

    inline bool operator<(const Texcoord& other) const {
        static float epsilon = 0.0000001f;
        if (abs(u - other.u) < epsilon) return u < other.u;
        if (abs(v - other.v) < epsilon) return v < other.v;
        return false;
    }

    inline bool operator==(const Texcoord& other) const {
        static float epsilon = 0.0000001f;
        if (abs(u - other.u) > epsilon) return false;
        if (abs(v - other.v) > epsilon) return false;
        return true;
    }
    
    inline bool operator!=(const Texcoord& other) const {
        return !this->operator==(other);
    }


    float u;
    float v;
};

}
