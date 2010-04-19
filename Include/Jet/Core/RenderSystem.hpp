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
    virtual ~RenderSystem() {}

private:
    void on_init();
    void on_update() {}
    void on_post_update();
    void on_render();
    
    void init_default_states();
    void init_window();
    
    void generate_render_list(Node* node);
    void generate_shadow_map(Light* light);
    void render_final(Light* light);
    void render_shadow_casters();
    void render_visible_objects();
    
    Engine* engine_;
    
    // Shadow-mapping variables
    RenderTargetPtr shadow_target_;
    
    std::vector<MeshObjectPtr> mesh_objects_;
    std::vector<LightPtr> lights_;
};

}}