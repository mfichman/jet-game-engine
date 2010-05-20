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

#include <Jet/Bullet/PhysicsSystem.hpp>
#include <Jet/Bullet/RigidBody.hpp>
#include <Jet/Bullet/Geometry.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/FractureObject.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>

using namespace Jet;
using namespace std;

Bullet::RigidBody::RigidBody(Core::Engine* engine, Core::Node* parent) :
    engine_(engine),
    parent_(parent),
    mass_(0.0f),
	active_(false) {
        
    shape_.reset(new btCompoundShape);
    
    //btVector3 inertia(0.0f, 0.0f, 0.0f);
    //shape_->calculateLocalInertia(mass_, inertia);
    
    body_.reset(new btRigidBody(mass_, this, shape_.get()));
    body_->setUserPointer(this);
    body_->setSleepingThresholds(0.03f, 0.01f);
	
	active(parent_->visible());    
    update_collision_shapes();
}

Bullet::RigidBody::~RigidBody() {
	active(false);
}

void Bullet::RigidBody::getWorldTransform(btTransform& transform) const {
    const Quaternion& rotation = parent_->rotation();
    const Vector& position = parent_->position();
    
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    transform.setOrigin(btVector3(position.x, position.y, position.z));
}

void Bullet::RigidBody::setWorldTransform(const btTransform& transform) {
    // This function is called by Bullet to push updates from Bullet.
    // Therefore, we set the rotation and position of our parent node
    // accordingly.
    const btQuaternion& rotation = transform.getRotation();
    const btVector3& position = transform.getOrigin();
    
    parent_->raw_rotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    parent_->raw_position(Vector(position.x(), position.y(), position.z()));
}

void Bullet::RigidBody::update_collision_shapes() {
    // Clear old shapes out, then update the new compound shape
    // with the shapes of all sub-objects in the node tree
    while (shape_->getNumChildShapes() > 0) {
        shape_->removeChildShapeByIndex(shape_->getNumChildShapes() - 1);
    }

    attach_node(btTransform::getIdentity(), parent_);
}

void Bullet::RigidBody::attach_node(const btTransform& transform, Core::Node* node) {
    node->rigid_body(this);
    
	
    // Iterate through all child objects of "node".  For child MeshObjects,
	// make sure they are attached to the rigid body.  For child Nodes, make
	// sure that the node has a reference to the new rigid body.
    for (Iterator<ObjectPtr> i = node->objects(); i; i++) {
        const type_info& info = typeid(**i);
        if (typeid(Core::Node) == info) {
            // Add mesh objects connected to the child node
            Core::Node* node = static_cast<Core::Node*>(i->get());
            btQuaternion rotation(node->rotation().x, node->rotation().y, node->rotation().z, node->rotation().w);
            btVector3 position(node->position().x, node->position().y, node->position().z);
            attach_node(transform * btTransform(rotation, position), node);
            
        } else if (typeid(Core::MeshObject) == info) {
            // We found a mesh object, so add it to the rigid body
            Core::MeshObject* mesh_object = static_cast<Core::MeshObject*>(i->get());
            attach_mesh_object(transform, mesh_object);
            
        }
    }
}

void Bullet::RigidBody::attach_mesh_object(const btTransform& transform, Core::MeshObject* mesh_object) {
    // Attach the given mesh object to the node, using the local
    // transform.  
    Mesh* mesh = mesh_object->mesh();
    if (mesh) {
        if (UNLOADED == mesh->state()) {
            mesh->state(LOADED);
        }
        // This is for triangle mesh shapes
        Bullet::Geometry* geometry = static_cast<Bullet::Geometry*>(mesh->geometry());
        shape_->addChildShape(transform, geometry->shape());
    }
}

void Bullet::RigidBody::apply_force(const Vector& v) {
    body_->activate(true);
    body_->applyCentralForce(btVector3(v.x, v.y, v.z));
}

void Bullet::RigidBody::apply_torque(const Vector& v) {
	body_->activate(true);
    body_->applyTorque(btVector3(v.x, v.y, v.z));
}

void Bullet::RigidBody::apply_local_force(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 force = transform * btVector3(v.x, v.y, v.z);
    body_->applyCentralForce(force);
}

void Bullet::RigidBody::apply_local_torque(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 torque = transform * btVector3(v.x, v.y, v.z);
    body_->applyTorque(torque);
}

void Bullet::RigidBody::mass(float mass) {
    mass_ = mass;
    btVector3 inertia(0.0f, 0.0f, 0.0f);
    shape_->calculateLocalInertia(mass, inertia);
    body_->setMassProps(mass, inertia);
    body_->updateInertiaTensor();
    body_->activate(true);

	// If the body is active, remove and re-add it back into the world.
	// Otherwise, Bullet won't update the object's mass from zero to 
	// a non-zero mass (this will cause the object to be static even
	// though it has a positive mass).
	if (active_) {
        PhysicsSystem* physics = static_cast<PhysicsSystem*>(engine_->physics());
		physics->world()->removeCollisionObject(body_.get());
		physics->world()->addRigidBody(body_.get());
	}
    
    update_collision_shapes();
}


void Bullet::RigidBody::active(bool active) {
	if (active_ != active) {
		active_ = active;
        PhysicsSystem* physics = static_cast<PhysicsSystem*>(engine_->physics());
		if (active_) {
			physics->world()->addRigidBody(body_.get());
		} else {
			physics->world()->removeCollisionObject(body_.get());
		}
	}
}


//! Sets the rigid body position
void Bullet::RigidBody::position(const Vector& position) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    body_->setCenterOfMassTransform(transform);
}

//! Sets the rigid body rotation
void Bullet::RigidBody::rotation(const Quaternion& rotation) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    body_->setCenterOfMassTransform(transform);
}