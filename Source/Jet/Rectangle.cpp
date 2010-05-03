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

#include <Jet/Rectangle.hpp>

using namespace Jet;

Rectangle::Rectangle() :
    min_x(0.0f),
    min_y(0.0f),
    max_x(0.0f),
    max_y(0.0f) {
    
}

//! Creates a rectangle from a position and a width
Rectangle::Rectangle(real_t x, real_t y, real_t width, real_t height) :
    min_x(x),
    min_y(y),
    max_x(x + width),
    max_y(y + height) {
    
}

//! Returns the width of the box
real_t Rectangle::width() const {
    return max_x - min_x;
}

//! Returns the height of the box
real_t Rectangle::height() const {
    return max_y - min_y;
}