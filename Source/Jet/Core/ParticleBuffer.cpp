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

#include <Jet/Core/ParticleBuffer.hpp>
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Shader.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
  
using namespace Jet;
  
Core::ParticleBuffer::ParticleBuffer(size_t size, size_t buffers) :
    size_(size),
    current_buffer_(0) {
        
    vbuffer_.resize(buffers);
        
    glGenBuffers(vbuffer_.size(), &vbuffer_.front());
    for (size_t i = 0; i < vbuffer_.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vbuffer_[i]);
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(Particle), 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

Core::ParticleBuffer::~ParticleBuffer() {
    glDeleteBuffers(vbuffer_.size(), &vbuffer_.front());
}

void Core::ParticleBuffer::particle(const Particle& particle) {
    particle_.push_back(particle);
    if (particle_.size() >= size_) {
        flush();
    }
}
 
void Core::ParticleBuffer::texture(Texture* texture) {
    if (texture_ != texture) {
        // If any particles were using the previous texture, then flush them
        // to the graphics card.  We can only use 1 texture per draw call,
        // so we have to split draw calls across the different textures.
        if (particle_.size() > 0) {
            flush();
        }
        
        texture_ = texture;
        texture_->sampler(0);
    }
}

void Core::ParticleBuffer::shader(Shader* shader) {
    if (shader_ != shader) {
        
        if (particle_.size() > 0) {
            flush();
        }
        if (shader_) {
            shader_->enabled(false);
        }
        shader_ = shader;
        if (shader_) {
            
            shader_->enabled(true);
        }
    }
}

void Core::ParticleBuffer::flush() {    
    // If there is no texture, quit.
    if (!texture_) {
        particle_.clear();
        return;
    }
    glEnable(GL_TEXTURE_2D);
    
    // Bind and enable the vertex buffer.  Use the current buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_[current_buffer_]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particle_.size()*sizeof(Particle), &particle_.front());    
    glEnableClientState(GL_VERTEX_ARRAY);
   
    // Enable the particle texture and draw the particles
    glVertexPointer(3, GL_FLOAT, sizeof(Particle), 0);
    glDrawArrays(GL_POINTS, 0, particle_.size());
    glDisable(GL_TEXTURE_2D);
    
    // Disable the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    particle_.clear();
    
    // Rotate the buffers so that we can pipeline updates to the underlying
    // hardware buffer
    current_buffer_ = (current_buffer_ + 1) % vbuffer_.size();
}   