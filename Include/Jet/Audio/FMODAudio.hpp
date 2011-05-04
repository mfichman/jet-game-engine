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

#include <Jet/Audio/FMODTypes.hpp>
#include <Jet/Audio/FMODAudioSource.hpp>
#include <Jet/Audio/FMODSound.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Audio.hpp>

namespace Jet {

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class Audio
//! @brief Rigid body physics engine
class FMODAudio : public Audio, public EngineListener {
public:
    FMODAudio(CoreEngine* engine);

    //! Destructor.
    virtual ~FMODAudio();
    
    //! Returns the physics world
    inline FMOD_SYSTEM* system() const {
        return system_;
    }

private:
    inline FMODSound* sound(const std::string& name) {
        return new FMODSound(engine_, name);
    }
    
    inline FMODAudioSource* audio_source(Node* parent) {
        return new FMODAudioSource(engine_, static_cast<CoreNode*>(parent));
    }
    
    void on_tick();
    void on_init();
    void on_update();
    void on_render() {}
    
    CoreEngine* engine_;
    FMOD_SYSTEM* system_;
};

}