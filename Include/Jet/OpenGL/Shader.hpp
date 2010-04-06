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
#include <Jet/Matrix.hpp>

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace Jet { namespace OpenGL {

//! This class loads and manipulates a GPU shader ().
//! @class Shader
//! @brief Loads and manipulates a  shader.
class Shader : public Object {
public:
    //! Constructor.
    Shader(const std::string& path, const std::vector<std::string>& defines);

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
    void matrix(const std::string& name, const Matrix& matrix);
    
    //! Attaches a vector to the shader
    //! @param name the name of the texture location
    //! @param vector the vector
    void vector(const std::string& name, const Vector& vector);
    
    //! Returns the given uniform location.
    //! @param name the name of the uniform location
    GLuint uniform(const std::string& name);

private:
    void source(GLuint shader, const std::string& path);

    GLuint vertex_;
    GLuint fragment_;
    GLuint shader_;
    GLuint program_;
    GLuint sampler_;
    std::vector<std::string> defines_;
};

}}
