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
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/RenderTarget.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/ParticleSystem.hpp>
#include <Jet/Core/ParticleBuffer.hpp>
#include <vector>

namespace Jet { namespace Core {

//! Renderer, uses OpenGL to traverse the scene graph and display visible
//! elements.  Handles shadows, bump-mapping, specular mapping, particle
//! systems, etc.
//! @class Renderer
//! @brief Renders visible objects.
class RenderSystem : public EngineListener {
public:
    //! Constructor.
    RenderSystem(Engine* engine);

    //! Destructor.
    virtual ~RenderSystem();

private:
    void on_tick() {}
    void on_init();
    void on_update();
    void on_render();
    
    void init_default_states();
    void init_window();
    void init_extensions();
    void init_shadow_target();
    
    void generate_render_list(Node* node);
    void generate_shadow_map(Light* light);
    void render_final(Light* light);
    void render_shadow_casters();
    void render_visible_mesh_objects();
    void render_visible_particle_systems();
    void render_fullscreen_quad();
    void render_overlays();
    void check_video_mode();
    
    static bool compare_mesh_objects(MeshObjectPtr o1, MeshObjectPtr o2);
    static bool compare_particle_systems(ParticleSystemPtr o1, ParticleSystemPtr o2);
    
    Engine* engine_;
    
    // Shadow-mapping variables
    std::vector<RenderTargetPtr> shadow_target_;
    RenderTargetPtr color_target_;
    RenderTargetPtr highpass_target_;
    RenderTargetPtr bloom_target1_;
    RenderTargetPtr bloom_target2_;
    
    ParticleBufferPtr particle_buffer_;
    
    std::vector<MeshObjectPtr> mesh_objects_;
    std::vector<ParticleSystemPtr> particle_systems_;
    std::vector<LightPtr> lights_;
    Material* active_material_;
};

}}
