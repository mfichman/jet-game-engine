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

Core::PhysicsSystem::PhysicsSystem(Engine* engine) :
    engine_(engine) {
        
    config_.reset(new btDefaultCollisionConfiguration);
    dispatcher_.reset(new btCollisionDispatcher(config_.get()));
    broadphase_.reset(new btDbvtBroadphase);
    solver_.reset(new btSequentialImpulseConstraintSolver);
	world_.reset(new btDiscreteDynamicsWorld(dispatcher_.get(), broadphase_.get(), solver_.get(), config_.get()));
    world_->setWorldUserInfo(this);
	world_->setGravity(btVector3(0.0f, -10.0f, 0.0f));
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher_.get());
}

Core::PhysicsSystem::~PhysicsSystem() {
    
}

void Core::PhysicsSystem::on_init() {
    std::cout << "Initializing physics system" << std::endl;
}

void Core::PhysicsSystem::on_update() {
    world_->stepSimulation(engine_->timestep()*engine_->simulation_speed(), 0);
}

void Core::PhysicsSystem::on_render() {
    
}