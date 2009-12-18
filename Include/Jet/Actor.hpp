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

#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <Jet/Object.hpp>
#include <Jet/Publisher.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Model.hpp>
#include <Jet/Quad.hpp>
#include <iostream>
#include <list>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class Actor : public Interface {
public:
    class Listener;
    friend class Root;
    typedef intrusive_ptr<Actor> Ptr;
    
    // Attributes
    inline Vector       linearVelocity() const { return linearVelocity_; }
    void                linearVelocity(const Vector& v);
    inline Vector       angularVelocity() const { return angularVelocity_; }
    void                angularVelocity(const Vector& v);
    inline Vector       force() const { return force_; }
    void                force(const Vector& v);
    void                forceAdd(const Vector& v);
    inline Vector       torque() const { return torque_; }
    void                torque(const Vector& v);
    void                torqueAdd(const Vector& v);

    // Components
    inline Object::Ptr  object() const { return object_; }

    // Utility
    inline Publisher<Listener>& publisher() const { return publisher_; }

private:
    Actor() : object_(new Object) {}    

    mutable Publisher<Listener> publisher_;
    Vector position_;
    Quaternion rotation_;
    Vector linearVelocity_;
    Vector angularVelocity_;
    Vector force_;
    Vector torque_;
    Object::Ptr object_;
};

class Actor::Listener : public Interface {
public:
    typedef intrusive_ptr<Actor::Listener> Ptr;
    
    virtual void onLinearVelocity()=0;
    virtual void onAngularVelocity()=0;
    virtual void onForce()=0;
    virtual void onTorque()=0;
};

}
