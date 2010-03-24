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

#include <Jet/ParticleSystem.hpp>
#include <Jet/Engine/Types.hpp>
#include <Jet/Object.hpp>
#include <string>

namespace Jet { namespace Engine {

class ParticleSystem : public Jet::ParticleSystem {
public:

    ParticleSystem();
    virtual ~ParticleSystem();
    virtual SceneNode* parent();
    virtual Material* material() const;
    virtual EmitterType emitter_type() const;
    virtual CoordSystem coord_system() const;
    virtual size_t quota() const;
    virtual real_t life() const;
    virtual real_t drag() const;
    virtual real_t angle();
    virtual const Vector& acceleration() const;
    virtual const Vector& direction() const;
    virtual range_t particle_life() const;
    virtual range_t velocity() const;
    virtual range_t rotation() const;
    virtual range_t rotation_rate() const;
    virtual range_t scale() const;
    virtual range_t scale_rate() const;
    virtual range_t emitter_depth() const;
    virtual range_t emitter_width() const;
    virtual range_t emitter_height() const;
    virtual void material(const std::string& s);
    virtual void quota(size_t i);
    virtual void life(real_t range);

protected:
    virtual ParticleSystem* clone();

    Engine* engine_;
    SceneNode* parent_;
    Material* material_;
    EmitterType emitter_type_;
    CoordSystem coord_system_;
    size_t quota_;
    real_t life_;
    real_t drag_;
    real_t angle_;
    Vector acceleration_;
    Vector direction_;
    range_t particle_life_;
    range_t velocity_;
    range_t rotation_;
    range_t rotation_rate_;
    range_t scale_;
    range_t scale_rate;
    range_t emitter_depth_;
    range_t emitter_width_;
    range_t emitter_height_;
};

}
