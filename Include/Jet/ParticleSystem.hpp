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
    virtual const Range& life() const=0;
    
    //! Returns the width this particle system.  This describes the range of
    //! x-values that particles will be created at.
    virtual const Range& width() const=0;
    
    //! Returns the height of this particle system.  This describes the rnage
    //! of y-values that particles will be created at.
    virtual const Range& height() const=0;
    
    //! Returns the depth of this particle system.  This describes the range of
    //! z-values that particles will be created at.
    virtual const Range& depth() const=0;
    
    //! Returns the speed particles will be created with.
    virtual const Range& speed() const=0;
    
    //! Returns the particle system distribution type.  This can either be
    //! cubic or spherical.
    virtual ParticleSystemType type() const=0;
    
    //! Returns the direction the particle system is facing.
    virtual const Vector& direction() const=0;
    
    //! Sets the angle of distribution in radians.
    virtual const Range& angle() const=0;
    
    //! Sets the life of this particle system.
    //! @param life the min and max life of the particle system.
    virtual void life(const Range& life)=0;
    
    //! Sets the range of x-values that particles will be created at.
    //! @param width the range of x-values
    virtual void width(const Range& width)=0;
    
    //! Sets the range of y-values that particles will be created at.
    //! @param height the range of y values
    virtual void height(const Range& height)=0;
    
    //! Sets the range z-values that particles will be created at.
    //! @param depth the range of z-values
    virtual void depth(const Range& depth)=0;
    
    //! Sets the speed of created particles.
    //! @param speed the speed range
    virtual void speed(const Range& speed)=0;
    
    //! Sets the particle system type.
    virtual void type(ParticleSystemType type)=0;
    
    //! Sets the direction of the system.
    //! @param direction the direction
    virtual void direction(const Vector& direction)=0;
    
    //! Sets the angle of the distribution in radians.
    virtual void angle(const Range& angle)=0;    
};

}