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
    
#include <Jet/Box.hpp>
#include <Jet/Frustum.hpp>
#include <cfloat>
    
using namespace Jet;
    
Box::Box() :
    min_x(FLT_MAX),
    max_x(-FLT_MAX),
    min_y(FLT_MAX),
    max_y(-FLT_MAX),
    min_z(FLT_MAX),
    max_z(-FLT_MAX) {
        
}

Box::Box(const Frustum& frustum) :
    min_x(FLT_MAX),
    max_x(-FLT_MAX),
    min_y(FLT_MAX),
    max_y(-FLT_MAX),
    min_z(FLT_MAX),
    max_z(-FLT_MAX) {
        
    point(frustum.near_top_left);
    point(frustum.near_top_right);
    point(frustum.near_bottom_left);
    point(frustum.near_bottom_right);
    
    point(frustum.far_top_left);
    point(frustum.far_top_right);
    point(frustum.far_bottom_left);
    point(frustum.far_bottom_right);
    
}

float Box::volume() const {
    return width() * height() * depth();
}

float Box::width() const {
    return max_x - min_x;
}

float Box::height() const {
    return max_y - min_y;
}

float Box::depth() const {
    return max_z - min_z;
}

Vector Box::half_extents() const {
    return Vector(width()/2.0f, height()/2.0f, depth()/2.0f);
}

Vector Box::origin() const {
    return Vector((max_x + min_x)/2.0f, (max_y + min_y)/2.0f, (max_z + min_z)/2.0f);
}

void Box::point(const Vector& point) {
    // Check to see if the added point expands the bounding box.
    if (point.x < min_x) {
        min_x = point.x;
    } 
	if (point.x > max_x) {
        max_x = point.x;
    }
    
    if (point.y < min_y) {
        min_y = point.y;
    }
	if (point.y > max_y) {
        max_y = point.y;   
    }
    
    if (point.z < min_z) {
        min_z = point.z;
    }
	if (point.z > max_z) {
        max_z = point.z;
    }
}
