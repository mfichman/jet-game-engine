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

#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <Jet/Publisher.hpp>
#include <iostream>
#include <list>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;

class Object : public Interface {
public:
    class Observer;
    typedef intrusive_ptr<Object> Ptr;
    enum NetworkSync { disabled, enabled };

    // Attributes    
    inline const Vector&        position() const { return position_; }
    void                        position(const Vector& v);
    inline const Quaternion&    rotation() const { return rotation_; }
    void                        rotation(const Quaternion& q);
    inline NetworkSync          networkSync() const { return networkSync_; }
    void                        networkSync(NetworkSync s);

    // Utility
    inline Publisher<Observer>& publisher() const { publisher_; }

private:
    mutable Publisher<Observer> publisher_;
    NetworkSync networkSync_;
    Vector position_;
    Quaternion rotation_;
    Vector linearVelocity_;
    Vector angularVelocity_;
    Vector force_;
    Vector torque_;
};

class Object::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Observer> Ptr;
    
    virtual void onPosition()=0;
    virtual void onRotation()=0;
    virtual void onNetworkSync()=0;
};

}
