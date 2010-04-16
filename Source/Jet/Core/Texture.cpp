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
#include <IL/IL.h>
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
		// Load the image data
		string file = engine_->resource_path(name_);
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(file.c_str());
		
		//if (ilGetError()) {
		//	throw runtime_error("Error loading image: " + name_);
		//}
		
		width(ilGetInteger(IL_IMAGE_WIDTH));
		height(ilGetInteger(IL_IMAGE_HEIGHT));
		ilCopyPixels(0, 0, 0, width(), height(), 1, IL_RGBA, IL_UNSIGNED_BYTE, data());
		ilDeleteImages(1, &image);
	}
	
	// Entering the SYNCED state
	if (SYNCED == state) {
		// Initialize the texture
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		
		//! Set texture sampling parameters; we use mip filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0);
		
		// Load the image
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	// Leaving the SYNCED state
	if (SYNCED == state_) {
		glDeleteTextures(1, &texture_);
		texture_ = 0;
	}
	
	// Entering the UNLOADED state
	if (UNLOADED == state) {
		data_.clear();
	}
	
	state_ = state;
}

void Core::Texture::sampler(TextureSampler sampler) {
	state(SYNCED);
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glEnable(GL_TEXTURE_2D);
}