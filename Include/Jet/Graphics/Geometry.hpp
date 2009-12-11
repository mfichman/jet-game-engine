/*
 * Copyright (c) 2008 Matt Fichman
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
#include <Jet/Physics/Object.hpp>
#include <Jet/Graphics/Renderable.hpp>
#include <Jet/Graphics/Texture.hpp>
#include <Jet/Graphics/Shader.hpp>

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;
using namespace Physics;

class Geometry : public Renderable {
public:
    typedef intrusive_ptr<Geometry> Ptr;

    Vector          scale() const { return scale_; }
    void            scale(const Vector& s) { scale_ = s; }
    Texture::Ptr 	texture(Texture::Index i) const { return texture_[i]; }
    void 		    texture(Texture::Index i, Texture::Ptr t) { texture_[i] = t; }
    Shader::Ptr 	shader() const { return shader_; }
    void 		    shader(Shader::Ptr s) { shader_ = s; }
    Object::Ptr     object() const { return parent_; }
    void            object(Object::Ptr b) { parent_ = b; }
  
    
protected:
    Geometry() {}
    Object::Ptr     parent_;
    Vector          scale_;
	Texture::Ptr    texture_[Texture::Index::maxValue];
    Shader::Ptr     shader_;
};

}}
