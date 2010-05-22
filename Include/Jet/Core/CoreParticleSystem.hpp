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
#pragma once

#include <Jet/Core/CoreTypes.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Resources/Texture.hpp>
#include <Jet/Resources/Shader.hpp>
#include <Jet/Scene/ParticleSystem.hpp>
#include <Jet/Types/Particle.hpp>

namespace Jet {

//! Class for generating particles effects, like smoke, rain, fire and dust.
//! @class ParticleSystem
//! Generates particle effects.
class CoreParticleSystem : public ParticleSystem {
public:
    inline CoreParticleSystem(CoreEngine* engine, CoreNode* parent) :
        engine_(engine),
        parent_(parent),
		life_(0.0f),
		type_(ET_POINT),
        accumulator_(0.0f) {

        shader("Particle");
    }
    
    //! Destructor.
    virtual ~CoreParticleSystem() {}
    
    //! Returns the parent node.
    inline CoreNode* parent() const {
        return parent_;
    }
    
    //! Returns the life of this particle system.
    inline float life() const {
        return life_;
    }
    
    //! Returns the width this particle system.  This describes the range of
    //! x-values that particles will be created at.
    inline const Range& width() const {
        return width_;
    }
    
    //! Returns the height of this particle system.  This describes the rnage
    //! of y-values that particles will be created at.
    inline const Range& height() const {
        return height_;
    }
    
    //! Returns the depth of this particle system.  This describes the range of
    //! z-values that particles will be created at.
    inline const Range& depth() const {
        return depth_;
    }

	//! Particle growth rate
	inline const Range& particle_growth_rate() const {
		return particle_growth_rate_;
	}

	//! Returns the particle life.
	inline const Range& particle_life() const {
		return particle_life_;
	}

	//! Returns the range in particle sizes.
	inline const Range& particle_size() const {
		return particle_size_;
	}
    
    //! Returns the speed particles will be created with.
    inline const Range& emission_speed() const {
        return emission_speed_;
    }
    
    //! Returns the direction the particle system is facing.
    inline const Vector& emission_direction() const {
        return emission_direction_;
    }
    
    //! Sets the angle of distribution in radians.
    inline const Range& emission_angle() const {
        return emission_angle_;
    }
    
    //! Sets the emission rate
    inline const Range& emission_rate() const {
        return emission_rate_;
    }
    
    //! Returns the particle system distribution type.  This can either be
    //! cubic or spherical.
    inline EmitterType type() const {
        return type_;
    }
    
    //! Returns the maximum number of particles that can be active in the
    //! system at one time
    inline size_t quota() const {
        return particle_.size();
    }
    
    //! Returns the texture in use for this particle system.
    inline Texture* texture() const {
        return texture_.get();
    }
    
    //! Returns the shader in user for this particle system.
    inline Shader* shader() const {
        return shader_.get();
    }
    
    //! Sets the life of this particle system.
    //! @param life the min and max life of the particle system.
    inline void life(float life) {
        life_ = life;
    }
    
    //! Sets the range of x-values that particles will be created at.
    //! @param width the range of x-values
    inline void width(const Range& width) {
        width_ = width;
    }
    
    //! Sets the range of y-values that particles will be created at.
    //! @param height the range of y values
    inline void height(const Range& height) {
        height_ = height;
    }
    
    //! Sets the range z-values that particles will be created at.
    //! @param depth the range of z-values
    inline void depth(const Range& depth) {
        depth_ = depth;
    }

	//! Sets the particle growth rate.
	inline void particle_growth_rate(const Range& rate) {
		particle_growth_rate_ = rate;
	}

	//! Sets the particle life.
	//! @param life the range in particle lifetime
	inline void particle_life(const Range& life) {
		particle_life_ = life;
	}

	//! Sets the range in particle size.
	//! @param size the range in particle sizes
	inline void particle_size(const Range& size) {
		particle_size_ = size;
	}
    
    //! Sets the speed of created particles.
    //! @param speed the speed range
    inline void emission_speed(const Range& speed) {
        emission_speed_ = speed;
    }
    
    //! Sets the direction of the system.
    //! @param direction the direction
    inline void emission_direction(const Vector& direction) {
        emission_direction_ = direction;
    }
    
    //! Sets the angle of the distribution in radians.
    inline void emission_angle(const Range& angle) {
        emission_angle_ = angle;
    }
    
    //! Sets the emission rate
    inline void emission_rate(const Range& rate) {
        emission_rate_ = rate;
    }
    
    //! Sets the particle system type.
    inline void type(EmitterType type) {
        type_ = type;
    }
    
    //! Sets the maximum number of particles that can be active in the system
    //! at one time
    inline void quota(size_t quota) {
        particle_.resize(quota);
        dead_particle_.clear();
        
        dead_particle_.reserve(quota);
        alive_particle_.reserve(quota);
        
        for (size_t i = 0; i < particle_.size(); i++) {
            dead_particle_.push_back(&particle_[i]);
        }
        
    }
    
    //! Sets the texture in use for his particle system by name.
    //! @param name the name of the texture
    inline void texture(const std::string& name) {
        texture(engine_->texture(name));
    }

    //! Sets the texture.
    //! @param texture a pointer to the texture object
    inline void texture(Texture* texture) {
        texture_ = texture;
    }
    
    //! Sets the shader in user for this particle system by name.
    //! @param name the name of the shader
    inline void shader(const std::string& name) {
        shader(engine_->shader(name));
    }
    
    //! Sets the shader.
    //! @param shader a pointer to the shader object.
    inline void shader(Shader* shader) {
        shader_ = shader;
        if (!shader_) {
            // If the shader variable is null, switch to the default
            // shader variable
            CoreParticleSystem::shader("Particle");
        }
    }
    
    //! Returns a list of alive particles
    Iterator<Particle*> alive_particles() {
        return Iterator<Particle*>(alive_particle_.begin(), alive_particle_.end());
    }
    
    //! Renders this particle system using the given particle buffer
    void update();
    
private:    
    void init_particle_box(Particle& p);
    void init_particle_ellipsoid(Particle& p);
    void init_particle_point(Particle& p);
    
    CoreEngine* engine_;
    CoreNode* parent_;
    Matrix prev_matrix_;
    float life_;
    Range width_;
    Range height_;
    Range depth_;
	Range particle_growth_rate_;
	Range particle_life_;
	Range particle_size_;
    Range emission_speed_;
    Vector emission_direction_;
    Range emission_rate_;
    EmitterType type_;
    Range emission_angle_;
    TexturePtr texture_;
    ShaderPtr shader_;
    std::vector<Particle> particle_;
    std::vector<Particle*> dead_particle_;
    std::vector<Particle*> alive_particle_;
    float accumulator_;  
};

}
