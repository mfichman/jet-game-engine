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
    Shader(Engine* engine, Jet::Shader* shader);

    //! Destructor.
    virtual ~Shader();

    //! Enables this shader.
    void begin();

    //! Disables this shader.
    void end();
    
    //! Returns the given uniform location.
    //! @param name the name of the uniform location
    //GLuint uniform(const std::string& name);
    
    //! Returns the location of the tangent variable.
    GLint tangent() const {
        return tangent_;
    }

private:
    void source(GLuint shader, const std::string& path);

    GLuint vertex_;
    GLuint fragment_;
    GLuint shader_;
    GLuint program_;
    GLuint sampler_;
    GLint diffuse_map_;
    GLint specular_map_;
    GLint normal_map_;
    GLint shadow_map_;
    GLint shadow_matrix_;
    GLint tangent_;
};

}}
