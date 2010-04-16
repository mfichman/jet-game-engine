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

namespace Jet { namespace Core {

//! This class enables render-to-texture.
//! @class RenderTarget
//! @brief Loads and manipulates a render target for render-to-texture.
class RenderTarget : public Object {
public:
    //! Constructor.  Creates the render target and associated texture.
    //! @param the width of the render target
    //! @param height the height of the render target
    //! @param component the type of render target (either GL_DEPTH_COMPONENT
    //! or GL_RGBA).
    RenderTarget(uint32_t width, uint32_t height, uint32_t component);

    //! Destructor.
    virtual ~RenderTarget();

    //! Returns true if the target is enabled
    inline bool enabled() const {
        return enabled_;
    }
    
    //! Returns the texture handle.
    inline uint32_t texture() const {
        return texture_;
    }
    
    //! Enables or disables the render target.
    void enabled(bool enabled);

private:

    uint32_t texture_;
    uint32_t framebuffer_;
    uint32_t component_;
    uint32_t width_;
    uint32_t height_;
    bool enabled_;
};

}}
