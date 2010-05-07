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

std::ostream& operator<<(std::ostream& out, const Jet::Color& color);
std::istream& operator>>(std::istream& in, Jet::Color& color);

namespace Jet {

//! Represents a 4-component RGBA color value.
//! @class Color
//! @brief RGBA color value.
class Color {
public:
    //! Creates a new color.
    //! @param red red component
    //! @param blue blue component
    //! @param green green component
    //! @param alpha alpha component
    Color(float red, float blue, float green, float alpha);
    
    //! Creates a new color, initialized to black.
    Color();
    
    //! Returns a pointer to the internal float data of this color
    inline operator const float*() const {
        return &red;
    }
    
    //! Returns a pointer to the internal float data of this color
    inline operator float*() {
        return &red;
    }
    
    //! Stream operator.
    friend std::ostream& ::operator<<(std::ostream& out, const Color& color);

    //! Stream operator.
    friend std::istream& ::operator>>(std::istream& in, Color& color);

    float red;
    float blue;
    float green;
    float alpha;
};

}

