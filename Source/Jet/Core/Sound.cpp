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

#include <Jet/Core/Sound.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/AudioSystem.hpp>
#include <fmodex/fmod_errors.h>

using namespace Jet;
using namespace std;


inline void fmod_check(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        throw std::runtime_error(FMOD_ErrorString(result));
    }
}

Core::Sound::~Sound() {
    state(UNLOADED);
}

void Core::Sound::state(ResourceState state) {
    if (state_ == state) {
        return;
    }
    
    if (UNLOADED == state_) {
        string file = engine_->resource_path(name_);
        
        // Load the sound
        FMOD_SYSTEM* system = engine_->audio_system()->system();
        fmod_check(FMOD_System_CreateSound(system, file.c_str(), FMOD_HARDWARE|FMOD_3D, 0, &sound_));
        fmod_check(FMOD_Sound_Set3DMinMaxDistance(sound_, 2.0f, 1000.0f));
    }
    
    if (UNLOADED == state) {
        fmod_check(FMOD_Sound_Release(sound_));
    }
    
    state_ = state;
}