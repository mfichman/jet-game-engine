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

#include <Jet/Graphics/OpenGLRenderTarget.hpp>
#include <stdexcept>
#include <iostream>

using namespace Jet;
using namespace std;

OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, bool depth_only) :
	depth_buffer_(0),
    width_(width),
    height_(height),
	enabled_(false),
	depth_only_(depth_only) {

	GLenum component = depth_only ? GL_DEPTH_COMPONENT : GL_RGBA;

	// Generate the framebuffer object
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    // Initialize the texture
    glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	//Set texture sampling parameters; we use simple filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, component, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (depth_only) {
		// Depth buffer texture only
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else {
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(1, buffers);

        // Color buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

		// Depth buffer
        glGenRenderbuffers(1, &depth_buffer_);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_);
	}

    //! Check for support
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		throw runtime_error("Invalid framebuffer configuration");
	}
    
    // Switch to the back buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
}
	

//! Destructor.
OpenGLRenderTarget::~OpenGLRenderTarget() {
    glDeleteTextures(1, &texture_);
    glDeleteFramebuffers(1, &framebuffer_);
	if (!depth_only_) {
		glDeleteRenderbuffers(1, &depth_buffer_);
	}
}

//! Enables this render target.
void OpenGLRenderTarget::enabled(bool enabled) {
	if (enabled == enabled_) {
		return;
	}

	if (enabled) {
        // Bind the framebuffer, and set the viewport
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
		glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width_, height_);

		if (depth_only_) {
			glDrawBuffer(GL_NONE);
			glClear(GL_DEPTH_BUFFER_BIT);
		} else {
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}
	} else {
		glPopAttrib();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	enabled_ = enabled;
}

void OpenGLRenderTarget::sampler(uint32_t sampler) {
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, texture_);
}
