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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

namespace Jet {

//! Manages 3D sounds emitting from the node.
//! @class AudioSource
//! @brief Emits positional audio.
class AudioSource : public Object {
public:    
    //! Returns the parent node
    virtual Node* parent() const=0;
    
    //! Returns the clip playing at the given channel.
    //! @param chan the channel to use
    virtual Sound* sound(size_t chan) const=0;
    
    //! Returns the state of the clip playing at the given channel.
    //! @param chan the channel to use
    virtual PlaybackState state(size_t chan) const=0;
    
    //! Sets the sound clip at the given index for the audio
    //! source.
    //! @param chan the channel to use
    //! @param name the name of the sound clip
    virtual void sound(size_t chan, const std::string& name)=0;

    //! Sets the sound clip for the given channel.
    //! @param chan the channel to use
    //! @param sound the sound clipe
    virtual void sound(size_t chan, Sound* sound)=0;
    
    //! Sets the mode of the given channel.
    //! @param chan the channel to use
    //! @param mode the audio state
    virtual void state(size_t chan, PlaybackState state)=0;
    
    //! Sets the rigid body position
    virtual void position(const Vector& position)=0;
};

}