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

#include <Jet/Physics/Object.hpp>
#include <ode/ode.h>

namespace Jet { namespace Impl { namespace Ode {
using namespace Physics;
class EngineReactor;

class ObjectReactor : public Object::Listener {

public:
    typedef intrusive_ptr<ObjectReactor> Ptr;
    typedef intrusive_ptr<EngineReactor> EngineReactorPtr;
    
    ObjectReactor(Object::Ptr o, EngineReactorPtr e);
    ~ObjectReactor();
    void onPosition();
    void onRotation();
    void onLinearVelocity();
    void onAngularVelocity();
    void onForce();
    void onTorque();
    void onObject();
    void onDynamic();
    inline Object::Ptr object() { return object_; }
    inline dGeomID geom() { return geom_; }
    inline dBodyID body() { return body_; }
    inline dMass mass() { return mass_; }
    void massAdd(const dMass& mass);
    void massSub(const dMass& mass);
    
private:
    ObjectReactor::Ptr parent_;
    dGeomID geom_;
    dBodyID body_;
    dMass mass_;
    Object::Ptr object_;
    EngineReactorPtr engine_;
};

}}}
