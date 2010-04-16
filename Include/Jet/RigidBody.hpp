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
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <string>

namespace Jet {

//! Physical simulation.  This class is used for simple rigid body dynamics.
//! Access to the underlying physics engine, if needed, can be obtained by
//! compiling your program with the headers for the physics engine plugin you
//! are using.
//! @class RigidBody
//! @brief Physical simulation with collisions.
class RigidBody : public Object {
public:
    //! Returns the parent node.
    virtual Node* parent() const=0;

    //! Returns the linear velocity of the object.
    virtual const Vector& linear_velocity() const=0;

    //! Returns the angular velocity of the object.
    virtual const Vector& angular_velocity() const=0;
    
    //! Returns total force on this object in world coordinates.
    virtual const Vector& force() const=0;

    //! Sets the linear velocity of the object.
    //! @param v the new linear velocity
    virtual void linear_velocity(const Vector& v)=0;

    //! Sets the angular velocity of the object.
    //! @param v the new angular velocity
    virtual void angular_velocity(const Vector& v)=0;

    //! Applies a force to the object, relative to the world coordinates.
    //! @param v the force to apply
    virtual void apply_force(const Vector& v)=0;

    //! Applies a torque to the object, relative to the world coordinates.
    //! @param v the torque to apply
    virtual void apply_torque(const Vector& v)=0;

    //! Applies a force to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the force to apply
    virtual void apply_local_force(const Vector& v)=0;

    //! Applies a torque to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the torque to apply
    virtual void apply_local_torque(const Vector& v)=0;
};


}