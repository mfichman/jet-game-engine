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

#include <Jet/Jet.hpp>
#include <Jet/Object.hpp>
#include <string>

namespace Jet {

//! Class for creating particle effects; generates particles according to the
//! attributes listed below as part of the class.  Individual particles are
//! configured via the particle script callback.
//! @class ParticleSystem
//! @brief Displays particle effects.
class ParticleSystem : public Object {
public:

    //! Destroys the scene component
    virtual ~ParticleSystem() {}
    
    //! Returns the parent node
    virtual SceneNode* parent()=0;

    //! Returns the material used to render particles.
    virtual const std::string& material_name() const=0;
    
    //! Returns the emitter type
    virtual EmitterType emitter_type() const=0;
    
    //! Returns true if world coordinates should be used.
    virtual CoordSystem coord_system() const=0;

    //! Returns the maximum number of particles this system can own.
    virtual size_t quota() const=0;

    //! Returns the life of this particle system, in seconds.  The particle
    //! system will spawn particles until this amount of time has elapsed.
    virtual real_t life() const=0;
    
    //! Particle drag
    virtual real_t drag() const=0;
    
    //! Angle of particle emission velocity
    virtual real_t angle()=0;
    
    //! Range of acceleration of particles in meters per second
    virtual const Vector& acceleration() const=0;
    
    //! Direction of emission
    virtual const Vector& direction() const=0;
    
    //! Returns the range of particle TTLs
    virtual range_t particle_life() const=0;
    
    //! Range of velocity of particles in meters per second
    virtual range_t velocity() const=0;
    
    //! Rotation rate of particles in radians per second.
    virtual range_t rotation() const=0;
    
    //! Rotation speed range of particles
    virtual range_t rotation_rate() const=0;
    
    //! Scale of particles
    virtual range_t scale() const=0;
    
    //! Scaling rate of particles
    virtual range_t scale_rate() const=0;
    
    //! Depth of zone for particle creation (z-axis).
    virtual range_t emitter_depth() const=0;
    
    //! Width of zone for particle creation (x-axis).
    virtual range_t emitter_width() const=0;
    
    //! Height of zone for particle creation (y-axis).
    virtual range_t emitter_height() const=0;

    //! Sets the material used to render particles.  See the documentation
    //! about materials for more information.
    //! @param s the name of the material
    virtual void material_name(const std::string& s)=0;

    //! Sets the maximum number of particles this system can own.
    //! @param i the maximum number of particles
    virtual void quota(size_t i)=0;

    //! Sets the life of this particle system, in seconds.  The particle system
    //! will spawn particles until this amount of time has elapsed.  Calling
    //! this function after the particle system is dead will restart the 
    //! particle system with the new life.  A negative value means the particle
    //! system will run forever.
    //! @param range of new life of the particle system
    virtual void life(real_t range)=0;
};

}
