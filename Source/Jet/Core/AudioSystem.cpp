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

#include <Jet/Core/AudioSystem.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Matrix.hpp>
#include <fmodex/fmod_errors.h>

using namespace Jet;
using namespace std;

inline void fmod_check(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        throw std::runtime_error(FMOD_ErrorString(result));
    }
}

Core::AudioSystem::AudioSystem(Engine* engine) :
    engine_(engine),
    system_(0) {
        
    engine_->listener(this);
    
    FMOD_SPEAKERMODE mode;
    FMOD_CAPS caps;
    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
    
    fmod_check(FMOD_System_Create(&system_));
    fmod_check(FMOD_System_GetDriverCaps(system_, 0, &caps, 0, 0, &mode));
    fmod_check(FMOD_System_SetSpeakerMode(system_, mode));
    fmod_check(FMOD_System_Init(system_, 32, FMOD_INIT_NORMAL, 0));
    fmod_check(FMOD_System_Set3DListenerAttributes(system_, 0, &pos, &vel, &forward, &up));
    fmod_check(FMOD_System_Set3DSettings(system_, 1.0f, 1.0f, 0.05f));
}

Core::AudioSystem::~AudioSystem() {
    if (system_) {
        fmod_check(FMOD_System_Close(system_));
        fmod_check(FMOD_System_Release(system_));
    }
}

void Core::AudioSystem::on_update() {
    if (engine_->camera()) {
        Node* node = static_cast<Node*>(engine_->camera()->parent());
        
        Matrix matrix = node->matrix();
        Vector up = matrix.up();
        Vector forward = matrix.forward();
        Vector origin = matrix.origin();
        Vector velocity;
        if (node->rigid_body_) {
            velocity = node->rigid_body()->linear_velocity();
        }
        FMOD_VECTOR* u = (FMOD_VECTOR*)&up;
        FMOD_VECTOR* f = (FMOD_VECTOR*)&forward;
        FMOD_VECTOR* v = (FMOD_VECTOR*)&velocity;
        FMOD_VECTOR* o = (FMOD_VECTOR*)&origin;
        
        fmod_check(FMOD_System_Set3DListenerAttributes(system_, 0, o, v, f, u));
        fmod_check(FMOD_System_Update(system_));
    }
}