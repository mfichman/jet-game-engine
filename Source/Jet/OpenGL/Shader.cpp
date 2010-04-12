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

#include <Jet/OpenGL/Shader.hpp>
#include <Jet/OpenGL/Types.hpp>
#include <Jet/Matrix.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Shader.hpp>
#include <Jet/Engine.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace Jet::OpenGL;
using namespace std;

Shader::Shader(Jet::Engine* engine, Jet::Shader* shader) :
    sampler_(0) {

	shader->loaded(true);
    shader->impl(this);

    // Find the shader files.  Both the .frag.glsl file and
    // the .vert.glsl files must exist
	std::string frag_path = engine->resource_path(shader->name() + ".frag.glsl");
    std::string vert_path = engine->resource_path(shader->name() + ".vert.glsl");
        
    // Load the fragment shader
    fragment_ = glCreateShader(GL_FRAGMENT_SHADER);
    source(fragment_, frag_path);

    // Load the vertex shader
    vertex_ = glCreateShader(GL_VERTEX_SHADER);
    source(vertex_, vert_path);

    // Create the vertex program and attach the shaders
    program_ = glCreateProgram();
    glAttachShader(program_, fragment_);
    glAttachShader(program_, vertex_);
    glLinkProgram(program_);
    
    // Get the uniform locations of common variables
    diffuse_map_ = glGetUniformLocation(program_, "diffuse_map");
    specular_map_ = glGetUniformLocation(program_, "specular_map");
    normal_map_ = glGetUniformLocation(program_, "normal_map");
    shadow_map_ = glGetUniformLocation(program_, "shadow_map");
    shadow_matrix_ = glGetUniformLocation(program_, "shadow_matrix");
    tangent_ = glGetAttribLocation(program_, "tangent_in");

    // If there was an error, report it and throw an exception
    GLchar log[1024];
    GLsizei length;
    
    // Fragment shader errors
    glGetShaderInfoLog(fragment_, 1024, &length, log);
    if (length) {
        throw runtime_error("Fragment shader log: " + string(log, length));
    }
    
    // Vertex shader errors
    glGetShaderInfoLog(vertex_, 1024, &length, log);
    if (length) {
        throw runtime_error("Vertex shader log: " + string(log, length));
    }
}

Shader::~Shader() {
    glDeleteProgram(program_);
    glDeleteShader(vertex_);
    glDeleteShader(fragment_);
}

void Shader::source(GLuint shader, const std::string& path) {
    std::vector<GLchar> buffer;

    // Open the file
    ifstream in(path.c_str());
    if (in.fail()) {
        throw range_error("Shader not found: " + path);
    }
    
    // Seek to the end, and reserve a buffer
    in.seekg(0, ios::end);
    buffer.reserve(1 + in.tellg());
    buffer.resize(in.tellg());
    in.seekg(0, ios::beg);
    
    if (!buffer.size()) {
        throw runtime_error("Empty shader file: " + path);
    }
    
    // Read the whole buffer in one call
    in.read(&buffer.front(), buffer.size());
    buffer.push_back(0); // Null terminate the string

    // 
    std::vector<const GLchar*> source;
    source.push_back(&buffer.front());
    glShaderSource(shader, source.size(), &source.front(), 0);
    glCompileShader(shader);
}

void Shader::begin() {
    glUseProgram(program_);
    glUniform1i(diffuse_map_, DIFFUSE_MAP_SAMPLER);
    glUniform1i(specular_map_, SPECULAR_MAP_SAMPLER);
    glUniform1i(normal_map_, NORMAL_MAP_SAMPLER);
    glUniform1i(shadow_map_, SHADOW_MAP_SAMPLER);
}

void Shader::end() {
    glUseProgram(0);
   /* glActiveTexture(GL_TEXTURE0 + DIFFUSE_MAP_SAMPLER);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + SPECULAR_MAP_SAMPLER);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + NORMAL_MAP_SAMPLER);
    glBindTexture(GL_TEXTURE_2D, 0);*/
}

