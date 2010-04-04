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
#include <Jet/Matrix.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace Jet::OpenGL;
using namespace Jet;
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

    ifstream in(path.c_str());
    if (in.fail()) {
        throw range_error("Shader not found: " + path);
    }
    
    in.seekg(0, ios::end);
    buffer.reserve(1 + in.tellg());
    buffer.resize(in.tellg());
    in.seekg(0, ios::beg);
    
    if (!buffer.size()) {
        throw runtime_error("Empty shader file: " + path);
    }
    
    in.read(&buffer.front(), buffer.size());
    in.close();
    buffer.push_back(0); // Null terminate the string

    vector<const GLchar*> source;
    source.push_back("#define LIGHT_POINT\n");
    source.push_back("#define SHADOW_MAP\n");
    source.push_back(&buffer.front());
    glShaderSource(shader, source.size(), &source.front(), 0);
    glCompileShader(shader);
}

void Shader::texture(const std::string& name, GLuint texture) {    
    GLuint location = glGetUniformLocation(program_, name.c_str());
    if (texture) {
        texture_.insert(make_pair(location, texture));
    } else {
        texture_.erase(location);
    }
}

void Shader::texture_matrix(const std::string& name, const Matrix& matrix) {
    GLuint location = glGetUniformLocation(program_, name.c_str());
    texture_matrix_.insert(make_pair(location, matrix));
}

void Shader::begin() {
    
    glUseProgram(program_);
        
    GLuint sampler = 0;
    for (map<GLuint, GLuint>::iterator i = texture_.begin(); i != texture_.end(); i++) {
        glActiveTexture(GL_TEXTURE0 + sampler);
        glBindTexture(GL_TEXTURE_2D, i->second);
        glUniform1i(i->first, sampler);
        
        map<GLuint, Matrix>::iterator j = texture_matrix_.find(i->first);
        if (j != texture_matrix_.end()) {
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
            glLoadMatrixf(j->second);
            glMatrixMode(GL_MODELVIEW);
        }

        sampler++;
    }
    
}

void Shader::end() {
    glUseProgram(0);
    
    GLuint sampler = 0;
    for (map<GLuint, GLuint>::iterator i = texture_.begin(); i != texture_.end(); i++) {
        glActiveTexture(GL_TEXTURE0 + sampler);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        map<GLuint, Matrix>::iterator j = texture_matrix_.find(i->first);
        if (j != texture_matrix_.end()) {
            glMatrixMode(GL_TEXTURE);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
        
        sampler++;
    }
}

