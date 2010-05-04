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
#include <Jet/Core/Node.hpp>
#include <Jet/Core/PhysicsSystem.hpp>
#include <Jet/RigidBody.hpp>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>

namespace Jet { namespace Core {

//! Physical simulation.  This class is used for simple rigid body dynamics.
//! Access to the underlying physics engine, if needed, can be obtained by
//! compiling your program with the headers for the physics engine plugin you
//! are using.
//! @class RigidBody
//! @brief Physical simulation with collisions.
class RigidBody : public Jet::RigidBody, public btMotionState {
public:
    //! Creates a new rigid body with the given parent node.
    RigidBody(Engine* engine, Node* parent);
    
    //! Destructor
    virtual ~RigidBody();
    
    //! Returns the parent node.
    inline Node* parent() const {
        return parent_;
    }

    //! Returns the linear velocity of the object.
    inline Vector linear_velocity() const {
		const btVector3& v = body_->getLinearVelocity();
        return Vector(v.x(), v.y(), v.z());
    }

    //! Returns the angular velocity of the object.
    inline Vector angular_velocity() const {
        const btVector3& v = body_->getAngularVelocity();
		return Vector(v.x(), v.y(), v.z());
    }
    
    //! Returns the mass
    inline real_t mass() const {
        return mass_;
    }

    //! Returns the rigid body shape.
    inline btCollisionShape* shape() const {
        return shape_.get();
    }

    //! Sets the linear velocity of the object.
    //! @param v the new linear velocity
    inline void linear_velocity(const Vector& v) {
        body_->setLinearVelocity(btVector3(v.x, v.y, v.z));
    }
    
    //! Sets the angular velocity of the object.
    //! @param v the new angular velocity
    inline void angular_velocity(const Vector& v) {
        body_->setAngularVelocity(btVector3(v.x, v.y, v.z));
    }

    //! Applies a force to the object, relative to the world coordinates.
    //! @param v the force to apply
    void apply_force(const Vector& v);

    //! Applies a torque to the object, relative to the world coordinates.
    //! @param v the torque to apply
    void apply_torque(const Vector& v);

    //! Applies a force to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the force to apply
    void apply_local_force(const Vector& v);

    //! Applies a torque to the object, relative to the parent scene node's
    //! coordinates.
    //! @param v the torque to apply
    void apply_local_torque(const Vector& v);
    
    //! Sets the mass of the rigid body.
    void mass(real_t mass);
    
    //! Updates the collision shapes attached to this rigid body
    void update_collision_shapes();
  
    
private:    
    void getWorldTransform(btTransform& transform) const;
    void setWorldTransform(const btTransform& transform);
    void attach_mesh_object(const btTransform& trans, MeshObject* mesh_object);
    void attach_node(const btTransform& transform, Node* node);
    
    Engine* engine_;
    Node* parent_;
    real_t mass_;

    std::auto_ptr<btRigidBody> body_;
    std::auto_ptr<btCompoundShape> shape_;
    
    friend class Node;
};

}}