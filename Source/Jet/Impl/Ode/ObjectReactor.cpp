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
 
#include <Jet/Impl/Ode/ObjectReactor.hpp>
#include <Jet/Impl/Ode/EngineReactor.hpp>

using namespace Jet::Impl::Ode;
using namespace Jet::Physics;

//------------------------------------------------------------------------------
ObjectReactor::ObjectReactor(Object::Ptr o, EngineReactor::Ptr e) :
    object_(o),
    engine_(e),    
    body_(dBodyCreate(e->world())),
    geom_(dCreateSphere(e->space(), 10.0f)) {
    
    dMassSetSphereTotal(&mass_, 10.0f, 10.0f);
    
    dBodySetMass(body_, &mass_);
    dBodySetData(body_, this);

    dGeomDisable(geom_);
    dGeomSetData(geom_, this);
    dGeomSetBody(geom_, body_);
    dGeomSetCategoryBits(geom_, 0);
    dGeomSetCollideBits(geom_, 0);
}

//------------------------------------------------------------------------------
ObjectReactor::~ObjectReactor() {
    dGeomDestroy(geom_);
    dBodyDestroy(body_);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onPosition() {
    const Vector& pos = object_->position();
    dGeomSetPosition(geom_, pos[0], pos[1], pos[2]);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onRotation() {
    const Quaternion& quat = object_->rotation();
    dGeomSetQuaternion(geom_, &quat[0]);
}

//------------------------------------------------------------------------------
void
ObjectReactor::onLinearVelocity() {
    const Vector& vel = object_->linearVelocity();
    dBodySetLinearVel(body_, vel[0], vel[1], vel[2]);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onAngularVelocity() {
    const Vector& vel = object_->angularVelocity();
    dBodySetAngularVel(body_, vel[0], vel[1], vel[2]);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onForce() {
    const Vector& force = object_->force();
    dBodySetForce(body_, force[0], force[1], force[2]);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onTorque() {
    const Vector& torq = object_->torque();
    dBodySetTorque(body_, torq[0], torq[1], torq[2]);
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onObject() {
    if (parent_) {
    
        // Subract the mass of this object from the parent
        dMass neg = mass_;
        neg.mass = -neg.mass;
        dMass total;
        dBodyGetMass(parent_->body_, &total);
        dMassAdd(&total, &neg);
        dBodySetMass(parent_->body_, &total);
    }
    
    if (object_->parent()) {
    
        // Add the mass of this object to the parent
        parent_ = engine_->reactor(object_->parent());
        dMass total = parent_->mass_;
        dMassAdd(&total, &mass_);
        dBodySetMass(parent_->body_, &total);
        dBodyDisable(body_);
        
        dGeomSetBody(geom_, parent->body_);
    }
}

//------------------------------------------------------------------------------
void 
ObjectReactor::onDynamic() {
    if (object_->dynamic()) {
        const dReal* pos = dGeomGetPosition(geom_);
        dQuaternion quat = {0};
        dGeomGetQuaternion(geom_, quat);
        dBodySetPosition(body_, pos[0], pos[1], pos[2]);
        dBodySetQuaternion(body_, quat);
        dGeomSetBody(geom_, body_);
        dBodyEnable(body_);
    } else {
        dGeomSetBody(geom_, 0);
        dBodyDisable(body_);
    }
}
