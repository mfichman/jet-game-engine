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
#include <Jet/SceneComponent.hpp>
#include <string>

namespace Jet {

//! This class plays sound effects.  If 3D sound is enabled, the origin of 
//! the sound will be equal to the position of the parent scene node.  To play
//! non-position audio, like music, use a ambient sounds.
//! @class AudioSource
//! @brief Plays sound effects.
class JETAPI AudioSource : public SceneComponent {
public:

    //! Name of the sound currently playing.
    virtual const std::string& get_sound() const=0;

    //! Returns the gain or volume of the sound.  Note that this is the volume
    //! of the sound if the listener is right next to the parent scene node;
    //! if the listener is farther away the perceived volume will be less.
    virtual real_t get_gain() const=0;

    //! Returns true if the sound should be looped.
    virtual bool is_looping_enabled() const=0;

    //! Returns true if the sound is playing
    virtual bool is_playing() const=0;
    
    //! Sets the sound currently playing.
    //! @param sound the name of the new sound
    virtual void set_sound(const std::string& s)=0;

    //! Sets the gain or volume of the sound.
    //! @param f the gain of the sound
    virtual void set_gain(real_t f)=0;

    //! Sets looping for sounds.
    //! @param b true to enable looping
    virtual void set_looping_enabled(bool b)=0;

    //! Plays/stops the sound.
    //! @param b true to start the sound
    virtual void set_playing(bool b)=0;

private:
    AudioSource(SceneNode* parent, const std::string& name);
};

}
