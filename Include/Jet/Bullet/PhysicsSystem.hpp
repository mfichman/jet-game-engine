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

#include <Jet/Bullet/Types.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Physics.hpp>
#include <vector>
#include <memory>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>

namespace Jet { namespace Bullet {

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class PhysicsSystem
//! @brief Rigid body physics engine
class PhysicsSystem : public Physics, public EngineListener {
public:
    PhysicsSystem(Core::Engine* engine);

    //! Destructor.
    virtual ~PhysicsSystem();
    
    //! Returns the physics world
    inline btDynamicsWorld* world() const {
        return world_.get();
    }

private:
    void on_tick() {}
    void on_init();
    void on_update();
    void on_render() {}
    
    static void on_tick(btDynamicsWorld* world, btScalar step);    
    Core::Engine* engine_;
    
    std::auto_ptr<btCollisionConfiguration> config_;
    std::auto_ptr<btCollisionDispatcher> dispatcher_;
    std::auto_ptr<btBroadphaseInterface> broadphase_;
    std::auto_ptr<btConstraintSolver> solver_;
    std::auto_ptr<btDynamicsWorld> world_;

    
};

}}