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

namespace Jet {

//! Represents an infinite plane.
//! @class Plane
//! @brief An infinite plane
class Plane {
public:
    //! Creates a plane with the normal vector (0, 1, 0) at the point (0, 0, 0)
    Plane();
    
    //! Creates a plane using the given normal vector and point.
    //! @param normal the normal vector
    //! @param point the point
    Plane(const Vector& normal, const Vector& point);
    
    //! Creates a plane using three points.
    Plane(const Vector& p1, const Vector& p2, const Vector& p3);
    
    //! Creates a plane from the plane equation coefficients.  They are used
    //! in the following equation to form the plane:
    //! ax + by + cz + d = 0
    Plane(float a, float b, float c, float d);
    
    //! Returns the distance from the plane to the given vector.  If this value
    //! is positive, then it is on the same side of the plane as the normal
    //! vector.  Otherwise, it is on the opposite side.
    //! @param vector the vector to test
    float distance(const Vector& vector) const;

    /*
     * The plane coefficients are used for the following equation:
     * ax + by + cz + d = 0
     */
    float a;
    float b;
    float c;
    float d;
};

}
