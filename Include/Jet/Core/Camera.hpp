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

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Camera.hpp>

namespace Jet { namespace Core {
    
//! The scene is rendered from the viewpoint of the camera.
//! @class Camera
//! @brief Controls the viewpoint.
class Camera : public Jet::Camera {
public:
    
    //! Returns the parent node.
    inline Node* parent() const {
        return parent_;
    }
    
    //! Returns true if this is the active camera.
    inline bool active() const {
        return this == engine_->camera();
    }
    
    //! Returns the forward vector
    inline real_t field_of_view() const {
        return field_of_view_;
    }
    
    //! Returns the clipping distance
    inline real_t far_clipping_distance() const {
        return far_clipping_distance_;
    }
    
    //! Returns the clipping distance
    inline real_t near_clipping_distance() const {
        return near_clipping_distance_;
    }
    
    //! Returns the viewing frustum
    inline Frustum view_frustum() const {
        return compute_frustum(far_clipping_distance());
    }
    
    //! Returns the shadow frustum
    inline Frustum shadow_frustum() const {
        real_t shadow_distance = engine_->option<real_t>("shadow_distance");
        real_t far_distance = min(shadow_distance, far_clipping_distance());
        return compute_frustum(far_distance);
    }
    
    //! Sets the field of view
    inline void field_of_view(real_t fov) {
        field_of_view_ = fov;
    }
    
    //! Sets this camera as the active camera
    inline void active(bool active) {
        engine_->camera(this);
    }
    
    //! Sets the clipping distance
    inline void far_clipping_distance(real_t distance) {
        far_clipping_distance_ = distance;
    }
    
    //! Sets the clipping distance
    inline void near_clipping_distance(real_t distance) {
        near_clipping_distance_ = distance;
    }

private:
    Camera(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent),
        field_of_view_(45.0f),
        near_clipping_distance_(0.1f),
        far_clipping_distance_(1000.0f) {
            
    }
    
    Frustum compute_frustum(real_t far_distance) const;
    
    Engine* engine_;
    Node* parent_;
    real_t field_of_view_;
    real_t near_clipping_distance_;
    real_t far_clipping_distance_;
    
    friend class Node;
};

}}