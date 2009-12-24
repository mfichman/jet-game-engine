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

#include <Jet/Impl/Ode/ModelReactor.hpp>

using namespace Jet;
using namespace Jet::Impl::Ode;    

//------------------------------------------------------------------------------
ModelReactor::ModelReactor(Model::Ptr m, RootReactor::Ptr e) : 
    rootReactor_(e),
    model_(m),
    geom_(dCreateSphere(e->space(), 10.0f)) {
    
    model_->publisher().observerAdd(this);
    model_->object()->publisher().observerAdd(this);
    model_->anchor()->publisher().observerAdd(this);
    model_->collidable()->publisher().observerAdd(this);
    dGeomDisable(geom_);
}

//------------------------------------------------------------------------------
ModelReactor::~ModelReactor() {
    model_->publisher().observerDel(this);
    model_->object()->publisher().observerDel(this);
    model_->anchor()->publisher().observerDel(this);
    model_->collidable()->publisher().observerDel(this);
    dGeomDestroy(geom_);
}

//------------------------------------------------------------------------------
void 
ModelReactor::onMesh() {
    dGeomSphereSetRadius(geom_, model_->mesh()->boundingSphere().radius_);   
    if (Collidable::enabled == model_->collidable()->state()) {
        dGeomEnable(geom_);
    }
}

//------------------------------------------------------------------------------
void
ModelReactor::onPosition() {
    const Vector& v = model_->object()->position();
    dGeomSetOffsetPosition(geom_, v[0], v[1], v[2]);
}

//------------------------------------------------------------------------------
void
ModelReactor::onRotation() {
    const Quaternion& q = model_->object()->rotation();
    dGeomSetOffsetQuaternion(geom_, (dReal*)&q);
}

//------------------------------------------------------------------------------
void
ModelReactor::onParent() {
    ActorReactor::Ptr reactor = rootReactor_->actorReactor(model_->anchor()->parent());
    if (reactor) {
        dGeomSetBody(geom_, reactor->body());
    }
}

//------------------------------------------------------------------------------
void
ModelReactor::onState() {
    Collidable::State s = model_->collidable()->state();
    if (Collidable::enabled == s) {
        dGeomEnable(geom_);    
    } else if (Collidable::disabled == s) {
        dGeomDisable(geom_);
    }   
}

