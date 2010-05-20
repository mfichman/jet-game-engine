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

#include <Jet/OpenGL/Types.hpp>
#include <Jet/OpenGL/RenderTarget.hpp>
#include <Jet/OpenGL/ParticleBuffer.hpp>
#include <Jet/OpenGL/Shader.hpp>
#include <Jet/OpenGL/Font.hpp>
#include <Jet/OpenGL/Material.hpp>
#include <Jet/OpenGL/Texture.hpp>
#include <Jet/OpenGL/Cubemap.hpp>
#include <Jet/OpenGL/Mesh.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/ParticleSystem.hpp>
#include <Jet/Renderer.hpp>
#include <vector>

namespace Jet { namespace OpenGL {

//! Renderer, uses OpenGL to traverse the scene graph and display visible
//! elements.  Handles shadows, bump-mapping, specular mapping, particle
//! systems, etc.
//! @class Renderer
//! @brief Renders visible objects.
class RenderSystem : public Renderer, public EngineListener {
public:
    //! Constructor.
    RenderSystem(Core::Engine* engine);

    //! Destructor.
    virtual ~RenderSystem();

private:
    inline Shader* shader(const std::string& name) {
        return new OpenGL::Shader(engine_, name);
    }
    
    inline Font* font(const std::string& name) {
        return new OpenGL::Font(engine_, name);
    }
    
    inline Material* material(const std::string& name) {
        return new OpenGL::Material(engine_, name);
    }
    
    inline Texture* texture(const std::string& name) {
        return new OpenGL::Texture(engine_, name);
    }
    
    inline Cubemap* cubemap(const std::string& name) {
        return new OpenGL::Cubemap(engine_, name);
    }
    
    inline Mesh* mesh(const std::string& name) {
        return new OpenGL::Mesh(engine_, name);
    }
    
    inline Mesh* mesh(const std::string& name, Jet::Mesh* parent) {
        return new OpenGL::Mesh(engine_, name, static_cast<OpenGL::Mesh*>(parent));
    }
    
    void on_tick() {}
    void on_init();
    void on_update() {}
    void on_render();
    
    void init_default_states();
    void init_window();
    void init_extensions();
    void init_shadow_target();                                                                         
    
    void generate_render_list(Core::Node* node);
    void generate_shadow_map(Core::Light* light);
    void render_final(Core::Light* light);
    void render_shadow_casters();
    void render_visible_mesh_objects();
    void render_visible_particle_systems();
    void render_fullscreen_quad();
    void render_overlays();
    void render_overlay(Core::Overlay* overlay);
    void render_skysphere();
    void check_video_mode();
    
    static bool compare_mesh_objects(MeshObjectPtr o1, MeshObjectPtr o2);
    static bool compare_particle_systems(ParticleSystemPtr o1, ParticleSystemPtr o2);
    
    Core::Engine* engine_;
    
    // Shadow-mapping variables
    std::vector<RenderTargetPtr> shadow_target_;
    
    ParticleBufferPtr particle_buffer_;
    
    std::vector<Core::MeshObjectPtr> mesh_objects_;
    std::vector<Core::ParticleSystemPtr> particle_systems_;
    std::vector<Core::LightPtr> lights_;
};

}}
