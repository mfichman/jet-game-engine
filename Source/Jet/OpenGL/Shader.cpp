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
#include <fstream>
#include <vector>

using namespace Jet::OpenGL;
using namespace std;

Shader::Shader(const std::string& path) {
    fragment_ = glCreateShader(GL_FRAGMENT_SHADER);
    source(fragment_, path + ".frag.glsl");

    vertex_ = glCreateShader(GL_VERTEX_SHADER);
    source(vertex_, path + ".vert.glsl");

    program_ = glCreateProgram();
    glAttachShader(program_, fragment_);
    glAttachShader(program_, vertex_);
    glLinkProgram(program_);

    texture_map_ = glGetUniformLocation(program_, "texture_map");
    specular_map_ = glGetUniformLocation(program_, "specular_map");
    normal_map_ = glGetUniformLocation(program_, "normal_map");
    environment_map_ = glGetUniformLocation(program_, "environment_map");

    binormal_ = glGetAttribLocation(program_, "binormal");

    GLchar log[1024];
    GLsizei length;
    glGetShaderInfoLog(fragment_, 1024, &length, log);
    if (length) {
        throw runtime_error("Fragment shader log: " + string(log, length));
    }
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
    vector<GLchar> buffer;
    const GLchar* source;

    ifstream in(path.c_str());
    in.seekg(0, ios::end);
    buffer.reserve(1 + in.tellg());
    buffer.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&buffer.front(), buffer.size());
    in.close();
    buffer.push_back(0); // Null terminate the string
    source = &buffer.front();

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
}

void Shader::begin() {
    glUseProgram(program_);
    glUniform1i(texture_map_, 0);
    glUniform1i(specular_map_, 1);
    glUniform1i(normal_map_, 2);
    glUniform1i(environment_map_, 3);
}

void Shader::end() {
    glUseProgram(0);
}

