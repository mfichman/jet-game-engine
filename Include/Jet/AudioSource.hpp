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

#include <Jet/Jet.hpp>
#include <Jet/Object.hpp>
#include <string>

namespace Jet {

//! This class plays sound effects.  If 3D sound is enabled, the origin of 
//! the sound will be equal to the position of the parent scene node.  To play
//! non-positional audio, like music, use a ambient sounds.
//! @class AudioSource
//! @brief Plays sound effects.
class AudioSource : public Object {
public:
    //! Returns the parent scene node
    virtual SceneNode* get_parent() const=0;

    //! Name of the sound currently playing.
    virtual const std::string& get_sound() const=0;

    //! Returns the gain or volume of the sound.  Note that this is the volume
    //! of the sound if the listener is right next to the parent scene node;
    //! if the listener is farther away the perceived volume will be less.
    virtual real_t get_gain() const=0;

    //! Returns the playback mode of the sound.
    virtual PlaybackMode get_playback_mode() const=0;

    //! Returns whether or not the audio clip is stopped/playing/paused.
    virtual PlaybackState get_playback_state() const=0;
    
    //! Sets the sound currently playing.
    //! @param sound the name of the new sound
    virtual void set__sound(const std::string& s)=0;

    //! Sets the gain or volume of the sound.
    //! @param f the gain of the sound
    virtual void set_gain(real_t f)=0;

    //! Sets looping for sounds.  The options are PM_LOOP for looping playback, 
    //! or PM_ONCE to play the sound once, then stop.
    //! @param b playback mode
    virtual void set_playback_mode(PlaybackMode b)=0;

    //! Plays/stops the sound.  PS_PLAYING starts playing the sound, 
    //! PS_STOPPED stops the sound, and PS_PAUSED pasues the sound.
    //! @param b the playback state
    virtual void set_playback_state(PlaybackState b)=0;

protected:
    //! Clones this object
    virtual AudioSource* clone()=0;
};

}
