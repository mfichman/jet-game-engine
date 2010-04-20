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
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

namespace Jet { namespace Core {

//! This class enables render-to-texture.
//! @class RenderTarget
//! @brief Loads and manipulates a render target for render-to-texture.
class RenderTarget : public Object {
public:
    //! Constructor.  Creates the render target and associated texture.
    //! @param engine the engine
    //! @param the width of the render target
    //! @param height the height of the render target
	//! @param depth_only true if this render target should save depth only (for shadow mapping)
	//! @param ntargets the number of render targets to be used with this RT
    RenderTarget(uint32_t width, uint32_t height, bool depth_only, uint32_t ntargets=1);

    //! Destructor.
    virtual ~RenderTarget();

    //! Returns true if the target is enabled
    inline bool enabled() const {
        return enabled_;
    }
    
    //! Enables or disables the render target.
    void enabled(bool enabled);
    
    //! Sets the sampler the texture is bound to.
	//! @param the sampler to bind the texture to
	//! @param index the texture to bind to the sampler;
	//! when multiple render targets are used, this could
	//! be render target 0 or render target 1
	void sampler(uint32_t sampler, size_t index=0);

private:
    uint32_t texture_[2];
    uint32_t framebuffer_;
	uint32_t depth_buffer_;
    uint32_t width_;
    uint32_t height_;
	uint32_t ntargets_;
    bool enabled_;
	bool depth_only_;
};

}}
