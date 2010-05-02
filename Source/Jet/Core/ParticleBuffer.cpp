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
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Shader.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
  
using namespace Jet;
using namespace std;

Core::ParticleBuffer::ParticleBuffer(Engine* engine, size_t size, size_t buffers) :
	engine_(engine),
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
		if (texture_) {
			texture_->sampler(0);
			glUniform1i(diffuse_map_loc_, 0);
		}
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
			shader_->state(SYNCED);
            shader_->enabled(true);
			
			diffuse_map_loc_ = shader_->uniform_location("diffuse_map");
			time_loc_ = shader_->uniform_location("time");
			scale_loc_ = shader_->uniform_location("scale");
			init_position_attrib_ = shader_->attrib_location("init_position");
			init_velocity_attrib_ = shader_->attrib_location("init_velocity");
			init_time_attrib_ = shader_->attrib_location("init_time");
			init_size_attrib_ = shader_->attrib_location("init_size");
			init_rotation_attrib_ = shader_->attrib_location("init_rotation");
			life_attrib_ = shader_->attrib_location("life");
			growth_rate_attrib_ = shader->attrib_location("growth_rate");

		} else {
			diffuse_map_loc_ = -1;
			time_loc_ = -1;
			scale_loc_ = -1;
			init_position_attrib_ = -1;
			init_velocity_attrib_ = -1;
			init_time_attrib_ = -1;
			init_size_attrib_ = -1;
			life_attrib_ = -1;
			growth_rate_attrib_ = -1;
		}
    }
}

void Core::ParticleBuffer::flush() {    
    // If there is no texture, quit.
    if (!texture_) {
        particle_.clear();
        return;
    }
	if (particle_.size() <= 0) {
		return;
	}
	
	glPointSize(5000.0f);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SPRITE);
	glDepthMask(GL_FALSE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); USE THIS FOR SMOKE
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUniform1f(time_loc_, engine_->frame_time());
	glUniform1f(scale_loc_, engine_->option<float>("display_height"));
    
    // Bind and enable the vertex buffer.  Use the current buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_[current_buffer_]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particle_.size()*sizeof(Particle), &particle_.front());    
    glEnableVertexAttribArray(init_position_attrib_);
	glEnableVertexAttribArray(init_velocity_attrib_);
	glEnableVertexAttribArray(init_time_attrib_);
	glEnableVertexAttribArray(init_size_attrib_);
	glEnableVertexAttribArray(init_rotation_attrib_);
	glEnableVertexAttribArray(life_attrib_);
	glEnableVertexAttribArray(growth_rate_attrib_);
	glVertexAttribPointer(init_position_attrib_, 3, GL_FLOAT, 1, sizeof(Particle), (void*)0);
	glVertexAttribPointer(init_velocity_attrib_, 3, GL_FLOAT, 1, sizeof(Particle), (void*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(init_time_attrib_, 1, GL_FLOAT, 1, sizeof(Particle), (void*)(6*sizeof(GLfloat)));
	glVertexAttribPointer(init_size_attrib_, 1, GL_FLOAT, 1, sizeof(Particle), (void*)(7*sizeof(GLfloat)));
	glVertexAttribPointer(init_rotation_attrib_, 1, GL_FLOAT, 1, sizeof(Particle), (void*)(8*sizeof(GLfloat)));
	glVertexAttribPointer(life_attrib_, 1, GL_FLOAT, 1, sizeof(Particle), (void*)(9*sizeof(GLfloat)));
	glVertexAttribPointer(growth_rate_attrib_, 1, GL_FLOAT, 1, sizeof(Particle), (void*)(10*sizeof(GLfloat)));
   
    // Enable the particle texture and draw the particles
    glDrawArrays(GL_POINTS, 0, particle_.size());
    
    // Disable the vertex buffer
	glDisableVertexAttribArray(init_position_attrib_);
	glDisableVertexAttribArray(init_velocity_attrib_);
	glDisableVertexAttribArray(init_time_attrib_);
	glDisableVertexAttribArray(init_size_attrib_);
	glDisableVertexAttribArray(init_rotation_attrib_);
	glDisableVertexAttribArray(life_attrib_);
	glDisableVertexAttribArray(growth_rate_attrib_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Reset blend states and depth buffer
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE);
	glDepthMask(GL_TRUE);
    
    particle_.clear();
    
    // Rotate the buffers so that we can pipeline updates to the underlying
    // hardware buffer
    current_buffer_ = (current_buffer_ + 1) % vbuffer_.size();
}   