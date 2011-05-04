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

#include <Jet/Graphics/OpenGLShader.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Types/Vertex.hpp>
#include <fstream>

using namespace Jet;
using namespace std;


OpenGLShader::~OpenGLShader() {
	state(RS_UNLOADED);
}

void OpenGLShader::state(ResourceState state) {
	if (state == state_ || !engine_->option<bool>("shaders_enabled")) {
		return;
	}
	
	// Leaving the RS_UNLOADED state
	if (RS_UNLOADED == state_) {
		// Load source files
		string frag_path = engine_->resource_path(name() + ".frag.glsl");
		string vert_path = engine_->resource_path(name() + ".vert.glsl");
		
		read_source(frag_path, fsource_);
		read_source(vert_path, vsource_);
	}
	
	// Entering the RS_LOADED state
	if (RS_LOADED == state) {
		init_program();
	}
	
	// Leaving the RS_LOADED state
	if (RS_LOADED == state_) {
		assert(program_ && vshader_ && fshader_);
		glUseProgram(0);
		glDeleteShader(vshader_);
		glDeleteShader(fshader_);
		glDeleteProgram(program_);
		program_ = 0;
		vshader_ = 0;
		fshader_ = 0;
	}
	
	// Entering the RS_UNLOADED state
	if (RS_UNLOADED == state) {
		fsource_.clear();
		vsource_.clear();
	}
	
	state_ = state;
}

void OpenGLShader::init_program() {
	
	// Load vertex shader
	const GLchar* source = &fsource_.front();
	fshader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader_, 1, &source, 0);
	glCompileShader(fshader_);
	
	// Load fragment shader
	source = &vsource_.front();
	vshader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader_, 1, &source, 0);
	glCompileShader(vshader_);
	
	// Create the vertex program
	program_ = glCreateProgram();
	glAttachShader(program_, fshader_);
	glAttachShader(program_, vshader_);
	glLinkProgram(program_);

	
	// Fragment shader errors
	GLint success;
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (!success) {
		// Check for errors
		GLchar log[1024];
		GLsizei length;

		glGetShaderInfoLog(fshader_, 1024, &length, log);
		string fshader_log(log, length);
		glGetShaderInfoLog(vshader_, 1024, &length, log);
		string vshader_log(log, length);
		throw runtime_error(fshader_log + vshader_log);
	}	
}

void OpenGLShader::read_source(const string& path, vector<char>& source) {
	
    // Open the file
    ifstream in(path.c_str());
    if (in.fail()) {
        throw range_error("Shader not found: " + path);
    }
    
    // Seek to the end, and reserve a buffer
    in.seekg(0, ios::end);
    source.reserve(static_cast<size_t>(1 + in.tellg()));
    source.resize(static_cast<size_t>(in.tellg()));
    in.seekg(0, ios::beg);
    
    if (!source.size()) {
        throw runtime_error("Empty shader file: " + path);
    }
    
    // Read the whole buffer in one call
    in.read(&source.front(), source.size());
    source.push_back(0); // Null terminate the string
}

void OpenGLShader::enabled(bool enabled) {
	if (enabled == enabled_ || !engine_->option<bool>("shaders_enabled")) {
		return;
	}
	
	if (enabled) {
		state(RS_LOADED);
		glUseProgram(program_);
	} else {
		glUseProgram(0);
	}
	
	enabled_ = enabled;
}

GLint OpenGLShader::uniform_location(const std::string& name) const {
	return glGetUniformLocation(program_, name.c_str());
}

GLint OpenGLShader::attrib_location(const std::string& name) const {
	return glGetAttribLocation(program_, name.c_str());
}