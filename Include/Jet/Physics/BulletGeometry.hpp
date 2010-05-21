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

#include <Jet/Physics/BulletTypes.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Resources/Geometry.hpp>
#include <Jet/Types/Box.hpp>

namespace Jet {
    
//! Class to hold mesh geometry for physics calculations.
//! @class Shape
//! @brief Class to hold mesh geometry for physics calculations.
class BulletGeometry : public Geometry {
public:
    //! Creates a new geometry object.
    inline BulletGeometry(CoreEngine* engine, const std::string& name) :
        engine_(engine),
        name_(name),
        state_(UNLOADED) {        
    }
    
    //! Destructor
    virtual ~BulletGeometry();
    
    //! Returns the state of the shape
    inline ResourceState state() const {
        return state_;
    }
    
    //! Returns the mesh associated with this geometric shape.
    inline Mesh* mesh() const {
        return engine_->mesh(name_);
    }
    
    //! Returns the collision shape
	inline btCollisionShape* shape() {
		return &shape_;
	}
    
    //! Sets the state of this shape.
    void state(ResourceState state);

private:
    void update_collision_shape();
    
    CoreEngine* engine_;
    std::string name_;
    ResourceState state_;
    btConvexHullShape shape_;
    Box bounding_box_;
};
    
}