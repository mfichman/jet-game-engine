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

#include <Jet/Actor.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void
Actor::linearVelocity(const Vector& v) {
    if (linearVelocity_ != v) {
        linearVelocity_ = v;
        publisher_.notify(&Observer::onLinearVelocity);
    }
}

//------------------------------------------------------------------------------
void
Actor::linearVelocityUpdate(const Vector& v) {
    linearVelocity_ = v;
}   

//------------------------------------------------------------------------------
void                
Actor::angularVelocity(const Vector& v) {
    if (angularVelocity_ != v) {
        angularVelocity_ = v;
        publisher_.notify(&Observer::onAngularVelocity);
    }
}

//------------------------------------------------------------------------------
void
Actor::angularVelocityUpdate(const Vector& v) {
    angularVelocity_ = v;
}

//------------------------------------------------------------------------------
void                
Actor::force(const Vector& v) {
    if (force_ != v) {
        force_ = v;
        publisher_.notify(&Observer::onForce);
    }
}

//------------------------------------------------------------------------------
void                
Actor::forceAdd(const Vector& v) {
    force_ += v;
    publisher_.notify(&Observer::onForce);
}

//------------------------------------------------------------------------------
void                
Actor::torque(const Vector& v) {
    if (torque_ != v) {
        torque_ = v;
        publisher_.notify(&Observer::onTorque);
    }
}

//------------------------------------------------------------------------------
void                
Actor::torqueAdd(const Vector& v) {
    torque_ += v;
    publisher_.notify(&Observer::onTorque);
}
