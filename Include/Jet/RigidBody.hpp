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

#include <Jet/Jet.hpp>
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
    //! Destructor.
    virtual ~RigidBody() {}
    // 1/2 at^2 + vt + x

    //! Returns the parent of this rigid body.
    virtual SceneNode* parent() const=0;

    //! Returns the linear velocity of the object.
    virtual const Vector& linear_velocity() const=0;

    //! Returns the angular velocity of the object.
    virtual const Vector& angular_velocity() const=0;

    //! Returns the total accumulated force on this object relative to the
    //! world coordinates.
    virtual const Vector& force()=0;

    //! Returns the total accumulated torque on this object relative to the
    //! world coordinates.
    virtual const Vector& torque()=0;

    //! Sets the linear velocity of the object.
    //! @param v the new linear velocity
    virtual void linear_velocity(const Vector& v)=0;

    //! Sets the angular velocity of the object.
    //! @param v the new angular velocity
    virtual void angular_velocity(const Vector& v)=0;

    //! Applies a force to the object, relative to the world coordinates.
    //! @param v the force to apply
    virtual void force_add(const Vector& v)=0;

    //! Applies a torque to the object, relative to the world coordinates.
    //! @param v the torque to apply
    virtual void torque_add(const Vector& v)=0;

    //! Applies a force to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the force to apply
    virtual void local_force_add(const Vector& v)=0;

    //! Applies a torque to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the torque to apply
    virtual void local_torque_add(const Vector& v)=0;

    //! Clears all accumulated forces on this object.
    virtual void force_clear();

    //! Clears all accumulated torques on this object.
    virtual void torque_clear();
};

}
