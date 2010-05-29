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
#include <Jet/Core/CoreNode.hpp>

using namespace Jet;
using namespace std;

BulletPhysics::BulletPhysics(CoreEngine* engine) :
    engine_(engine) {
        
    engine_->listener(this);
        
    // Blah blah blah config blah blah
    config_.reset(new btDefaultCollisionConfiguration);
    dispatcher_.reset(new btCollisionDispatcher(config_.get()));
    broadphase_.reset(new btDbvtBroadphase);
    solver_.reset(new btSequentialImpulseConstraintSolver);
	world_.reset(new btDiscreteDynamicsWorld(dispatcher_.get(), broadphase_.get(), solver_.get(), config_.get()));
	world_->setGravity(btVector3(0.0f, -10.0f, 0.0f));
    
    // Enable the pre-tick callback
	world_->setInternalTickCallback(&BulletPhysics::on_post_tick, this, false);
    world_->setInternalTickCallback(&BulletPhysics::on_pre_tick, this, true);
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher_.get());    
}

BulletPhysics::~BulletPhysics() {
    
}

void BulletPhysics::on_update() {
    float gravity = engine_->option<float>("gravity");
    world_->setGravity(btVector3(0.0f, -gravity, 0.0f));
    
    // Step simulation returns how many substeps were taken.  If at least one
    // substep was taken, then we have to update matrices for our objects.
	world_->stepSimulation(engine_->frame_delta(), 4, engine_->timestep());
}

void BulletPhysics::on_init() {
    std::cout << "Initializing physics system" << std::endl;
}

void BulletPhysics::on_post_tick(btDynamicsWorld* world, btScalar step) {
    BulletPhysics* system = static_cast<BulletPhysics*>(world->getWorldUserInfo());

    // Update all engine listeners
    for (Iterator<EngineListenerPtr> i = system->engine_->listeners(); i; i++) {
        (*i)->on_tick();
    }

}

void BulletPhysics::on_pre_tick(btDynamicsWorld* world, btScalar step) {
    
    // Clear force, and apply gravity to all rigid bodies
    btCollisionObjectArray objects = world->getCollisionObjectArray();
    //world->clearForces();
    for (int i = 0; i < objects.size(); i++) {
         btRigidBody* rigid_body = btRigidBody::upcast(objects[i]);
         if (rigid_body) {
            rigid_body->applyGravity();
         }
    }

    BulletPhysics* system = static_cast<BulletPhysics*>(world->getWorldUserInfo());

    // Update the active module
    if (system->engine_->module()) {
        system->engine_->module()->on_tick();
    }
    
    // Update the nodes
    static_cast<CoreNode*>(system->engine_->root())->tick();
    
    // Check for collisions
    int nmanifolds = world->getDispatcher()->getNumManifolds();
    for (int i = 0; i < nmanifolds; i++) {
        btPersistentManifold* manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* a = static_cast<btCollisionObject*>(manifold->getBody0());
        btCollisionObject* b = static_cast<btCollisionObject*>(manifold->getBody1());

        BulletRigidBody* ca = static_cast<BulletRigidBody*>(a->getUserPointer());
        BulletRigidBody* cb = static_cast<BulletRigidBody*>(b->getUserPointer());
        
        if (manifold->getNumContacts() > 0) {
        
            btVector3 pa = manifold->getContactPoint(0).getPositionWorldOnA();
            btVector3 pb = manifold->getContactPoint(0).getPositionWorldOnB();
    
            ca->parent()->collision(cb->parent(), Vector(pa.x(), pa.y(), pa.z()));
            cb->parent()->collision(ca->parent(), Vector(pb.x(), pb.y(), pb.z()));
        }
    }

	system->engine_->tick_id_inc();
    
    return;
}
