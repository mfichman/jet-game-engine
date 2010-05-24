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

#include <Jet/Core/CoreParticleSystem.hpp>
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

void CoreParticleSystem::update() {    
    
    // Add particles that are alive to the particle buffer, and
    // add dead particles to the free list
	for (size_t i = alive_particle_.size(); i > 0; i--) {
        Particle* p = alive_particle_[i-1];
        if ((engine_->frame_time() - p->init_time) > p->life || p->life <= 0.0f) {
			if (i != alive_particle_.size()) {
				swap(alive_particle_[i-1], alive_particle_[alive_particle_.size()-1]);
			}
			alive_particle_.pop_back();
			dead_particle_.push_back(p);
		}
    }
	
	if (life_ <= 0.0f && life_ > -1.0f) {
		return;
	}
        
    if (life_ > -1.0f) {
		life_ = max(0.0f, life_ - engine_->frame_delta());
	}
    
    if (frame_id_ != engine_->frame_id() || !accumulator_) {
        // If the frame id is different from the engine ID, then we must've
        // skipped a frame (probably because the parent wasn't visible)
        // so reset the particle emitter
        accumulator_ = engine_->frame_time();
        prev_matrix_ = parent_->matrix();
        frame_id_ = engine_->frame_id();
    }

    float init = accumulator_;
    
    const Vector& old_position = prev_matrix_.origin();
    const Vector& new_position = parent_->matrix().origin();
    const Quaternion& old_rotation = prev_matrix_.rotation();
    const Quaternion& new_rotation = parent_->matrix().rotation();
 
    // Spawn additional particle
    while (accumulator_ < engine_->frame_time()) {
        
        if (dead_particle_.empty()) {
            accumulator_ = engine_->frame_delta();
            break;
        }
        
        // Initialize the particle to the current time and
        // set up the randomized parameters
        Particle* p = dead_particle_.back();
        dead_particle_.pop_back();
		alive_particle_.push_back(p);
        p->init_time = accumulator_;
		p->init_size = rand_range(particle_size_);
		p->init_rotation = rand_range(Range(0.0, PI));
		p->life = rand_range(particle_life_);
		p->growth_rate = rand_range(particle_growth_rate_);
        
        // Set up initial parameters
        if (ET_BOX == type_) {
            init_particle_box(*p);
            
        } else if (ET_ELLIPSOID == type_) {
            init_particle_ellipsoid(*p);

        } else if (ET_POINT == type_) {
            init_particle_point(*p);
        }
        
        // Ratio of etween-frame time to the total time difference
        // between frames
        float alpha = (accumulator_ - init)/(engine_->frame_time() - init);
    
        // Interpolate position and rotation between this frame and
        // the last frame renderered
        Vector position = old_position.lerp(new_position, alpha);
        Quaternion rotation = old_rotation.slerp(new_rotation, alpha);

        
        // Rotate the velocity vector by the node's rotation
        p->init_velocity = rotation * p->init_velocity;
        if (inherit_velocity_) {
            p->init_velocity = parent_->linear_velocity() + p->init_velocity;
        }
        
        // Rotate the start position by the node's rotation, and then
        // translate it to the node's position
        p->init_position = rotation * p->init_position;
        p->init_position = position + p->init_position;
        
        // Increment time to the next particle emission
        accumulator_ += 1.0f/rand_range(emission_rate_);
    }
    
    prev_matrix_ = parent_->matrix();
    frame_id_++;
}
    
void CoreParticleSystem::init_particle_box(Particle& p) {
    float w = rand_range2(Range(0, 1));
    float h = rand_range2(Range(0, 1));
    float d = rand_range2(Range(0, 1));
    p.init_position.x = w * (width_.end - width_.begin);
    p.init_position.y = h * (height_.end - height_.begin);
    p.init_position.z = d * (depth_.end - depth_.begin);
    
    float speed = rand_range(emission_speed_);
    p.init_velocity = Vector(w, h, d).unit() * speed;
}

void CoreParticleSystem::init_particle_ellipsoid(Particle& p) {
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

void CoreParticleSystem::init_particle_point(Particle& p) {
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
