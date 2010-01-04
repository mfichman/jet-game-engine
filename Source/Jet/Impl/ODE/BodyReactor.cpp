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

#include <Jet/Impl/ODE/BodyReactor.hpp>
#include <Jet/Impl/ODE/Types.hpp>

using namespace Jet;
using namespace Jet::Impl::ODE;

//------------------------------------------------------------------------------
BodyReactor::BodyReactor(Body::Ptr a, RootReactor::Ptr e) :
    id_(dBodyCreate(e->world())),
    body_(a) {

    dMass mass;
    dMassSetSphereTotal(&mass, body_->mass(), 1.0f);
    dBodySetMass(id_, &mass);
}

//------------------------------------------------------------------------------
BodyReactor::~BodyReactor() {
    dBodyDestroy(id_);
}

//------------------------------------------------------------------------------
void 
BodyReactor::onLinearVelocity() {
    const Vector& v = body_->linearVelocity();
    dBodySetLinearVel(id_, v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
void 
BodyReactor::onAngularVelocity() {
    const Vector& v = body_->angularVelocity();
    dBodySetAngularVel(id_, v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
void 
BodyReactor::onForce() {
    const Vector& v = body_->force();
    dBodySetForce(id_, v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
void 
BodyReactor::onTorque() {
    const Vector& v = body_->torque();
    dBodySetTorque(id_, v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
void
BodyReactor::onMass() {
    dMass mass;
    dMassSetSphereTotal(&mass, body_->mass(), body_->radius());
    dBodySetMass(id_, &mass);
}

//------------------------------------------------------------------------------
void
BodyReactor::onRadius() {
    dMass mass;
    dMassSetSphereTotal(&mass, body_->mass(), body_->radius());
    dBodySetMass(id_, &mass);
}

//------------------------------------------------------------------------------
void
BodyReactor::onPosition() {
    const Vector& v = body_->position();
    dBodySetPosition(id_, v.x(), v.y(), v.z());
}

//------------------------------------------------------------------------------
void
BodyReactor::onRotation() {
    dBodySetRotation(id_, convert(body_->rotation()));
}

//------------------------------------------------------------------------------
void 
BodyReactor::onAttachmentAdd(const Attachment& a) {
    dGeomID geom_ = (dGeomID)a.object_->geometry();
    if (geom_) {
        dGeomSetBody(geom_, id_);
    }
}

//------------------------------------------------------------------------------
void 
BodyReactor::onAttachmentDel(const Attachment& a) {
    dGeomID geom_ = (dGeomID)a.object_->geometry();
    if (geom_) {
        dGeomSetBody(geom_, 0);
    }
}

/*

//------------------------------------------------------------------------------
void
BodyReactor::attachmentAdd(const AttachmentInfo& n) {
    // Get the total mass
    dMass totalMass;
    dBodyGetMass(id_, &totalMass);

    const Vector& position = n.object_->position();
    const Quaternion& rotation = n.object_->rotation();  
    dMass mass = n.mass_;

    // Get rotation matrix
    dMatrix3 rotationMatrix;
    dRfromQ(rotationMatrix, (dReal*)&rotation);

    // Recalculate mass, and translate for offset
    dMassRotate(&mass, rotationMatrix);
    dMassTranslate(&mass, position[1], position[1], position[2]);

    // Add to the total    
    dMassAdd(&totalMass, &mass);

    // Now reset the id's mass
    dBodySetMass(id_, &totalMass);

    // Add the geom
    dGeomSetBody(n.geom_, id_);
}

//------------------------------------------------------------------------------
void
BodyReactor::attachmentDel(const AttachmentInfo& n) {  
    // Get the total mass
    dMass totalMass;
    dBodyGetMass(id_, &totalMass);
 
    // Subtract out the mass of the model
    dMass mass = n.mass_;
    mass.mass *= -1;
    dMassAdd(&totalMass, &mass);
    mass.mass *= -1;

    // Now reset the id's mass
    dBodySetMass(id_, &totalMass);  

    // Remove the geom
    dGeomSetBody(n.geom_, 0);
}*/
