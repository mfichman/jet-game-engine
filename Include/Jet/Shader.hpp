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

namespace Jet {

//! Shader loader class.
//! @class Shader
//! @brief Shader.
class JETAPI Shader : public Object {
public:
    
    //! Destroys the shader.
    virtual ~Shader() {}
    
    //! Returns true if the shader is loaded.
    inline bool loaded() const {
        return loaded_;
    }
    	
    //! Returns the implementation object.
	inline Object* impl() const {
		return impl_.get();
	}
    
    //! Returns the shader's path.
    const std::string& name() const {
        return name_;
    }
    
    //! Marks the shader as loaded.
    void loaded(bool loaded);
    
    //! Sets the implementation object.
	inline void impl(Object* object) {
		impl_ = object;
	}
    
private:
    //! Creates a shader.
    Shader(Engine* engine, const std::string& name);
    
    Engine* engine_;
    std::string name_;
    bool loaded_;
#pragma warning(disable:4251)
    ObjectPtr impl_;
#pragma warning(default:4251)
    
    friend class Engine;
};

}
