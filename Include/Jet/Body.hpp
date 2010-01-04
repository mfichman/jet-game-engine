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
#include <Jet/Object.hpp>
#include <Jet/Iterator.hpp>
#include <vector>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class JETAPI Body : public Interface {
public:
    class Observer;
    class Functor;
    friend class Root;
    typedef intrusive_ptr<Body> Ptr;
    typedef Iterator<vector<Attachment> > AttachmentItr;
    
    // Attributes
    inline Vector               linearVelocity() const { return linearVelocity_; }
    void                        linearVelocity(const Vector& v);
    void                        linearVelocityUpdate(const Vector& v);
    inline Vector               angularVelocity() const { return angularVelocity_; }
    void                        angularVelocity(const Vector& v);
    void                        angularVelocityUpdate(const Vector& v);
    inline Vector               force() const { return force_; }
    void                        force(const Vector& v);
    void                        forceAdd(const Vector& v);
    inline Vector               torque() const { return torque_; }
    void                        torque(const Vector& v);
    void                        torqueAdd(const Vector& v);
    inline Real                 mass() const { return mass_; }
    void                        mass(Real f);
    inline Real                 radius() const { return radius_; }
    void                        radius(Real r);
    inline const Vector&        position() const { return position_; }
    void                        position(const Vector& v);
    void                        positionUpdate(const Vector& v);
    inline const Quaternion&    rotation() const { return rotation_; }
    void                        rotation(const Quaternion& q);
    void                        rotationUpdate(const Quaternion& q);
    inline AttachmentItr        attachmentItr() { return AttachmentItr(attachment_); }
    void                        attachmentAdd(const Attachment& a);
    void                        attachmentDel(const Attachment& a);

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; }

private:
    Body() : mass_(0.000001f) {}    

    mutable Publisher<Observer> publisher_;
    Vector linearVelocity_;
    Vector angularVelocity_;
    Vector force_;
    Vector torque_;
    Real mass_;
    Real radius_;
    Vector position_;
    Quaternion rotation_;
    vector<Attachment> attachment_;
};

class Body::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Body::Observer> Ptr;
    
    virtual void onLinearVelocity() {}
    virtual void onAngularVelocity() {}
    virtual void onForce() {}
    virtual void onTorque() {}
    virtual void onMass() {}
    virtual void onRadius() {}
    virtual void onPosition() {}
    virtual void onRotation() {}
    virtual void onAttachmentAdd(const Attachment&) {}
    virtual void onAttachmentDel(const Attachment&) {}
};

}
