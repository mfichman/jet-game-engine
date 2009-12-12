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
#pragma once

#include <Jet/Physics/Engine.hpp>
#include <Jet/Impl/Ode/ObjectReactor.hpp>
#include <ode/ode.h>

namespace Jet { namespace Impl { namespace Ode {
using namespace Physics;
class ObjectReactor;

class EngineReactor : public Engine::Listener {

public:
    typedef intrusive_ptr<EngineReactor> Ptr;

    EngineReactor(Engine::Ptr e);
    ~EngineReactor();
    void onObjectCreate(Object::Ptr o);
    void onSphereCreate(Sphere::Ptr c);
    void onBoxCreate(Box::Ptr s);
    void onPlaneCreate(Plane::Ptr m);
    void onRayCreate(Ray::Ptr s);
    
    inline dWorldID world() { return world_; }
    inline dSpaceID space() { return space_; }
    inline ObjectReactor::Ptr reactor(Object::Ptr o) { return reactor_[o]; }
    
private:
    Engine::Ptr engine_;
    dWorldID world_;
    dSpaceID space_;
    dJointGroupID joints_;
    dContact contact_;
    map<Object::Ptr, ObjectReactor::Ptr> reactor_;
};

}}}
