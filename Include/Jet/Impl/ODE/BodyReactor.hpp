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

#include <Jet/Body.hpp>
#include <Jet/Root.hpp>
#include <ode/ode.h>

namespace Jet { namespace Impl { namespace ODE {
using namespace std;
using namespace std::tr1;
using namespace boost;

class RootReactor;

class BodyReactor : public Body::Observer {
public:
    typedef intrusive_ptr<BodyReactor> Ptr;

    BodyReactor(Body::Ptr a, RootReactor* e);
    ~BodyReactor();

    static void onBodyMoved(dBodyID);    
    
    virtual void onLinearVelocity();
    virtual void onAngularVelocity();
    virtual void onForce();
    virtual void onTorque();
    virtual void onMass();
    virtual void onRadius();
    virtual void onPosition();
    virtual void onRotation();
    virtual void onAttachmentAdd(const Attachment&);
    virtual void onAttachmentDel(const Attachment&);

private:
    dBodyID id_;
    Body::Ptr body_;
    Root::Ptr root_;
};

}}}
