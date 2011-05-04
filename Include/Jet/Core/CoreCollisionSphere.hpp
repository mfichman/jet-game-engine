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

#include <Jet/Core/CoreTypes.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Scene/CollisionSphere.hpp>

namespace Jet {
    
//! Represents a simple sphere that can be used for collisions and triggers.
//! @class CoreCollisionSphere
//! @brief Spherical collision shape
class CoreCollisionSphere : public CollisionSphere {
public:
    //! Creates a new collision sphere
    inline CoreCollisionSphere(CoreEngine* engine, CoreNode* node) :
        engine_(engine),
        parent_(node) {
        
    }
    
    //! Returns the parent node.
    inline CoreNode* parent() const {
        return parent_;
    }
    
    //! Returns the radius of the sphere
    inline float radius() const {
        return radius_;
    }
    
    //! Sets the radius of the sphere
    inline void radius(float radius) {
        radius_ = radius;
    }
    
private:
    CoreEngine* engine_;
    CoreNode* parent_;
    float radius_;
};

}