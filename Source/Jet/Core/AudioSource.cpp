/*
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

#include <Jet/Core/AudioSource.hpp>
#include <Jet/Core/AudioSystem.hpp>
#include <fmodex/fmod_errors.h>

using namespace Jet;
using namespace std;


inline void fmod_check(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        throw std::runtime_error(FMOD_ErrorString(result));
    }
}

inline PlaybackState Core::AudioSource::state(size_t chan) const {
    
    // If the channel is not allocated, then stop
    if (chan >= channel_.size()) {
        return STOP;
    }
    
    // If the channel is set to null, then the audio has stopped.
    if (!channel_[chan]) {
        return STOP;
    }
    
    // Otherwise, the audio is playing and the channel is allocated
    return PLAY;
}

void Core::AudioSource::sound(size_t chan, Jet::Sound* sound) {
    // If we don't have enough space, then allocate another slot in the set
    // of vectors for this channel.
    if (chan > sound_.size()) {
        sound_.resize(chan + 1);
        channel_.resize(chan + 1);
    }
    sound_[chan] = static_cast<Sound*>(sound);
}

void Core::AudioSource::state(size_t chan, PlaybackState state) {
    // If we don't have enough space, then allocate another slot in the set
    // of vectors for this channel.
    if (chan > channel_.size()) {
        sound_.resize(chan + 1);
        channel_.resize(chan + 1);
    }
    
    // If the state is the same, quit (idempotence)
    if (AudioSource::state(chan) == state) {
        return;
    }
    
    if (STOP == state && channel_[chan]) {
        // Stop the clip
        fmod_check(FMOD_Channel_Stop(channel_[chan]));
    }
    
    if (PLAY == state && sound_[chan] && !channel_[chan]) {
        // Make sure the sound is loaded, and that the channel is not in use.
        // Then play the sound and register callbacks
        sound_[chan]->state(SYNCED);
        FMOD_SYSTEM* system = engine_->audio_system()->system();
        FMOD_SOUND* sound = sound_[chan]->sound();
        fmod_check(FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, true, &channel_[chan]));
        fmod_check(FMOD_Channel_SetUserData(channel_[chan], this));
        fmod_check(FMOD_Channel_SetCallback(channel_[chan], &on_channel_event));
        fmod_check(FMOD_Channel_SetPaused(channel_[chan], false));
    }
}

FMOD_RESULT F_CALLBACK Core::AudioSource::on_channel_event(FMOD_CHANNEL* ch, FMOD_CHANNEL_CALLBACKTYPE type, void* data1, void* data2) {
    AudioSource* self;
    fmod_check(FMOD_Channel_GetUserData(ch, (void**)&self));
    
    // Find the channel and disable it
    for (size_t i = 0; i < self->channel_.size(); i++) {
        if (self->channel_[i] == ch) {
            self->channel_[i] = 0;
        }
    }
    
    return FMOD_OK;
}