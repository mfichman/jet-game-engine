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

namespace Jet { namespace OpenGL {
    class Cubemap;
    class Font;
    class Material;
    class Mesh;
    class ParticleBuffer;
    class RenderSystem;
    class RenderTarget;
    class Shader;
    class Texture;
    
    typedef boost::intrusive_ptr<Cubemap> CubemapPtr;
    typedef boost::intrusive_ptr<Font> FontPtr;
    typedef boost::intrusive_ptr<Material> MaterialPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    typedef boost::intrusive_ptr<ParticleBuffer> ParticleBufferPtr;
    typedef boost::intrusive_ptr<RenderSystem> RenderSystemPtr;
    typedef boost::intrusive_ptr<RenderTarget> RenderTargetPtr;
    typedef boost::intrusive_ptr<Shader> ShaderPtr;
    typedef boost::intrusive_ptr<Texture> TexturePtr;

    enum TextureSampler {
        DIFFUSE_MAP_SAMPLER = 0,
        SPECULAR_MAP_SAMPLER = 1,
        NORMAL_MAP_SAMPLER = 2,
        SHADOW_MAP_SAMPLER = 3
    };
}}
