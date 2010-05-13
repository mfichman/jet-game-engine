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
#include <Jet/Vector.hpp>

namespace Jet {

//! Holds information about an action/event.
//! @class Texcoord
//! @brief 3-component texcoord.
class Action {
public:
    //! Creates a zero-size bounding box.
    Box();
    
    //! Creates a bounding box from a frustum.
    //! @param frustum the frustum to get the bounding box for.
    Box(const Frustum& frustum);

    //! Returns the width of the box
    float width() const;
    
    //! Returns the height of the box
    float height() const;
    
    //! Returns the depth of the box
    float depth() const;
    
    //! Returns the volume of the box.
    float volume() const;
    
    //! Returns the half-extents of the box
    Vector half_extents() const;
    
    //! Returns the origin of the box
    Vector origin() const;
    
    //! Adds a point to the box, expanding it if necessary.
    void point(const Vector& point);

    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
};

}
