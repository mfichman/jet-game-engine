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

#include <Jet/Physics/BulletPhysics.hpp>
#include <Jet/Physics/BulletRigidBody.hpp>
#include <Jet/Physics/BulletGeometry.hpp>
#include <Jet/Core/CoreMeshObject.hpp>
#include <Jet/Core/CoreFractureObject.hpp>
#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Quaternion.hpp>

using namespace Jet;
using namespace std;

BulletRigidBody::BulletRigidBody(CoreEngine* engine, CoreNode* parent) :
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

BulletRigidBody::~BulletRigidBody() {
	active(false);
}

void BulletRigidBody::getWorldTransform(btTransform& transform) const {
    const Quaternion& rotation = parent_->rotation();
    const Vector& position = parent_->position();
    
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    transform.setOrigin(btVector3(position.x, position.y, position.z));
}

void BulletRigidBody::setWorldTransform(const btTransform& transform) {
    // This function is called by Physics to push updates from Physics.
    // Therefore, we set the rotation and position of our parent node
    // accordingly.
    const btQuaternion& rotation = transform.getRotation();
    const btVector3& position = transform.getOrigin();
    
    parent_->raw_rotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    parent_->raw_position(Vector(position.x(), position.y(), position.z()));
}

void BulletRigidBody::update_collision_shapes() {
    // Clear old shapes out, then update the new compound shape
    // with the shapes of all sub-objects in the node tree
    while (shape_->getNumChildShapes() > 0) {
        shape_->removeChildShapeByIndex(shape_->getNumChildShapes() - 1);
    }

    attach_node(btTransform::getIdentity(), parent_);
}

void BulletRigidBody::attach_node(const btTransform& transform, CoreNode* node) {
    node->rigid_body(this);
    
	
    // Iterate through all child objects of "node".  For child MeshObjects,
	// make sure they are attached to the rigid body.  For child Nodes, make
	// sure that the node has a reference to the new rigid body.
    for (Iterator<ObjectPtr> i = node->objects(); i; i++) {
		Object& obj = **i;
		const type_info& info = typeid(obj);
        if (typeid(CoreNode) == info) {
            // Add mesh objects connected to the child node
            CoreNode* node = static_cast<CoreNode*>(i->get());
            btQuaternion rotation(node->rotation().x, node->rotation().y, node->rotation().z, node->rotation().w);
            btVector3 position(node->position().x, node->position().y, node->position().z);
            attach_node(transform * btTransform(rotation, position), node);
            
        } else if (typeid(CoreMeshObject) == info) {
            // We found a mesh object, so add it to the rigid body
            CoreMeshObject* mesh_object = static_cast<CoreMeshObject*>(i->get());
            attach_mesh_object(transform, mesh_object);
            
        }
    }
}

void BulletRigidBody::attach_mesh_object(const btTransform& transform, CoreMeshObject* mesh_object) {
    // Attach the given mesh object to the node, using the local
    // transform.  
    Mesh* mesh = mesh_object->mesh();
    if (mesh) {
        if (RS_UNLOADED == mesh->state()) {
            mesh->state(RS_LOADED);
        }
        // This is for triangle mesh shapes
        BulletGeometry* geometry = static_cast<BulletGeometry*>(mesh->geometry());
        shape_->addChildShape(transform, geometry->shape());
    }
}

void BulletRigidBody::apply_force(const Vector& v) {
    body_->activate(true);
    body_->applyCentralForce(btVector3(v.x, v.y, v.z));
}

void BulletRigidBody::apply_torque(const Vector& v) {
	body_->activate(true);
    body_->applyTorque(btVector3(v.x, v.y, v.z));
}

void BulletRigidBody::apply_local_force(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 force = transform * btVector3(v.x, v.y, v.z);
    body_->applyCentralForce(force);
}

void BulletRigidBody::apply_local_torque(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 torque = transform * btVector3(v.x, v.y, v.z);
    body_->applyTorque(torque);
}

void BulletRigidBody::mass(float mass) {
    mass_ = mass;
    btVector3 inertia(0.0f, 0.0f, 0.0f);
    shape_->calculateLocalInertia(mass, inertia);
    body_->setMassProps(mass, inertia);
    body_->updateInertiaTensor();
    body_->activate(true);

	// If the body is active, remove and re-add it back into the world.
	// Otherwise, Physics won't update the object's mass from zero to 
	// a non-zero mass (this will cause the object to be static even
	// though it has a positive mass).
	if (active_) {
        BulletPhysics* physics = static_cast<BulletPhysics*>(engine_->physics());
		physics->world()->removeCollisionObject(body_.get());
		physics->world()->addRigidBody(body_.get());
	}
    
    update_collision_shapes();
}


void BulletRigidBody::active(bool active) {
	if (active_ != active) {
		active_ = active;
        BulletPhysics* physics = static_cast<BulletPhysics*>(engine_->physics());
		if (active_) {
			physics->world()->addRigidBody(body_.get());
		} else {
			physics->world()->removeCollisionObject(body_.get());
		}
	}
}


//! Sets the rigid body position
void BulletRigidBody::position(const Vector& position) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    body_->setCenterOfMassTransform(transform);
}

//! Sets the rigid body rotation
void BulletRigidBody::rotation(const Quaternion& rotation) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    body_->setCenterOfMassTransform(transform);
}
