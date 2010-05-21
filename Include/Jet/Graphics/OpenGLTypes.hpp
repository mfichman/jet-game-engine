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
#define MAX_SHADOW_CASCADES 4U
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define putenv _putenv
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace Jet {
    class OpenGLCubemap;
    class OpenGLFont;
    class OpenGLMaterial;
    class OpenGLMesh;
    class OpenGLParticleBuffer;
    class OpenGLRenderSystem;
    class OpenGLRenderTarget;
    class OpenGLShader;
    class OpenGLTexture;
    
    typedef boost::intrusive_ptr<OpenGLCubemap> OpenGLCubemapPtr;
    typedef boost::intrusive_ptr<OpenGLFont> OpenGLFontPtr;
    typedef boost::intrusive_ptr<OpenGLMaterial> OpenGLMaterialPtr;
    typedef boost::intrusive_ptr<OpenGLMesh> OpenGLMeshPtr;
    typedef boost::intrusive_ptr<OpenGLParticleBuffer> OpenGLParticleBufferPtr;
    typedef boost::intrusive_ptr<OpenGLRenderSystem> OpenGLRenderSystemPtr;
    typedef boost::intrusive_ptr<OpenGLRenderTarget> OpenGLRenderTargetPtr;
    typedef boost::intrusive_ptr<OpenGLShader> OpenGLShaderPtr;
    typedef boost::intrusive_ptr<OpenGLTexture> OpenGLTexturePtr;

    enum OpenGLTextureSampler {
        TS_DIFFUSE = 0,
        TS_SPECULAR = 1,
        TS_NORMAL = 2,
        TS_SHADOW = 3
    };
}
