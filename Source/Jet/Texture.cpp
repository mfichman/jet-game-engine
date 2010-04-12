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

#include <Jet/Texture.hpp>
#include <Jet/Engine.hpp>

using namespace Jet;

Texture::Texture(Engine* engine, const std::string& name) :
	engine_(engine),
    name_(name),
    loaded_(false),
	width_(0), 
	height_(0) {
}

void Texture::width(size_t width) {
    width_ = width;
    data_.resize(width_ * height_ * 4);
}

void Texture::height(size_t height) {
    height_ = height;
    data_.resize(width_ * height_ * 4);
}

void Texture::loaded(bool loaded) {
	if (loaded_ != loaded) {
		if (loaded) {
			engine_->resource(name_);
		}
		loaded_ = loaded;
	}
}