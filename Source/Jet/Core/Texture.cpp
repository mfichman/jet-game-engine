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

#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Engine.hpp>
#include <SDL/SDL_image.h>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdexcept>

using namespace Jet;
using namespace std;

Core::Texture::~Texture() {
	state(UNLOADED);
}

void Core::Texture::state(ResourceState state) {
	if (state == state_) {
		return;
	}
	
	// Leaving the UNLOADED state
	if (UNLOADED == state_) {
		read_texture_data();
	}
	
	// Entering the LOADED state
	if (LOADED == state) {
		init_texture();
	}
	
	// Leaving the LOADED state
	if (LOADED == state_) {
		glDeleteTextures(1, &texture_);
		texture_ = 0;
	}
	
	// Entering the UNLOADED state
	if (UNLOADED == state) {
		data_.clear();
	}
	
	state_ = state;
}

void Core::Texture::read_texture_data() {
	// Load the image data
	string file = engine_->resource_path(name_);
	SDL_Surface* surface = IMG_Load(file.c_str());
	
	// Check to make sure the image was supported
	if (!surface) {
		throw runtime_error("Unsupported image format: " + name_);
	}
	
	// Check image format
	bytes_per_pixel_ = surface->format->BytesPerPixel;
	if (4 == surface->format->BytesPerPixel) {
		if (0xff == surface->format->Rmask) {
			texture_format_ = GL_RGBA;
		} else {
			texture_format_ = GL_BGRA;
		}
	} else if (3 == surface->format->BytesPerPixel) {
		if (0xff == surface->format->Rmask) {
			texture_format_ = GL_RGB;
		} else {
			texture_format_ = GL_BGR;
		}
	} else {
		SDL_FreeSurface(surface);
		throw runtime_error("Invalid image format: " + name_);
	}

	// Set the width and height of the texture, then copy all the texture data
	width(surface->w);
	height(surface->h);
	memcpy(data(), surface->pixels, data_.size());

	SDL_FreeSurface(surface);
}

void Core::Texture::init_texture() {
	
	// Initialize the texture
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	
	//! Set texture sampling parameters; we use mip filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0);
	
	// Load the image
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width(), height(), texture_format_, GL_UNSIGNED_BYTE, data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Core::Texture::sampler(uint32_t sampler) {
	state(LOADED);
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, texture_);
}
