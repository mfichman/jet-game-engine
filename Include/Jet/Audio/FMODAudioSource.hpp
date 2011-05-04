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
#pragma once

#include <Jet/Audio/FMODTypes.hpp>
#include <Jet/Audio/FMODSound.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Scene/AudioSource.hpp>
#include <vector>

namespace Jet {
    
//! Manages 3D sounds emitting from the node.
//! @class AudioSource
//! @brief Emits positional audio.
class FMODAudioSource : public AudioSource {
public:
    inline FMODAudioSource(CoreEngine* engine, CoreNode* parent) :
        engine_(engine),
        parent_(parent) {
        
    }
    
    //! Returns the parent node
    inline CoreNode* parent() const {
        return parent_;
    }
    
    //! Returns the clip playing at the given channel.
    //! @param chan the channel to use
    inline FMODSound* sound(size_t chan) const {
        return sound_[chan].get();
    }
    
    //! Returns the state of the clip playing at the given channel.
    //! @param chan the channel to use
    PlaybackState state(size_t chan) const;
    
    //! Sets the sound clip at the given index for the audio
    //! source.
    //! @param chan the channel to use
    //! @param name the name of the sound clip
    inline void sound(size_t chan, const std::string& name) {
        sound(chan, engine_->sound(name));
    }
    
    //! Sets the sound clip at the given index for the audio
    //! source.
    //! @param chan the channel to use
    //! @param sound the sound clip
    void sound(size_t chan, Sound* sound);
    
    //! Sets the mode of the given channel.
    //! @param chan the channel to use
    //! @param mode the audio state
    void state(size_t chan, PlaybackState state);
    
    //! Sets the position of this audio source.
    void position(const Vector& position);
    
private:    
    static FMOD_RESULT F_CALLBACK on_channel_event(FMOD_CHANNEL* ch, FMOD_CHANNEL_CALLBACKTYPE type, void* data1, void* data2);

    CoreEngine* engine_;
    CoreNode* parent_;
    std::vector<FMODSoundPtr> sound_;
    std::vector<FMOD_CHANNEL*> channel_;
};

}