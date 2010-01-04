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

#include <Jet/Cloud.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void 
Cloud::particleNew(Particle& o) {
    particle_.push_back(o);
    push_heap(particle_.begin(), particle_.end());
    publisher_.notify<Particle&>(&Observer::onParticleNew, o);
}

//------------------------------------------------------------------------------
void
Cloud::particleDel() { 
    pop_heap(particle_.begin(), particle_.end()); 
}

//------------------------------------------------------------------------------
void
Cloud::particleFn(const string& s) {
    particleFn_ = s;
}

//------------------------------------------------------------------------------
void 
Cloud::texture(const string& t) {
    if (texture_ != t) {
        texture_ = t;
        publisher_.notify(&Observer::onTexture);
    }
}

//------------------------------------------------------------------------------
void 
Cloud::scale(const Vector& s) {
    if (scale_ != s) {
        scale_ = s;
        publisher_.notify(&Observer::onScale);
    }
}

//------------------------------------------------------------------------------
void 
Cloud::shader(const string& s) { 
    if (shader_ != s) {
        shader_ = s;    
        publisher_.notify(&Observer::onShader);
    }
}

//------------------------------------------------------------------------------
void            
Cloud::state(State s) {
    if (s != state_) {
        state_ = s;
        publisher_.notify(&Observer::onState);
    }
}

//------------------------------------------------------------------------------
void            
Cloud::collisionFn(const string& o) {
    if (o != collisionFn_) {
        collisionFn_ = o;
        publisher_.notify(&Observer::onCollisionFn);
    }
}
