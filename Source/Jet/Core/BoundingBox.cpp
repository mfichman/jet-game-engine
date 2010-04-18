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
    
#include <Jet/BoundingBox.hpp>
    
using namespace Jet;
    
BoundingBox::BoundingBox() :
    min_x(0.0f),
    max_x(0.0f),
    min_y(0.0f),
    max_y(0.0f),
    min_z(0.0f),
    max_z(0.0f) {
        
}

real_t BoundingBox::width() const {
    return max_x - min_x;
}

real_t BoundingBox::height() const {
    return max_y - min_y;
}

real_t BoundingBox::depth() const {
    return max_z - min_z;
}

Vector BoundingBox::half_extents() const {
    return Vector(width()/2.0f, height()/2.0f, depth()/2.0f);
}

Vector BoundingBox::origin() const {
    return Vector((max_x + min_x)/2.0f, (max_y + min_y)/2.0f, (max_z + min_z)/2.0f);
}

void BoundingBox::point(const Vector& point) {
    if (point.x < min_x) {
        min_x = point.x;
    } else if (point.x > max_x) {
        max_x = point.x;
    }
    
    if (point.y < min_y) {
        min_y = point.y;
    } else if (point.y > max_y) {
        max_y = point.y;   
    }
    
    if (point.z < min_z) {
        min_z = point.z;
    } else if (point.z > max_z) {
        max_z = point.z;
    }
}
