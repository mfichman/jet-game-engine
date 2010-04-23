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

#include <Jet/Core/PhysicsSystem.hpp>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace Jet;
using namespace std;

Core::PhysicsSystem::PhysicsSystem(Engine* engine) :
    engine_(engine) {
        
    config_.reset(new btDefaultCollisionConfiguration);
    dispatcher_.reset(new btCollisionDispatcher(config_.get()));
    broadphase_.reset(new btDbvtBroadphase);
    solver_.reset(new btSequentialImpulseConstraintSolver);
	world_.reset(new btDiscreteDynamicsWorld(dispatcher_.get(), broadphase_.get(), solver_.get(), config_.get()));
	world_->setGravity(btVector3(0.0f, -10.0f, 0.0f));
    world_->setInternalTickCallback(&Core::PhysicsSystem::on_tick, this, true);
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher_.get());

	cout << "GOOD" << endl;
    
}

Core::PhysicsSystem::~PhysicsSystem() {
    
}

void Core::PhysicsSystem::step() {
	cout << "delta: " << engine_->delta() << " step: " << engine_->timestep() << endl;
	world_->stepSimulation(engine_->delta()*engine_->simulation_speed(), 4, engine_->timestep());
}

void Core::PhysicsSystem::on_init() {
    std::cout << "Initializing physics system" << std::endl;
}

void Core::PhysicsSystem::on_update() {
}

void Core::PhysicsSystem::on_tick(btDynamicsWorld* world, btScalar step) {
    
    btCollisionObjectArray objects = world->getCollisionObjectArray();
    world->clearForces();
    for (int i = 0; i < objects.size(); i++) {
         btRigidBody* rigid_body = btRigidBody::upcast(objects[i]);
         if (rigid_body) {
            rigid_body->applyGravity();
         }
    }
	    
    PhysicsSystem* system = static_cast<PhysicsSystem*>(world->getWorldUserInfo());    
    std::list<EngineListenerPtr>& listener = system->engine_->listener_;
	
	for (list<EngineListenerPtr>::iterator i = listener.begin(); i != listener.end(); i++) {
        (*i)->on_update();
    }
    if (system->engine_->module()) {
        system->engine_->module()->on_update();
    }
    return;
}
