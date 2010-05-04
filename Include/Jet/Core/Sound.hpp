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

#include <Jet/Core/Types.hpp>
#include <Jet/Sound.hpp>
#include <fmodex/fmod.h>

namespace Jet { namespace Core {

//! Class to hold a sound data.
//! @class Sound
//! @brief Class to hold sound data.
class Sound : public Jet::Sound {
public:
    //! Creates a new sound with the given name.
    inline Sound(Engine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(UNLOADED),
        sound_(0) {
			
	}
    
    //! Destroys the sound.
    virtual ~Sound();
    
	//! Sets the state of the shader
	inline ResourceState state() const {
        return state_;
    }

	//! Returns the name of the texture
	inline const std::string& name() const {
        return name_;
    }
    
    //! Returns the sound.
    inline FMOD_SOUND* sound() const {
        return sound_;
    }

	//! Returns the resource state of the shader
	void state(ResourceState state);
    
private:    
    Engine* engine_;
    std::string name_;
    ResourceState state_;
    FMOD_SOUND* sound_;
    
    friend class Engine;
};

}}
