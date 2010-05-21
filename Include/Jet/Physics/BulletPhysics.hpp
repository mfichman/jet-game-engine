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

#include <Jet/Physics/BulletTypes.hpp>
#include <Jet/Physics/BulletGeometry.hpp>
#include <Jet/Physics/BulletRigidBody.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Physics.hpp>
#include <vector>
#include <memory>

namespace Jet { 

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class Physics
//! @brief Rigid body physics engine
class BulletPhysics : public Physics, public EngineListener {
public:
    BulletPhysics(CoreEngine* engine);

    //! Destructor.
    virtual ~BulletPhysics();
    
    //! Returns the physics world
    inline btDynamicsWorld* world() const {
        return world_.get();
    }

private:
    inline BulletGeometry* geometry(const std::string& name) {
        return new BulletGeometry(engine_, name);
    }
    
    inline BulletRigidBody* rigid_body(Node* parent) {
        return new BulletRigidBody(engine_, static_cast<CoreNode*>(parent));
    }
    
    void on_tick() {}
    void on_init();
    void on_update();
    void on_render() {}
    
    static void on_tick(btDynamicsWorld* world, btScalar step);    
    CoreEngine* engine_;
    
    std::auto_ptr<btCollisionConfiguration> config_;
    std::auto_ptr<btCollisionDispatcher> dispatcher_;
    std::auto_ptr<btBroadphaseInterface> broadphase_;
    std::auto_ptr<btConstraintSolver> solver_;
    std::auto_ptr<btDynamicsWorld> world_;

    
};

}