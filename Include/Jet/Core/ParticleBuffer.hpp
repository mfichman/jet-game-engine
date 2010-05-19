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

#include <Jet/Core/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Particle.hpp>
#include <vector>

namespace Jet { namespace Core {

//! This class is used to buffer particles for display to the screen.  This
//! aids performance, because all particles can be written at once, even if
//! they belong to different particle systems.
//! @class ParticleBuffer
//! @brief Stores particles for rendering.
class ParticleBuffer : public Object {
public:
    //! Constructor.  Creates a new particle buffer of the given size.
    //! When the particle buffer is full, it will be flushed
    //! @param engine the engine object
	//! @param size the maximum number of particles in each draw batch
    //! @param buffers the number of hardware buffers to use (for pipelining)
    ParticleBuffer(Engine* engine, size_t size=4096, size_t buffers=2);
    
    //! Destructor.
    ~ParticleBuffer();
    
    //! Adds a particle to this buffer.  If the particle can't fit, then the
    //! particle buffer is flushed and the contents will be drawn.
    void particle(const Particle& particle);
    
    //! Sets the texture used by the particle buffer.  If the texture
    //! changes and there are particle in the buffer, then the particles will
    //! be flushed and drawn.
    void texture(Texture* texture);
    
    //! Sets the shader used by this particle buffer.
    void shader(Shader* shader);
    
    //! Flushes particles that are currently contained in the buffer
    void flush();
    
private:
    
	Engine* engine_;
    TexturePtr texture_;
    ShaderPtr shader_;
    std::vector<Particle> particle_;
    std::vector<uint32_t> vbuffer_;
    size_t size_;
    size_t current_buffer_;
	int32_t diffuse_map_loc_;
	int32_t time_loc_;
	int32_t scale_loc_;
	int32_t init_position_attrib_;
	int32_t init_velocity_attrib_;
	int32_t init_time_attrib_;
	int32_t init_size_attrib_;
	int32_t init_rotation_attrib_;
	int32_t life_attrib_;
	int32_t growth_rate_attrib_;
    int32_t drag_coefficient_attrib_;
};

}}