/*
 * Copyright (c) 2009 Matt Fichman
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

#include <Jet/Model.hpp>
#include <Jet/Root.hpp>
#include <Jet/Impl/Ode/RootReactor.hpp>
#include <ode/ode.h>

namespace Jet { namespace Impl { namespace Ode {
using namespace std;
using namespace std::tr1;
using namespace boost;

class ActorReactor : public Actor::Observer, public Object::Observer {
public:
    typedef intrusive_ptr<ActorReactor> Ptr;

    ActorReactor(Actor::Ptr a, RootReactor::Ptr e);
    ~ActorReactor();

    void onLinearVelocity() {}
    void onAngularVelocity() {}
    void onForce() {}
    void onTorque() {}
    void onNetworkSync() {}
    void onPosition() {}
    void onRotation() {}

    inline dBodyID body() { return body_; }

private:
    RootReactor::Ptr rootReactor_;
    dBodyID body_;
    Actor::Ptr actor_;
};

}}}
