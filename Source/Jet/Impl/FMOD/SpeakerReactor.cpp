/*
 * Copyright (c) 2009 Matt Fichman
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

#include <Jet/Impl/FMOD/SpeakerReactor.hpp>
#include <stdexcept>
#include <fmodex/fmod_errors.h>

using namespace Jet;
using namespace Jet::Impl::FMOD;

//------------------------------------------------------------------------------
#define CHECK(x) {															\
	FMOD_RESULT result = x;													\
	if (result != FMOD_OK) {												\        logic_error(FMOD_ErrorString(result));								\
	}																		\
}
//------------------------------------------------------------------------------
SpeakerReactor::SpeakerReactor(Speaker::Ptr s, RootReactor::Ptr r) :
    rootReactor_(r),
    speaker_(s),
    channel_(0) {

    speaker_->publisher().observerAdd(this);
}

//------------------------------------------------------------------------------
SpeakerReactor::~SpeakerReactor() {
    speaker_->publisher().observerDel(this);
}

//------------------------------------------------------------------------------
void
SpeakerReactor::onVolume() {
    if (channel_) {
        CHECK(FMOD_Channel_SetVolume(channel_, speaker_->volume()));
    }
}

//------------------------------------------------------------------------------
void
SpeakerReactor::onState() {
    Speaker::State state = speaker_->state();

    if (sound_ && (state == Speaker::statePlay || state == Speaker::stateLoop)) {

        Vector position;
        Vector velocity;
        FMOD_SYSTEM* system = rootReactor_->system();

        CHECK(FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound_.get(), true, &channel_));
        CHECK(FMOD_Channel_SetUserData(channel_, this));
            //CHECK(FMOD_Channel_SetCallback(channel_, &callback));        
        if (speaker_->state() == Speaker::statePlay) {
            CHECK(FMOD_Channel_SetMode(channel_, FMOD_LOOP_OFF));
        } else if (speaker_->state() == Speaker::stateLoop) {
            CHECK(FMOD_Channel_SetMode(channel_, FMOD_LOOP_NORMAL));
        }

        CHECK(FMOD_Channel_Set3DAttributes(channel_, (FMOD_VECTOR*)&position, (FMOD_VECTOR*)&velocity));
        CHECK(FMOD_Channel_SetVolume(channel_, speaker_->volume()));
        CHECK(FMOD_Channel_SetPaused(channel_, false));

    } else if (sound_ && state == Speaker::stateStop) {
        CHECK(FMOD_Channel_Stop(channel_));
    }
}
