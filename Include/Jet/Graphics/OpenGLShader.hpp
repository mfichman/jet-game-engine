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

#include <Jet/Graphics/OpenGLTypes.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Resources/Shader.hpp>
#include <vector>

namespace Jet { 

//! Shader loader class.
//! @class Shader
//! @brief Shader.
class OpenGLShader : public Shader {
public:
	//! Creates a shader.
    inline OpenGLShader(CoreEngine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(RS_UNLOADED),
		vshader_(0),
		fshader_(0),
		program_(0),
		enabled_(false) {
	}
	
	virtual ~OpenGLShader();
    
    //! Returns the shader's path.
    inline const std::string& name() const {
		return name_;
	}
	
	//! Sets the state of the shader
	inline ResourceState state() const {
		return state_;
	}
	
	//! Returns true if the shader is bound
	inline bool enabled() const {
		return enabled_;
	}
	
	//! Returns the resource state of the shader
	void state(ResourceState state);
	
	//! Turns this shader on or off
	void enabled(bool enabled);
	
	//! Returns the uniform location of the given parameter.
	GLint uniform_location(const std::string& name) const;

	//! Returns the attribute location of the given parameter.
	GLint attrib_location(const std::string& name) const;
    
private:	
	void init_program();
	void read_source(const std::string& path, std::vector<char>& source);
	
    CoreEngine* engine_;
    std::string name_;
    ResourceState state_;
	GLuint vshader_;
	GLuint fshader_;
	GLuint program_;
	bool enabled_;
	std::vector<char> vsource_;
	std::vector<char> fsource_;
    
    friend class Engine;
};

}
