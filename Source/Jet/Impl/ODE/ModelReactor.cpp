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

#include <Jet/Impl/ODE/ModelReactor.hpp>

using namespace Jet;
using namespace Jet::Impl::ODE;

//------------------------------------------------------------------------------
ModelReactor::ModelReactor(Model::Ptr m, RootReactor::Ptr r) : 
    model_(m), 
    geom_(dCreateSphere(r->space(), 0.0f)) {
    
    model_->publisher().observerAdd(this);
    model_->geometry(static_cast<Handle>(geom_));
    
    dGeomSetData(geom_, static_cast<Object*>(model_.get()));
}

//------------------------------------------------------------------------------
ModelReactor::~ModelReactor() {
    model_->geometry(0);
    dGeomDestroy(geom_);
}

//------------------------------------------------------------------------------
void 
ModelReactor::onMesh() {
    if (model_->mesh()) {
        dGeomSphereSetRadius(geom_, model_->mesh()->boundingSphere().radius_);
    }
    resetMode();
}

//-----------------------------------------------------------------------------
void
ModelReactor::resetMode() {
    if (Model::stateEnabled == model_->state() && model_->mesh()) {
        dGeomEnable(geom_);
    } else {
        dGeomDisable(geom_);
    }
}
