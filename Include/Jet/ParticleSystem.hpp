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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Range.hpp>
#include <Jet/Vector.hpp>

namespace Jet {

//! Class for generating particles effects, like smoke, rain, fire and dust.
//! @class ParticleSystem
//! Generates particle effects.
class ParticleSystem : public Object {
public:    
    //! Returns the parent node.
    virtual Node* parent() const=0;
    
    //! Returns the life of this particle system.
    virtual real_t life() const=0;
    
    //! Returns the width this particle system.  This describes the range of
    //! x-values that particles will be created at.
    virtual const Range& width() const=0;
    
    //! Returns the height of this particle system.  This describes the rnage
    //! of y-values that particles will be created at.
    virtual const Range& height() const=0;
    
    //! Returns the depth of this particle system.  This describes the range of
    //! z-values that particles will be created at.
    virtual const Range& depth() const=0;

	//! Returns the particle growth rate.
	virtual const Range& particle_growth_rate() const=0;

	//! Returns the particle life.
	virtual const Range& particle_life() const=0;

	//! Returns the range in particle sizes.
	virtual const Range& particle_size() const=0;
    
    //! Returns the speed particles will be created with.
    virtual const Range& emission_speed() const=0;
    
    //! Returns the direction the particle system is facing.
    virtual const Vector& emission_direction() const=0;
    
    //! Sets the angle of distribution in radians.
    virtual const Range& emission_angle() const=0;
    
    //! Returns the emission rate of the particle system in particles/second.
    virtual const Range& emission_rate() const=0;
    
    //! Returns the particle system distribution type.  This can either be
    //! cubic or spherical.
    virtual EmitterType type() const=0;
    
    //! Returns the maximum number of particles that can be active in the
    //! system at one time
    virtual size_t quota() const=0;

    //! Returns the texture in use for this particle system.
    virtual Texture* texture() const=0;
    
    //! Returns the shader in user for this particle system.
    virtual Shader* shader() const=0;
    
    //! Sets the life of this particle system.
    //! @param life the min and max life of the particle system.
    virtual void life(real_t life)=0;
    
    //! Sets the range of x-values that particles will be created at.
    //! @param width the range of x-values
    virtual void width(const Range& width)=0;
    
    //! Sets the range of y-values that particles will be created at.
    //! @param height the range of y values
    virtual void height(const Range& height)=0;
    
    //! Sets the range z-values that particles will be created at.
    //! @param depth the range of z-values
    virtual void depth(const Range& depth)=0;

	//! Sets the particle growth rate.
	//! @param rate the rate in pixels/second
	virtual void particle_growth_rate(const Range& rate)=0;

	//! Sets the particle life.
	//! @param life the range in particle lifetime
	virtual void particle_life(const Range& life)=0;

	//! Sets the range in particle size.
	//! @param size the range in particle sizes
	virtual void particle_size(const Range& size)=0;
    
    //! Sets the speed of created particles.
    //! @param speed the speed range
    virtual void emission_speed(const Range& speed)=0;
    
    //! Sets the direction of the system.
    //! @param direction the direction
    virtual void emission_direction(const Vector& direction)=0;
    
    //! Sets the angle of the distribution in radians.
    virtual void emission_angle(const Range& angle)=0;
    
    //! Sets the emission rate of the particle emitter in particles/second
    virtual void emission_rate(const Range& rate)=0;
    
    //! Sets the particle system type.
    virtual void type(EmitterType type)=0;
    
    //! Sets the maximum number of particles that can be active in the system
    //! at one time
    virtual void quota(size_t quota)=0;
    
    //! Sets the texture in use for his particle system by name.
    //! @param name the name of the texture
    virtual void texture(const std::string& name)=0;
    
    //! Sets the texture.
    //! @param texture a pointer to the texture object
    virtual void texture(Texture* texture)=0;
    
    //! Sets the shader in user for this particle system by name.
    //! @param name the name of the shader
    virtual void shader(const std::string& name)=0;
    
    //! Sets the shader.
    //! @param shader a pointer to the shader object.
    virtual void shader(Shader* shader)=0;
};

}