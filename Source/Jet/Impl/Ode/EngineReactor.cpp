/*
 * Copyright (c) 2008 Matt Fichman
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
 
#include <Jet/Impl/Ode/EngineReactor.hpp>

using namespace Jet::Impl::Ode;
using namespace Jet::Physics;

class Initializer {
public:
    inline Initializer() { dInitODE(); }
    inline ~Initializer() { dCloseODE(); }
};
Initializer init;

EngineReactor::EngineReactor(Engine::Ptr e) :
    engine_(e) {
    
    world_ = dWorldCreate();
    space_ = dSimpleSpaceCreate(0);
    joints_ = dJointGroupCreate(1024);
    contact_.surface.mode = dContactBounce;
    contact_.surface.mu = 0.2f;
    contact_.surface.bounce_vel = 0.0f;
}

EngineReactor::~EngineReactor() {
    dJointGroupDestroy(joints_);
    dSpaceDestroy(space_);
    dWorldDestroy(world_);
}

void 
EngineReactor::onObjectCreate(Object::Ptr o) {
    
}
 
void 
EngineReactor::onSphereCreate(Sphere::Ptr c) {

}
    
void 
EngineReactor::onBoxCreate(Box::Ptr s) {

}

void
EngineReactor::onPlaneCreate(Plane::Ptr m) {

}

void 
EngineReactor::onRayCreate(Ray::Ptr s) {

}
