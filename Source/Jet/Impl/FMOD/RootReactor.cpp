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

#include <Jet/Impl/FMOD/RootReactor.hpp>
#include <Jet/Impl/FMOD/SpeakerReactor.hpp>
#include <stdexcept>
#include <fmodex/fmod_errors.h>

using namespace Jet;
using namespace Jet::Impl::FMOD;

#pragma warning(disable: 4190)

//------------------------------------------------------------------------------
#define CHECK(x) {															\
	FMOD_RESULT result = x;													\
	if (result != FMOD_OK) {												\
		throw logic_error(FMOD_ErrorString(result));     					\
	}																		\
}

//------------------------------------------------------------------------------
extern "C" JETAPI Interface*
moduleLoad(Root* r) {  
    return new RootReactor(r);
}

//------------------------------------------------------------------------------
RootReactor::RootReactor(Root::Ptr e) :
    root_(e) {

    root_->publisher().observerAdd(this);
    root_->options()->publisher().observerAdd(this);

    FMOD_SPEAKERMODE mode;
    FMOD_CAPS caps;

    CHECK(FMOD_System_Create(&system_));
    CHECK(FMOD_System_GetDriverCaps(system_, 0, &caps, 0, 0, &mode));
    CHECK(FMOD_System_SetSpeakerMode(system_, mode));
    CHECK(FMOD_System_Init(system_, 32, FMOD_INIT_NORMAL, 0));
    CHECK(FMOD_System_Set3DSettings(system_, 1.0f, 1.0f, 0.05f));

}

//------------------------------------------------------------------------------
RootReactor::~RootReactor() {
    root_->publisher().observerDel(this);
    root_->options()->publisher().observerDel(this);

    reactors_.clear();
    CHECK(FMOD_System_Close(system_));
    CHECK(FMOD_System_Release(system_));
}

//------------------------------------------------------------------------------
void
RootReactor::onSpeakerNew(Speaker::Ptr s) {
    reactors_.push_back(new SpeakerReactor(s, this));
}

//------------------------------------------------------------------------------
void 
RootReactor::onSoundNew(Sound::Ptr) {
    
}

//------------------------------------------------------------------------------
void
RootReactor::onMasterVolume() {

    FMOD_CHANNELGROUP* group;
    CHECK(FMOD_System_GetMasterChannelGroup(system_, &group));
    CHECK(FMOD_ChannelGroup_SetVolume(group, root_->options()->masterVolume()));
}
