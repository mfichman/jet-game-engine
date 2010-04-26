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

#include <Jet/Core/ParticleSystem.hpp>
#include <Jet/Core/ParticleBuffer.hpp>
#include <cmath>

using namespace Jet;

#define PI 3.14159f

static inline real_t rand_range(const Range& range) {
    real_t r = (float)rand()/(float)RAND_MAX;
    return (range.end - range.begin) * r + range.begin;
}

static inline real_t rand_range2(const Range& range) {
    real_t r = (float)rand()/(float)RAND_MAX;
    if (r < 0.5) {
        return rand_range(Range(range.begin, range.end));
    } else {
        return rand_range(Range(-range.end, -range.begin));
    }
}

void Core::ParticleSystem::render(Core::ParticleBuffer* buffer) {
    time_ += engine_->delta();
    accumulator_ += engine_->delta();
    dead_particle_.clear();
    
    // Set the buffer's shader and pointer.  This may cause the
    // buffer to flush, but that's what we want.
    buffer->shader(shader_.get());
    buffer->texture(texture_.get());
    
    // Add particles that are alive to the particle buffer, and
    // add dead particles to the free list
    for (size_t i = 0; i < particle_.size(); i++) {
        Particle& p = particle_[i];
        if ((time_ - p.initial_time) > p.life || p.life <= 0.0f) {
            dead_particle_.push_back(&p);
        } else {
            buffer->particle(p);
        }
    }
    
    // Spawn additional particles
    while (accumulator_ > 1.0f/emission_rate_ && !dead_particle_.empty()) {
        Particle& p = *dead_particle_.back();
        dead_particle_.pop_back();
        p.initial_time = time_;
        
        // Set up initial parameters
        if (BOX_EMITTER == type_) {
            real_t w = rand_range2(Range(0, 1));
            real_t h = rand_range2(Range(0, 1));
            real_t d = rand_range2(Range(0, 1));
            p.initial_position.x = w * (width_.end - width_.begin);
            p.initial_position.y = h * (height_.end - height_.begin);
            p.initial_position.z = d * (depth_.end - depth_.begin);
            
            real_t speed = rand_range(emission_speed_);
            p.velocity = Vector(w, h, d).unit() * speed;
            
        } else if (ELLIPSOID_EMITTER == type_) {
            real_t phi = rand_range(Range(0.0f, PI));
            real_t theta = rand_range(Range(0.0f, 2.0f*PI));
            real_t a = rand_range(width_);
            real_t b = rand_range(height_);
            real_t c = rand_range(depth_);
            p.initial_position.x = a * sinf(phi) * cosf(theta);
            p.initial_position.y = b * sinf(phi) * sinf(theta);
            p.initial_position.z = c * cosf(phi);
            
            real_t speed = rand_range(emission_speed_);
            p.velocity.x = speed * sinf(phi) * cosf(theta);
            p.velocity.y = speed * sinf(phi) * sinf(theta);
            p.velocity.z = speed * cosf(phi);
        } else if (POINT_EMITTER == type_) {
            p.initial_position = Vector(0.0f, 0.0f, 0.0f);
            
            Vector forward = emission_direction_.unit();
            Vector up = forward.orthogonal();
            Vector right = forward.cross(up);
            
            real_t speed = rand_range(emission_speed_);
            real_t beta = rand_range(Range(0.0f, 2.0f*PI));
            real_t theta = PI / 180 * rand_range(emission_angle_);
            Vector side = up * cosf(beta) + right * sinf(beta);
            Vector direction = side * cosf(theta) + forward;
            p.velocity = direction.unit() * speed;
        }
        
        accumulator_ -= emission_rate_;
    }
}