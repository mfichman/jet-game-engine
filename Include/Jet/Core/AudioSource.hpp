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

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/AudioSource.hpp>
#include <vector>

namespace Jet { namespace Core {

//! Manages 3D sounds emitting from the node.
//! @class AudioSource
//! @brief Emits positional audio.
class AudioSource : public Jet::AudioSource {
public:
    
    //! Returns the parent node
    inline Node* parent() const {
        return parent_;
    }
    
    //! Returns the clip playing at the given channel.
    //! @param chan the channel to use
    inline const std::string& clip(size_t chan) const {
        return clip_[chan];
    }
    
    //! Returns the state of the clip playing at the given channel.
    //! @param chan the channel to use
    inline PlaybackState state(size_t chan) const {
        return state_[chan];
    }
    
    //! Sets the sound clip at the given index for the audio
    //! source.
    //! @param chan the channel to use
    //! @param name the name of the sound clip
    inline void clip(size_t chan, const std::string& name) {
        throw std::runtime_error("Not implemented");
    }
    
    //! Sets the mode of the given channel.
    //! @param chan the channel to use
    //! @param mode the audio state
    inline void state(size_t chan, PlaybackState state) {
        throw std::runtime_error("Not implemented");
    }
private:
    inline AudioSource(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent) {
        
    }

    Engine* engine_;
    Node* parent_;
    std::vector<std::string> clip_;
    std::vector<PlaybackState> state_;
    
    friend class Node;
};

}}