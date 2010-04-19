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

#include <Jet/Core/Shader.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Vertex.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>

using namespace Jet;
using namespace std;


Core::Shader::~Shader() {
	state(UNLOADED);
}

void Core::Shader::state(ResourceState state) {
	if (state == state_) {
		return;
	}
	
	// Leaving the UNLOADED state
	if (UNLOADED == state_) {
		// Load source files
		string frag_path = engine_->resource_path(name() + ".frag.glsl");
		string vert_path = engine_->resource_path(name() + ".vert.glsl");
		
		read_source(frag_path, fsource_);
		read_source(vert_path, vsource_);
	}
	
	// Entering the SYNCED state
	if (SYNCED == state) {
		const GLchar* source;
		
		// Load vertex shader
		source = &fsource_.front();
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
		
		// Check for errors
		GLchar log[1024];
		GLsizei length;
		
		// Fragment shader errors
		glGetShaderInfoLog(fshader_, 1024, &length, log);
		if (length) {
			throw runtime_error("Fragment shader log: " + string(log, length));
		}
		
		// Vertex shader errors
		glGetShaderInfoLog(vshader_, 1024, &length, log);
		if (length) {
			throw runtime_error("Vertex shader log: " + string(log, length));
		}
		
		diffuse_map_ = glGetUniformLocation(program_, "diffuse_map");
		specular_map_ = glGetUniformLocation(program_, "specular_map");
		normal_map_ = glGetUniformLocation(program_, "normal_map");
		shadow_map_ = glGetUniformLocation(program_, "shadow_map");
		diffuse_map_enabled_ = glGetUniformLocation(program_, "diffuse_map_enabled");
		specular_map_enabled_ = glGetUniformLocation(program_, "specular_map_enabled");
		normal_map_enabled_ = glGetUniformLocation(program_, "normal_map_enabled");
		shadow_map_enabled_ = glGetUniformLocation(program_, "shadow_map_enabled");
		tangent_attrib_ = glGetAttribLocation(program_, "tangent_in");
	}
	
	// Leaving the SYNCED state
	if (SYNCED == state_) {
		glDeleteProgram(program_);
		glDeleteShader(vshader_);
		glDeleteShader(fshader_);
		program_ = 0;
	}
	
	// Entering the UNLOADED state
	if (UNLOADED == state) {
		fsource_.clear();
		vsource_.clear();
	}
	
	state_ = state;
}

void Core::Shader::read_source(const string& path, vector<char>& source) {
	
    // Open the file
    ifstream in(path.c_str());
    if (in.fail()) {
        throw range_error("Shader not found: " + path);
    }
    
    // Seek to the end, and reserve a buffer
    in.seekg(0, ios::end);
    source.reserve(1 + in.tellg());
    source.resize(in.tellg());
    in.seekg(0, ios::beg);
    
    if (!source.size()) {
        throw runtime_error("Empty shader file: " + path);
    }
    
    // Read the whole buffer in one call
    in.read(&source.front(), source.size());
    source.push_back(0); // Null terminate the string
}

void Core::Shader::enabled(bool enabled) {
	if (enabled == enabled_) {
		return;
	}
	
	if (enabled) {
		state(SYNCED);
		glUseProgram(program_);
		sampler_enabled(SHADOW_MAP_SAMPLER, true);
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(diffuse_map_, DIFFUSE_MAP_SAMPLER);
		glUniform1i(specular_map_, SPECULAR_MAP_SAMPLER);
		glUniform1i(normal_map_, NORMAL_MAP_SAMPLER);
		glUniform1i(shadow_map_, SHADOW_MAP_SAMPLER);
	} else {
		glUseProgram(0);
	}
	
	enabled_ = enabled;
}

void Core::Shader::sampler_enabled(TextureSampler sampler, bool enabled) {
	glActiveTexture(GL_TEXTURE0 + sampler);
	switch (sampler) {
		case DIFFUSE_MAP_SAMPLER:
			glUniform1i(diffuse_map_enabled_, enabled);
			break;
		case SPECULAR_MAP_SAMPLER:
			glUniform1i(specular_map_enabled_, enabled);
			break;
		case NORMAL_MAP_SAMPLER:
			glUniform1i(normal_map_enabled_, enabled);
			break;
		case SHADOW_MAP_SAMPLER:
			glUniform1i(shadow_map_enabled_, enabled);
			break;
	}
}