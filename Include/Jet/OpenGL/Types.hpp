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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOVT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BVT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AVTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OVT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <boost/intrusive_ptr.hpp>

namespace Jet { namespace OpenGL {
    class MeshBuffer;
    class Renderer;
    class RenderTarget;
    class Shader;
    class TextureBuffer;

    typedef boost::intrusive_ptr<MeshBuffer> MeshBufferPtr;
    typedef boost::intrusive_ptr<Renderer> RendererPtr;
    typedef boost::intrusive_ptr<RenderTarget> RenderTargetPtr;
    typedef boost::intrusive_ptr<Shader> ShaderPtr;
    typedef boost::intrusive_ptr<TextureBuffer> TextureBufferPtr;
    
    enum MeshShaderOption {
        MSO_SHADOW_MAP = 1,
        MSO_DIFFUSE_MAP = 2,
        MSO_SPECULAR_MAP = 4,
        MSO_NORMAL_MAP = 8
    };
}}
