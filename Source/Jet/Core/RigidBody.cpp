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

#include <Jet/Core/RigidBody.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/PhysicsSystem.hpp>

using namespace Jet;

Core::RigidBody::RigidBody(Engine* engine, Node* parent) :
    engine_(engine),
    parent_(parent),
    mass_(0.0f) {

    shape_.reset(new btCompoundShape);
    
    //btVector3 inertia(0.0f, 0.0f, 0.0f);
    //shape_->calculateLocalInertia(mass_, inertia);
    
    body_.reset(new btRigidBody(mass_, this, shape_.get()));
    body_->setUserPointer(this);
    engine_->physics_system()->world()->addRigidBody(body_.get());
    
}

Core::RigidBody::~RigidBody() {
    engine_->physics_system()->world()->removeCollisionObject(body_.get());
}

void Core::RigidBody::getWorldTransform(btTransform& transform) const {
    const Quaternion& rotation = parent_->rotation();
    const Vector& position = parent_->position();
    
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    transform.setOrigin(btVector3(position.x, position.y, position.z));
}

void Core::RigidBody::setWorldTransform(const btTransform& transform) {
    const btQuaternion& rotation = transform.getRotation();
    const btVector3& position = transform.getOrigin();
    
    parent_->rotation_ = Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    parent_->position_ = Vector(position.x(), position.y(), position.z());
}
