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
#include <Jet/Object.hpp>
#include <Jet/Types/Color.hpp>
#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Frustum.hpp>

namespace Jet {
    
//! The scene is rendered from the viewpoint of the camera.
//! @class Camera
//! @brief Controls the viewpoint.
class Camera : public Object {
public:    
    //! Returns the parent node.
    virtual Node* parent() const=0;
    
    //! Returns the field of view
    virtual float field_of_view() const=0;
    
    //! Returns the clipping distance
    virtual float far_clipping_distance() const=0;
    
    //! Returns the clipping distance
    virtual float near_clipping_distance() const=0;
    
    //! Returns true if this is the active camera.
    virtual bool active() const=0;
    
    //! Returns the view frustum for this camera
    virtual Frustum view_frustum() const=0;
    
    //! Returns the shadow frustum for this camera.  This is the same as
    //! the view frustum except for the far clipping plane, which is set to
    //! the minimum of the shadow distance and the far clipping distance.
    virtual Frustum frustum(float near, float far) const=0;
    
    //! Sets the field of view
    virtual void field_of_view(float fov)=0;
    
    //! Sets the clipping distance
    virtual void far_clipping_distance(float distance)=0;

    //! Sets the clipping distance
    virtual void near_clipping_distance(float distance)=0;

    // Makes the camera active.
    virtual void active(bool active)=0;
};

}