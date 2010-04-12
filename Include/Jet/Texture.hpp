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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <vector>

namespace Jet {

//! Class to hold a texture data for rendering.
//! @class Texture
//! @brief Class to hold texture data.
class JETAPI Texture : public Object {
public:
    //! Destructor.
    virtual ~Texture() {}

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
    
    //! Returns true if the texture is loaded.
    inline bool loaded() const {
        return loaded_;
    }
	
	//! Returns the implementation object.
	inline Object* impl() const {
		return impl_.get();
	}

	//! Returns the name of the texture
	inline const std::string& name() const {
		return name_;
	}

    //! Sets the width of the texture in pixels.
    //! @param width the new width
    void width(size_t width);

    //! Sets the height of the texture in pixels.
    //! @param height the new height
    void height(size_t height);
    
    //! Marks this texture as loaded.
    void loaded(bool loaded);
	
	//! Sets the implementation object.
	inline void impl(Object* object) {
		impl_ = object;
	}
    
private:
	Texture(Engine* engine, const std::string& name);
    
	Engine* engine_;
	std::string name_;
    bool loaded_;
#pragma warning(disable:4251)
	ObjectPtr impl_;
    std::vector<uint8_t> data_;
    size_t width_;
    size_t height_;
#pragma warning(default:4251)

    friend class Engine;
};

}
