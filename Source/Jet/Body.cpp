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

#include <Jet/Body.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void
Body::linearVelocity(const Vector& v) {
    if (linearVelocity_ != v) {
        linearVelocity_ = v;
        publisher_.notify(&Observer::onLinearVelocity);
    }
}

//------------------------------------------------------------------------------
void
Body::linearVelocityUpdate(const Vector& v) {
    linearVelocity_ = v;
}   

//------------------------------------------------------------------------------
void                
Body::angularVelocity(const Vector& v) {
    if (angularVelocity_ != v) {
        angularVelocity_ = v;
        publisher_.notify(&Observer::onAngularVelocity);
    }
}

//------------------------------------------------------------------------------
void
Body::angularVelocityUpdate(const Vector& v) {
    angularVelocity_ = v;
}

//------------------------------------------------------------------------------
void                
Body::force(const Vector& v) {
    if (force_ != v) {
        force_ = v;
        publisher_.notify(&Observer::onForce);
    }
}

//------------------------------------------------------------------------------
void                
Body::forceAdd(const Vector& v) {
    force_ += v;
    publisher_.notify(&Observer::onForce);
}

//------------------------------------------------------------------------------
void                
Body::torque(const Vector& v) {
    if (torque_ != v) {
        torque_ = v;
        publisher_.notify(&Observer::onTorque);
    }
}

//------------------------------------------------------------------------------
void                
Body::torqueAdd(const Vector& v) {
    torque_ += v;
    publisher_.notify(&Observer::onTorque);
}

//------------------------------------------------------------------------------
void
Body::mass(Real f) {
    if (f != mass_) {
        mass_ = f;
        publisher_.notify(&Observer::onMass);
    }
}

//------------------------------------------------------------------------------
void
Body::radius(Real f) {
    if (f != radius_) {
        radius_ = f;
        publisher_.notify(&Observer::onRadius);
    }
}

//------------------------------------------------------------------------------
void
Body::position(const Vector& v) {
    if (position_ != v) {
        position_ = v;
        publisher_.notify(&Observer::onPosition);
    }
}

//------------------------------------------------------------------------------
void
Body::positionUpdate(const Vector& v) {
    position_ = v;
}

//------------------------------------------------------------------------------
void
Body::rotation(const Quaternion& q) {
    if (rotation_ != q) {
        rotation_ = q;
        publisher_.notify(&Observer::onRotation);
    }
}

//------------------------------------------------------------------------------
void
Body::rotationUpdate(const Quaternion& q) {
    rotation_ = q;
}
