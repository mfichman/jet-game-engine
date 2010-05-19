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
using namespace std;

#define PI 3.14159f

static inline float rand_range(const Range& range) {
    float r = (float)rand()/(float)RAND_MAX;
    return (range.end - range.begin) * r + range.begin;
}

static inline float rand_range2(const Range& range) {
    float r = (float)rand()/(float)RAND_MAX;
    if (r < 0.5) {
        return rand_range(Range(range.begin, range.end));
    } else {
        return rand_range(Range(-range.end, -range.begin));
    }
}

void Core::ParticleSystem::render(Core::ParticleBuffer* buffer) {
    dead_particle_.clear();
    
    // Set the buffer's shader and pointer.  This may cause the
    // buffer to flush, but that's what we want.
	buffer->shader(shader_.get());
    buffer->texture(texture_.get());
    
    
    // Add particles that are alive to the particle buffer, and
    // add dead particles to the free list
    for (size_t i = 0; i < particle_.size(); i++) {
        Particle& p = particle_[i];
        if ((engine_->frame_time() - p.init_time) > p.life || p.life <= 0.0f) {
            dead_particle_.push_back(&p);
        } else {
            buffer->particle(p);
        }
    }
    
	if (life_ <= 0.0f && life_ > -1.0f) {
		return;
	}
    
        
    if (life_ > -1.0f) {
		life_ = max(0.0f, life_ - engine_->frame_delta());
	}

	accumulator_ += engine_->frame_delta();
 
    // Spawn additional particles
    while (accumulator_ > next_emission_ && !dead_particle_.empty()) {
        Particle& p = *dead_particle_.back();
        dead_particle_.pop_back();
        p.init_time = engine_->frame_time();
		p.init_size = rand_range(particle_size_);
		p.init_rotation = rand_range(Range(0.0, PI));
		p.life = rand_range(particle_life_);
		p.growth_rate = rand_range(particle_growth_rate_);
        
        // Set up initial parameters
        if (BOX_EMITTER == type_) {
            init_particle_box(p);
            
        } else if (ELLIPSOID_EMITTER == type_) {
            init_particle_ellipsoid(p);

        } else if (POINT_EMITTER == type_) {
            init_particle_point(p);
        }
        p.init_position = parent_->matrix() * p.init_position;
        p.init_velocity = parent_->matrix().rotate(p.init_velocity);
		p.init_velocity += parent_->linear_velocity();
        
        accumulator_ -= next_emission_;
		next_emission_ = 1.0f/rand_range(emission_rate_);
    }
}
    
void Core::ParticleSystem::init_particle_box(Particle& p) {
    float w = rand_range2(Range(0, 1));
    float h = rand_range2(Range(0, 1));
    float d = rand_range2(Range(0, 1));
    p.init_position.x = w * (width_.end - width_.begin);
    p.init_position.y = h * (height_.end - height_.begin);
    p.init_position.z = d * (depth_.end - depth_.begin);
    
    float speed = rand_range(emission_speed_);
    p.init_velocity = Vector(w, h, d).unit() * speed;
}

void Core::ParticleSystem::init_particle_ellipsoid(Particle& p) {
    float phi = rand_range(Range(0.0f, PI));
    float theta = rand_range(Range(0.0f, 2.0f*PI));
    float a = rand_range(width_);
    float b = rand_range(height_);
    float c = rand_range(depth_);
    p.init_position.x = a * sinf(phi) * cosf(theta);
    p.init_position.y = b * sinf(phi) * sinf(theta);
    p.init_position.z = c * cosf(phi);
    
    float speed = rand_range(emission_speed_);
    p.init_velocity.x = speed * sinf(phi) * cosf(theta);
    p.init_velocity.y = speed * sinf(phi) * sinf(theta);
    p.init_velocity.z = speed * cosf(phi);
}

void Core::ParticleSystem::init_particle_point(Particle& p) {
    p.init_position = Vector(0.0f, 0.0f, 0.0f);
    
    Vector forward = emission_direction_;
    Vector up = forward.orthogonal();
    Vector right = forward.cross(up);
    
    float speed = rand_range(emission_speed_);
    float beta = PI / 180 * rand_range(emission_angle_);
    float theta = PI / 180 * rand_range(emission_angle_);
    Vector side = up * sinf(theta) + right * sinf(beta);
    Vector direction = side * cosf(theta) + forward;
    p.init_velocity = direction.unit() * speed;
}
