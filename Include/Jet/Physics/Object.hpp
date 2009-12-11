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
#include <iostream>

namespace Jet { namespace Physics {
using namespace std;
using namespace std::tr1;
using namespace boost;

class Object : public Interface {
public:
    class Listener;
    typedef intrusive_ptr<Object> Ptr;
    typedef intrusive_ptr<Listener> ListenerPtr;
    
	static Object::Ptr	make() { return Object::Ptr(new Object()); }
    
    inline Object::Ptr  parent() const { return parent_; }
	void                parent(Object::Ptr p);
    inline Vector       position() const { return position_; }
    void                position(const Vector& v); // Has no effect if the object is attached
    inline Quaternion   rotation() const { return rotation_; }
    void                rotation(const Quaternion& q); // Has no effect if the object is attached
    inline Vector       linearVelocity() const { return velocity_; }
    void                linearVelocity(const Vector& v); // Has no effect if the object is attached
    inline Vector       angularVelocity() const { return angularVelocity_; }
    void                angularVelocity(const Vector& v); // Has no effect if the object is attached
    inline Vector       force() const { return force_; }
    void                force(const Vector& v); // Has no effect if the object is attached
    void                forceAdd(const Vector& v); // Has no effect if the object is attached
    inline Vector       torque() const { return torque_; }
    void                torque(const Vector& v); // Has no effect if the object is attached
    void                torqueAdd(const Vector& v); // Has no effect if the object is attached
    inline bool         dynamic() const { return dynamic_; }
    void                dynamic(bool d); // Has no effect if the object is attached
    
    inline Vector       worldPosition();
    inline Quaternion   worldRotation();

    inline ListenerPtr  listener() const { return listener_; }
    void                listener(ListenerPtr l);

private:
    Object::Ptr     parent_;
    Vector          position_;
    Quaternion      rotation_;
    Vector          velocity_;
    Vector          angularVelocity_;
    Vector          force_;
    Vector          torque_;
    bool            dynamic_;
    
    ListenerPtr     listener_;
};

class Object::Listener : public Interface {
public:
    typedef intrusive_ptr<Listener> Ptr;
    
    virtual void onPosition()=0;
    virtual void onRotation()=0;
    virtual void onLinearVelocity()=0;
    virtual void onAngularVelocity()=0;
    virtual void onForce()=0;
    virtual void onTorque()=0;
    virtual void onObject()=0;
    virtual void onDynamic()=0;
};

}}
