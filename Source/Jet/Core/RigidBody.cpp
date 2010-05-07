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
#include <Jet/Core/FractureObject.hpp>
#include <Jet/Core/PhysicsSystem.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>

using namespace Jet;
using namespace std;

Core::RigidBody::RigidBody(Engine* engine, Node* parent) :
    engine_(engine),
    parent_(parent),
    mass_(0.0f) {
        
    shape_.reset(new btCompoundShape);
    
    //btVector3 inertia(0.0f, 0.0f, 0.0f);
    //shape_->calculateLocalInertia(mass_, inertia);
    
    body_.reset(new btRigidBody(mass_, this, shape_.get()));
    body_->setUserPointer(this);
    body_->setSleepingThresholds(0.3f, 0.1f);
    engine_->physics_system()->world()->addRigidBody(body_.get());
    
    update_collision_shapes();
    
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
    // This function is called by Bullet to push updates from Bullet.
    // Therefore, we set the rotation and position of our parent node
    // accordingly.
    const btQuaternion& rotation = transform.getRotation();
    const btVector3& position = transform.getOrigin();
    
    parent_->rotation_ = Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    parent_->position_ = Vector(position.x(), position.y(), position.z());
    parent_->transform_modified_count_++;
}

void Core::RigidBody::update_collision_shapes() {
    // Clear old shapes out, then update the new compound shape
    // with the shapes of all sub-objects in the node tree
    while (shape_->getNumChildShapes() > 0) {
        shape_->removeChildShapeByIndex(shape_->getNumChildShapes() - 1);
    }

    attach_node(btTransform::getIdentity(), parent_);
}

void Core::RigidBody::attach_node(const btTransform& transform, Node* node) {
    node->rigid_body_ = this;
    
	
    // Iterate through all child objects of "node".  For child MeshObjects,
	// make sure they are attached to the rigid body.  For child Nodes, make
	// sure that the node has a reference to the new rigid body.
    for (tr1::unordered_map<string, ObjectPtr>::iterator i = node->object_.begin(); i != node->object_.end(); i++) {
        const type_info& info = typeid(*i->second);
        if (typeid(Node) == info) {
            // Add mesh objects connected to the child node
            Node* node = static_cast<Node*>(i->second.get());
            btQuaternion rotation(node->rotation().x, node->rotation().y, node->rotation().z, node->rotation().w);
            btVector3 position(node->position().x, node->position().y, node->position().z);
            attach_node(transform * btTransform(rotation, position), node);
            
        } else if (typeid(MeshObject) == info) {
            // We found a mesh object, so add it to the rigid body
            MeshObject* mesh_object = static_cast<MeshObject*>(i->second.get());
            attach_mesh_object(transform, mesh_object);
            
        }
    }
}

void Core::RigidBody::attach_mesh_object(const btTransform& transform, MeshObject* mesh_object) {
    // Attach the given mesh object to the node, using the local
    // transform.  
    Mesh* mesh = mesh_object->mesh();
    if (mesh) {
        if (UNLOADED == mesh->state()) {
            mesh->state(SYNCED);
        }
        // This is for triangle mesh shapes
        shape_->addChildShape(transform, mesh->shape());
    }
}

void Core::RigidBody::apply_force(const Vector& v) {
    body_->activate(true);
    body_->applyCentralForce(btVector3(v.x, v.y, v.z));
}

void Core::RigidBody::apply_torque(const Vector& v) {
    body_->applyTorque(btVector3(v.x, v.y, v.z));
}

void Core::RigidBody::apply_local_force(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 force = transform * btVector3(v.x, v.y, v.z);
    body_->applyCentralForce(force);
}

void Core::RigidBody::apply_local_torque(const Vector& v) {
    btTransform transform = body_->getCenterOfMassTransform();
    transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btVector3 torque = transform * btVector3(v.x, v.y, v.z);
    body_->applyTorque(torque);
}

void Core::RigidBody::mass(float mass) {
    mass_ = mass;
    btVector3 inertia(0.0f, 0.0f, 0.0f);
    shape_->calculateLocalInertia(mass, inertia);
    body_->setMassProps(mass, inertia);
    body_->updateInertiaTensor();
    body_->activate(true);
    engine_->physics_system()->world()->removeCollisionObject(body_.get());
    engine_->physics_system()->world()->addRigidBody(body_.get());
}

