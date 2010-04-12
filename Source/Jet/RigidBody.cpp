/*
 * Copyright (c) 2010 Matt Fichman
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

#include <Jet/RigidBody.hpp>

using namespace Jet;

RigidBody::RigidBody(Engine* engine, Node* parent) :
    engine_(engine),
    parent_(parent),
    listener_(0) {
}

void RigidBody::linear_velocity(const Vector& v) {
    if (v != linear_velocity_) {
        linear_velocity_ = v;
        if (listener_) {
            listener_->on_linear_velocity();
        }
    }
}

void RigidBody::angular_velocity(const Vector& v) {
    if (v != angular_velocity_) {
        angular_velocity_ = v;
        if (listener_) {
            listener_->on_angular_velocity();
        }
    }
}

void RigidBody::apply_force(const Vector& v) {
    force_ += v;
    if (listener_) {
        listener_->on_force();
    }
}

void RigidBody::apply_torque(const Vector& v) {
    torque_ += v;
    if (listener_) {
        listener_->on_torque();
    }
}

void RigidBody::apply_local_force(const Vector& v) {

}

void RigidBody::apply_local_torque(const Vector& v) {
    
}