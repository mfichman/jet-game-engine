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

#include <Jet/Graphics/OpenGLTypes.hpp>
#include <Jet/Graphics/OpenGLRenderTarget.hpp>
#include <Jet/Graphics/OpenGLParticleBuffer.hpp>
#include <Jet/Graphics/OpenGLShader.hpp>
#include <Jet/Graphics/OpenGLFont.hpp>
#include <Jet/Graphics/OpenGLMaterial.hpp>
#include <Jet/Graphics/OpenGLTexture.hpp>
#include <Jet/Graphics/OpenGLCubemap.hpp>
#include <Jet/Graphics/OpenGLMesh.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreMeshObject.hpp>
#include <Jet/Graphics.hpp>
#include <vector>

namespace Jet {

//! Graphics, uses OpenGL to traverse the scene graph and display visible
//! elements.  Handles shadows, bump-mapping, specular mapping, particle
//! systems, etc.
//! @class Graphics
//! @brief Renders visible objects.
class OpenGLGraphics : public Graphics, public EngineListener {
public:
    //! Constructor.
    OpenGLGraphics(CoreEngine* engine);

    //! Destructor.
    virtual ~OpenGLGraphics();

private:
    inline OpenGLShader* shader(const std::string& name) {
        return new OpenGLShader(engine_, name);
    }
    
    inline OpenGLFont* font(const std::string& name) {
        return new OpenGLFont(engine_, name);
    }
    
    inline OpenGLMaterial* material(const std::string& name) {
        return new OpenGLMaterial(engine_, name);
    }
    
    inline OpenGLTexture* texture(const std::string& name) {
        return new OpenGLTexture(engine_, name);
    }
    
    inline OpenGLCubemap* cubemap(const std::string& name) {
        return new OpenGLCubemap(engine_, name);
    }
    
    inline OpenGLMesh* mesh(const std::string& name) {
        return new OpenGLMesh(engine_, name);
    }
    
    inline OpenGLMesh* mesh(const std::string& name, Mesh* parent) {
        return new OpenGLMesh(engine_, name, static_cast<OpenGLMesh*>(parent));
    }
    
    void on_tick() {}
    void on_init();
    void on_update() {}
    void on_render();
    
    void init_default_states();
    void init_window();
    void init_extensions();
    void init_shadow_target();                                                                         
    
    void generate_render_list(CoreNode* node);
    void generate_shadow_map(CoreLight* light);
    void render_final(CoreLight* light);
    void render_shadow_casters();
    void render_visible_mesh_objects();
    void render_visible_particle_systems();
	void render_visible_quad_sets();
    void render_fullscreen_quad();
    void render_overlays();
    void render_overlay(CoreOverlay* overlay);
    void render_skysphere();
    void check_video_mode();
    
    static bool compare_mesh_objects(MeshObjectPtr o1, MeshObjectPtr o2);
    static bool compare_particle_systems(ParticleSystemPtr o1, ParticleSystemPtr o2);
    static bool compare_quad_sets(QuadSetPtr o1, QuadSetPtr o2);
    CoreEngine* engine_;
    
    // Shadow-mapping variables
    std::vector<OpenGLRenderTargetPtr> shadow_target_;
    OpenGLParticleBufferPtr particle_buffer_;
    
    std::vector<CoreMeshObjectPtr> mesh_objects_;
    std::vector<CoreParticleSystemPtr> particle_systems_;
    std::vector<CoreLightPtr> lights_;
	std::vector<CoreQuadSetPtr> quad_sets_;
};

}
