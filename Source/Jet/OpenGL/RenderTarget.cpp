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

#include <Jet/OpenGL/RenderTarget.hpp>

using namespace Jet::OpenGL;
using namespace Jet;
using namespace std;

RenderTarget::RenderTarget(GLuint width, GLuint height, GLenum component) :
    component_(component),
    width_(width),
    height_(height) {
        
    // Initialize the texture
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
     
    //Set texture sampling parameters; we use simple filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, component_, width, height, 0, component_, GL_UNSIGNED_BYTE, 0);
  
    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Generate the framebuffer object
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    if (GL_DEPTH_COMPONENT == component_) {
        glDrawBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_, 0);
    } else {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);
    }
    
    //! Check for support
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        throw runtime_error("Framebuffer configuaration failed");
    }
    
    // Switch to the back buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (GL_DEPTH_COMPONENT == component_) {
        glDrawBuffer(GL_BACK);
    }
}

//! Destructor.
RenderTarget::~RenderTarget() {
    glDeleteTextures(1, &texture_);
    glDeleteFramebuffers(1, &framebuffer_);
}

//! Enables this render target.
void RenderTarget::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glViewport(0, 0, width_, height_);
    if (GL_DEPTH_COMPONENT == component_) {
        glDrawBuffer(GL_NONE);
        glClear(GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }
}

//! Disables this render target.
void RenderTarget::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (GL_DEPTH_COMPONENT == component_) {
        glDrawBuffer(GL_BACK);
    }
}