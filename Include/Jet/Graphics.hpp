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

//! Interface for render engine subsystems
//! @class Graphics
//! @brief Graphics subsystem
class Graphics : public virtual Object {
public:
    //! Creates a new shader with the given name
    virtual Shader* shader(const std::string& name)=0;
    
    //! Creates a new font with the given name
    virtual Font* font(const std::string& font)=0;
    
    //! Creates a new material with the given name
    virtual Material* material(const std::string& material)=0;
    
    //! Creates a new texture with the given name
    virtual Texture* texture(const std::string& texture)=0;
    
    //! Creates a new texture with the given name
    virtual Cubemap* cubemap(const std::string& cubemap)=0;
    
    //! Creates a new mesh with the given name
    virtual Mesh* mesh(const std::string& mesh)=0;
    
    //! Creates a new mesh with the given name and parent
    virtual Mesh* mesh(const std::string& name, Mesh* parent)=0;
};

}
