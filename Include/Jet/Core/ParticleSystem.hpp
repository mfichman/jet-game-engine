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

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Shader.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/Particle.hpp>

namespace Jet { namespace Core {

//! Class for generating particles effects, like smoke, rain, fire and dust.
//! @class ParticleSystem
//! Generates particle effects.
class ParticleSystem : public Jet::ParticleSystem {
public:
    
    //! Destructor.
    virtual ~ParticleSystem() {}
    
    //! Returns the parent node.
    inline Jet::Node* parent() const {
        return parent_;
    }
    
    //! Returns the life of this particle system.
    inline const Range& life() const {
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
    inline real_t emission_rate() const {
        return emission_rate_;
    }
    
    //! Returns the particle system distribution type.  This can either be
    //! cubic or spherical.
    inline ParticleSystemType type() const {
        return type_;
    }
    
    //! Returns the maximum number of particles that can be active in the
    //! system at one time
    inline size_t quota() const {
        return quota_;
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
    inline void life(const Range& life) {
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
    inline void emission_rate(real_t rate) {
        emission_rate_ = rate;
    }
    
    //! Sets the particle system type.
    inline void type(ParticleSystemType type) {
        type_ = type;
    }
    
    //! Sets the maximum number of particles that can be active in the system
    //! at one time
    inline void quota(size_t quota) {
        quota_ = quota;
        particle_.resize(quota);
        dead_particle_.clear();
    }
    
    //! Sets the texture in use for his particle system by name.
    //! @param name the name of the texture
    inline void texture(const std::string& name) {
        texture(engine_->texture(name));
    }

    //! Sets the texture.
    //! @param texture a pointer to the texture object
    inline void texture(Jet::Texture* texture) {
        texture_ = static_cast<Texture*>(texture);
    }
    
    //! Sets the shader in user for this particle system by name.
    //! @param name the name of the shader
    inline void shader(const std::string& name) {
        shader(engine_->shader(name));
    }
    
    //! Sets the shader.
    //! @param shader a pointer to the shader object.
    inline void shader(Jet::Shader* shader) {
        shader_ = static_cast<Shader*>(shader);
        if (!shader_) {
            // If the shader variable is null, switch to the default
            // shader variable
            ParticleSystem::shader("Particle");
        }
    }
    
    //! Renders this particle system using the given particle buffer
    void render(ParticleBuffer* buffer);
    
private:
    inline ParticleSystem(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent),
        quota_(1000),
        time_(0.0f),
        accumulator_(0.0f) {
            
        shader("Particle");
        particle_.resize(quota_);
    }
    
    Engine* engine_;
    Node* parent_;
    Range life_;
    Range width_;
    Range height_;
    Range depth_;
    Range emission_speed_;
    Vector emission_direction_;
    real_t emission_rate_;
    ParticleSystemType type_;
    Range emission_angle_;
    size_t quota_;
    TexturePtr texture_;
    ShaderPtr shader_;
    std::vector<Particle> particle_;
    std::vector<Particle*> dead_particle_;
    real_t time_;
    real_t accumulator_;
    
    friend class Node;
    
};

}}