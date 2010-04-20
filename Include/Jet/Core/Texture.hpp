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
#include <Jet/Texture.hpp>
#include <vector>

namespace Jet { namespace Core {

//! Class to hold a texture data for rendering.
//! @class Texture
//! @brief Class to hold texture data.
class Texture : public Jet::Texture {
public:
    //! Destructor.
    virtual ~Texture();

    //! Returns the width of this texture in pixels.
    inline size_t width() const {
        return width_;
    }

    //! Returns the height of this texture in pixels.
    inline size_t height() const {
        return height_;
    }

    //! Returns a pointer to the texture data.
    inline const uint8_t* data() const {
        return &data_.front();
    }

    //! Returns a pointer to the texture data.
    inline uint8_t* data() {
        return &data_.front();
    }
	
	//! Returns the state of the texture
	inline ResourceState state() const {
		return state_;
	}

	//! Returns the name of the texture
	inline const std::string& name() const {
		return name_;
	}

    //! Sets the width of the texture in pixels.
    //! @param width the new width
    inline void width(size_t width) {
		width_ = width;
		data_.resize(width_ * height_ * sizeof(uint32_t));
	}

    //! Sets the height of the texture in pixels.
    //! @param height the new height
    inline void height(size_t height) {
		height_ = height;
		data_.resize(width_ * height_ * sizeof(uint32_t));
	}
	
	//! Sets the resource state
	void state(ResourceState state);
	
	//! Sets the sampler this texture is bound to.
	void sampler(uint32_t sampler);

private:
	inline Texture(Engine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(UNLOADED),
		width_(0),
		height_(0),
		texture_(0) {
			
	}
    
	Engine* engine_;
	std::string name_;
	ResourceState state_;
    std::vector<uint8_t> data_;
    size_t width_;
    size_t height_;
	uint32_t texture_;
	

    friend class Engine;
};

}}
