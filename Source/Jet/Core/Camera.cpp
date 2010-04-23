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
    
#include <Jet/Core/Camera.hpp>
#include <cmath>

using namespace Jet;
using namespace std;

#define JET_RADIANS(x) ((x)*3.14f/180.0f)

Frustum Core::Camera::frustum() const {
    const Matrix& matrix = parent_->matrix();
    real_t width = engine_->option<real_t>("display_width");
	real_t height = engine_->option<real_t>("display_height");
	real_t shadow_distance = engine_->option<real_t>("shadow_distance");

	real_t far_distance = min(shadow_distance, far_clipping_distance());
    
    // Find the width and height of the near and far planes
    real_t ratio = width/height;
    real_t tang = tanf(JET_RADIANS(field_of_view()) * 0.5f);
    real_t nh = near_clipping_distance() * tang; // Height of the near plane
    real_t nw = nh * ratio; // Width of the near plane
    real_t fh = far_distance * tang; // Height of the far plane
    real_t fw = fh * ratio; // Width of the near plane
    
    Vector eye = matrix.origin();
    Vector at = matrix * Vector(0.0, 0.0, 1.0f);
    Vector up = matrix.up();
    
    Vector z = (eye - at).unit();
    Vector x = (up.cross(z)).unit();
    Vector y = z.cross(x);
    
    // Compute the centers of the near and far planes
    Vector nc = eye - z * near_clipping_distance();
    Vector fc = eye - z * far_distance;
    
    // Compute the 4 corners of the frustum on the near plane
    Frustum frustum;
    frustum.near_top_left = nc + y * nh - x * nw;
    frustum.near_top_right = nc + y * nh + x * nw;
    frustum.near_bottom_left = nc - y * nh - x * nw;
    frustum.near_bottom_right = nc - y * nh + x * nw;
    
    // Compute the 4 corners of the frustum on the far plane
    frustum.far_top_left = fc + y * fh - x * fw;
    frustum.far_top_right = fc + y * fh + x * fw;
    frustum.far_bottom_left = fc - y * fh - x * fw;
    frustum.far_bottom_right = fc - y * fh + x * fw;
    
    return frustum;
}