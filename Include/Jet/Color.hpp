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

JETAPI std::ostream& operator<<(std::ostream& out, const Jet::Color& color);
JETAPI std::istream& operator>>(std::istream& in, Jet::Color& color);

namespace Jet {

//! Represents a 4-component RGBA color value.
//! @class Color
//! @brief RGBA color value.
class JETAPI Color {
public:
    //! Creates a new color, initialized to black.
    Color();

    //! Creates a new color.
    //! @param red red component
    //! @param blue blue component
    //! @param green green component
    //! @param alpha alpha component
    Color(real_t red, real_t blue, real_t green, real_t alpha);
    
    //! Returns the type of avector.
    ValueType type() const {
        return VT_COLOR; 
    }
    
    //! Returns a pointer to the internal float data of this color
    operator const real_t*() const {
        return &red;
    }
    
    //! Returns a pointer to the internal float data of this color
    operator real_t*() {
        return &red;
    }
    
    //! Stream operator.
    friend std::ostream& ::operator<<(std::ostream& out, const Color& color);

    //! Stream operator.
    friend std::istream& ::operator>>(std::istream& in, Color& color);

    real_t red;
    real_t blue;
    real_t green;
    real_t alpha;
};

}

