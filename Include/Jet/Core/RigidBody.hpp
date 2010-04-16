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

#include <Jet/RigidBody.hpp>

namespace Jet { namespace Core {

//! Physical simulation.  This class is used for simple rigid body dynamics.
//! Access to the underlying physics engine, if needed, can be obtained by
//! compiling your program with the headers for the physics engine plugin you
//! are using.
//! @class RigidBody
//! @brief Physical simulation with collisions.
class RigidBody : public Jet::RigidBody {
public:
    
    //! Returns the parent node.
    inline Node* parent() const {
        return parent_;
    }

    //! Returns the linear velocity of the object.
    inline const Vector& linear_velocity() const {
        return linear_velocity_;
    }

    //! Returns the angular velocity of the object.
    inline const Vector& angular_velocity() const {
        return angular_velocity_;
    }
    
    //! Returns total force on this object in world coordinates.
    inline const Vector& force() const {
        return force_;
    }

    //! Sets the linear velocity of the object.
    //! @param v the new linear velocity
    inline void linear_velocity(const Vector& v) {
        
    }
    
    //! Sets the angular velocity of the object.
    //! @param v the new angular velocity
    inline void angular_velocity(const Vector& v) {
        
    }

    //! Applies a force to the object, relative to the world coordinates.
    //! @param v the force to apply
    inline void apply_force(const Vector& v) {
        
    }

    //! Applies a torque to the object, relative to the world coordinates.
    //! @param v the torque to apply
    inline void apply_torque(const Vector& v) {
        
    }

    //! Applies a force to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the force to apply
    inline void apply_local_force(const Vector& v) {
        
    }

    //! Applies a torque to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the torque to apply
    inline void apply_local_torque(const Vector& v) {
        
    }

private:
    inline RigidBody(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent) {
    }
    
    Engine* engine_;
    Node* parent_;
    Vector linear_velocity_;
    Vector angular_velocity_;
    Vector force_;
    Vector torque_;

    friend class Node;
};

}}