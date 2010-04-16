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
#include <Jet/ParticleSystem.hpp>

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
    inline const Range& speed() const {
        return speed_;
    }
    
    //! Returns the particle system distribution type.  This can either be
    //! cubic or spherical.
    inline ParticleSystemType type() const {
        return type_;
    }
    
    //! Returns the direction the particle system is facing.
    inline const Vector& direction() const {
        return direction_;
    }
    
    //! Sets the angle of distribution in radians.
    inline const Range& angle() const {
        return angle_;
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
    inline void speed(const Range& speed) {
        speed_ = speed;
    }
    
    //! Sets the particle system type.
    inline void type(ParticleSystemType type) {
        type_ = type;
    }
    
    //! Sets the direction of the system.
    //! @param direction the direction
    inline void direction(const Vector& direction) {
        direction_ = direction;
    }
    
    //! Sets the angle of the distribution in radians.
    inline void angle(const Range& angle) {
        angle_ = angle;
    }
    
    
private:
    inline ParticleSystem(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent) {
            
    }
    
    Engine* engine_;
    Node* parent_;
    Range life_;
    Range width_;
    Range height_;
    Range depth_;
    Range speed_;
    ParticleSystemType type_;
    Vector direction_;
    Range angle_;
    
    friend class Node;
    
};

}}