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

#include <Jet/OpenGL/Types.hpp>
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>

namespace Jet { namespace OpenGL {

//! This class holds the vertex and index buffers for a triangle mesh.
//! @class MeshBuffer
//! @brief Vertex and index buffers for a triangle mesh.
class MeshBuffer : public Object {
public:
    //! Constructor.
    MeshBuffer(Mesh* mesh);

    //! Destructor.
    virtual ~MeshBuffer();

    //! Renders this mesh.
    //! @param shader the shader to use
    void render(Shader* shader) const;

private:
    GLuint nvertices_;
    GLuint nindices_;
    GLuint vbuffer_;
    GLuint ibuffer_;
};

}}
