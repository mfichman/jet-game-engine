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

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <map>

namespace Jet { namespace OpenGL {

//! This class loads and manipulates a GPU shader ().
//! @class Shader
//! @brief Loads and manipulates a  shader.
class Shader : public Object {
public:
    //! Constructor.
    Shader(const std::string& path);

    //! Destructor.
    virtual ~Shader();

    //! Enables this shader.
    void begin();

    //! Disables this shader.
    void end();
    
    //! Attaches a texture sampler to this shader.
    //! @param name the name of the texture sampler in the shader source
    //! @param texture the texture handle
    void texture(const std::string& name, GLuint texture);
    
    //! Attaches a texture matrix to be used with the given
    //! texture.
    //! @param name the name of the texture sampler in the shader source
    //! @param matrix the texture matrix
    void texture_matrix(const std::string& name, const Matrix& matrix);

private:
    void source(GLuint shader, const std::string& path);
    
    std::map<GLuint, GLuint> texture_;
    std::map<GLuint, Matrix> texture_matrix_;

    GLuint vertex_;
    GLuint fragment_;
    GLuint shader_;
    GLuint program_;
    GLuint binormal_;
};

}}
