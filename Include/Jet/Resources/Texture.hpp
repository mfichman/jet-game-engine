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

namespace Jet {

//! Class to hold a texture data for rendering.
//! @class Texture
//! @brief Class to hold texture data.
class Texture : public Object {
public:
    //! Returns the width of this texture in pixels.
    virtual size_t width() const=0;

    //! Returns the height of this texture in pixels.
    virtual size_t height() const=0;

    //! Returns a pointer to the texture data.
    virtual const uint8_t* data() const=0;

    //! Returns a pointer to the texture data.
    virtual uint8_t* data()=0;
    
	//! Sets the state of the shader
	virtual ResourceState state() const=0;

	//! Returns the name of the texture
	virtual const std::string& name() const=0;

    //! Sets the width of the texture in pixels.
    //! @param width the new width
    virtual void width(size_t width)=0;

    //! Sets the height of the texture in pixels.
    //! @param height the new height
    virtual void height(size_t height)=0;

	//! Returns the resource state of the shader
	virtual void state(ResourceState state)=0;
};

}
