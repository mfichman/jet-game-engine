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

#include <Jet/Impl/Ode/RootReactor.hpp>
#include <Jet/Impl/Ode/ModelReactor.hpp>

using namespace Jet;
using namespace Jet::Impl::Ode;

//------------------------------------------------------------------------------
RootReactor::RootReactor(Root::Ptr e) :
    root_(e),
    world_(dWorldCreate()),
    space_(dSimpleSpaceCreate(0)),
    joints_(dJointGroupCreate(1024)) {

}

//------------------------------------------------------------------------------
RootReactor::~RootReactor() {
    reactors_.clear();
    actorReactor_.clear();
    dJointGroupDestroy(joints_);
    dSpaceDestroy(space_);
    dWorldDestroy(world_);
}
 
//------------------------------------------------------------------------------
void
RootReactor::onActorNew(Actor::Ptr a) {
}

//------------------------------------------------------------------------------   
void
RootReactor::onModelNew(Model::Ptr m) {
    reactors_.push_back(new ModelReactor(m, this));
}

//------------------------------------------------------------------------------
void 
RootReactor::onEntityNew(Entity::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onCameraNew(Camera::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onQuadNew(Quad::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onListenerNew(Listener::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onCloudNew(Listener::Ptr) {

}
