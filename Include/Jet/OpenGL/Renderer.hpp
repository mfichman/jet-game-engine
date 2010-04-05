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
#include <Jet/OpenGL/Shader.hpp>
#include <Jet/OpenGL/RenderTarget.hpp>
#include <Jet/OpenGL/MeshBuffer.hpp>
#include <Jet/OpenGL/TextureBuffer.hpp>
#include <Jet/Types.hpp>
#include <Jet/Handler.hpp>

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <map>
#include <vector>

namespace Jet { namespace OpenGL {

//! Renderer, uses OpenGL to traverse the scene graph and display visible
//! elements.  Handles shadows, bump-mapping, specular mapping, particle
//! systems, etc.
//! @class Renderer
//! @brief Renders visible objects.
class Renderer : public Handler {
public:
    //! Constructor.
    Renderer(Engine* engine);

    //! Destructor.
    virtual ~Renderer() {}

    //! Called when an event occurs.
    //! @param event the event code
    virtual void on_render();
    
    //! Retrieves the given shader.
    //! @param name the shader's name
    Shader* shader(const std::string& name, uint32_t opts);
    
    //! Retrieves the given mesh
    //! @param name the mesh's name
    MeshBuffer* mesh(const std::string& name);
    
    //! Retrieves the given texture buffer.
    //! @param name the texture's name
    TextureBuffer* texture(const std::string& name);
    
    //! Gets the rendering #define for shaders.
    //! @param tag the tag that identifies the #define
    inline const std::string& shader_option(uint32_t tag) const {
        return shader_option_.find(tag)->second;
    }
    
    //! Sets the rendering #define for shaders.
    //! @param tag the shader option tag
    //! @param define the shader define tag
    inline void shader_option(uint32_t tag, const std::string& define) {
        shader_option_[tag] = define;
    }

private:
    
    void init_default_states();
    void init_window();
    
    void render_teapots();
    
    void generate_shadow_map(const std::pair<NodePtr, ComponentPtr>& light);
    void render_final(const std::pair<NodePtr, ComponentPtr>& light);
    void render_shadow_casters();
    void render_visible_objects();
    
    void permute_shaders(const std::string& name);
    
    Engine* engine_;
    
    // Shadow-mapping variables
    ComponentPtr shadow_vars_;
    Matrix shadow_matrix_;
    RenderTargetPtr shadow_target_;

    ComponentPtr window_vars_;
    
    // Shader variables
    std::map<std::pair<std::string, uint32_t>, ShaderPtr> shader_;
    std::map<std::string, MeshBufferPtr> mesh_;
    std::map<std::string, TextureBufferPtr> texture_;
    
    std::map<uint32_t, std::string> shader_option_;
};

}}
