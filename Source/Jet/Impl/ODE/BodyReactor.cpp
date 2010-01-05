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
#include <Jet/Impl/ODE/RootReactor.hpp>
#include <Jet/Impl/ODE/Types.hpp>

using namespace Jet;
using namespace Jet::Impl::ODE;

//------------------------------------------------------------------------------
BodyReactor::BodyReactor(Body::Ptr a, RootReactor* e) :
    id_(dBodyCreate(e->world())),
    body_(a),
    root_(e->root()) {

    dMass mass;
    dMassSetSphereTotal(&mass, body_->mass(), 1.0f);
    dBodySetMass(id_, &mass);
    dBodySetData(id_, this);
    dBodySetMovedCallback(id_, &BodyReactor::onBodyMoved);
    
    body_->publisher().observerAdd(this);
}

//------------------------------------------------------------------------------
BodyReactor::~BodyReactor() {
    dBodyDestroy(id_);
    
    body_->publisher().observerDel(this);
}

//------------------------------------------------------------------------------
void
BodyReactor::onBodyMoved(dBodyID id) {
    BodyReactor::Ptr reactor = static_cast<BodyReactor*>(dBodyGetData(id));
    Body::Ptr body = reactor->body_;
   
    
    const Step& step = reactor->root_->step();
    if (Step::typeRender == step.type_) {
        body->positionUpdate(toVector(dBodyGetPosition(id))); // interp
        body->rotationUpdate(toQuaternion(dBodyGetQuaternion(id)));
        body->linearVelocityUpdate(toVector(dBodyGetLinearVel(id)));
        body->angularVelocityUpdate(toVector(dBodyGetAngularVel(id)));
        
       
        for (Body::AttachmentItr i = body->attachmentItr(); i; i++) {
            dGeomID geom = static_cast<dGeomID>(i->object_->geometry());
            if (geom) {

                i->object_->position(toVector(dGeomGetPosition(geom))); // interp

                dQuaternion temp;
                dGeomGetQuaternion(geom, temp);
                i->object_->rotation(toQuaternion(temp));
            } else {
                // Matrix multiply!?
            }
        }
    }
    
    // Do remainder calculations here!  Interpolate the position and rotation
    // based on the time remainder.  Only do it if this is the final 
    // simulation step before control is passed to the renderer.
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
    dBodySetRotation(id_, fromQuaternion(body_->rotation()));
}

//------------------------------------------------------------------------------
void 
BodyReactor::onAttachmentAdd(const Attachment& a) {
    dGeomID geom_ = (dGeomID)a.object_->geometry();
    if (geom_) {
        dGeomSetBody(geom_, id_);
        dGeomSetOffsetPosition(geom_, a.position_.x(), a.position_.y(), a.position_.z());
        dGeomSetOffsetQuaternion(geom_, fromQuaternion(a.rotation_));
    }
}

//------------------------------------------------------------------------------
void 
BodyReactor::onAttachmentDel(const Attachment& a) {
    dGeomID geom_ = static_cast<dGeomID>(a.object_->geometry());
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
