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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Color.hpp>
#include <Jet/Shader.hpp>

namespace Jet {

//! Specifies the color and texture properties of an object.
//! @class Material
//! @brief Specifies the color and texture properties of an object.
class JETAPI Material : public Object {
public:
    //! Destructor.
    virtual ~Material() {}
    
    //! Returns the ambient color.
    inline const Color& ambient_color() const {
        return ambient_color_;
    }
    
    //! Returns the diffuse color.
    inline const Color& diffuse_color() const {
        return diffuse_color_;
    }
    
    //! Returns the specular color.
    inline const Color& specular_color() const {
        return specular_color_;
    }
    
    //! Returns the diffuse texture map.
    inline Texture* diffuse_map() const {
        return diffuse_map_.get();
    }
    
    //! Returns the specular texture map.
    inline Texture* specular_map() const {
        return specular_map_.get();
    }
    
    //! Returns the normal map name.
    inline Texture* normal_map() const {
        return normal_map_.get();
    }
    
    //! Returns the shader.  If the default shader or fixed function pipeline
    //! is being used, this value will be null.
    inline Shader* shader() const {
        return shader_.get();
    }
    
    //! Returns true if the material is loaded.
    inline bool loaded() const {
        return loaded_;
    }

    //! Sets the ambient color of the material.
    //! @param color the ambient color
    inline void ambient_color(const Color& color) {
        ambient_color_ = color;
    }
    
    //! Sets the diffuse color of the material.
    //!! @param color the diffuse color
    inline void diffuse_color(const Color& color) {
        diffuse_color_ = color;
    }
    
    //! Sets the specular color of the material.
    //! @param color the specular color
    inline void specular_color(const Color& color) {
        specular_color_ = color;
    }
    
    //! Sets the diffuse texture map.
    //! @param texture the diffuse texture map
    inline void diffuse_map(Texture* texture) {
        diffuse_map_ = texture;
    }
    
    //! Sets the specular texture map.
    //! @param texture the specular texture map
    inline void specular_map(Texture* texture) {
        specular_map_ = texture;
    }
    
    //! Sets the normal texture map.
    //! @param texture the normal texture map
    inline void normal_map(Texture* texture) {
        normal_map_ = texture;
    }
    
    //! Sets the shader used to render this material.  If the shader is null,
    //! then the material will be rendered using the default shader or fixed
    //! function pipeline.
    //! @param shader the shader
    inline void shader(Shader* shader) {
        shader_ = shader;
    }
    
    //! Sets the diffuse texture map by name.
    //! @param name texture map name
    void diffuse_map(const std::string& name);
    
    //! Sets the specular texture map by name.
    //! @param name texture map name
    void specular_map(const std::string& name);
    
    //! Sets the normal map by name.
    //! @param name the normal map name
    void normal_map(const std::string& name);
    
    //! Sets the shader by name.  If the shader is null, then the material will
    //! be rendered using the default shader or fixed function pipeline.
    //! @param name the shader name
    void shader(const std::string& name);
    
    //! Marks this material as loaded.
    //! @param loaded true if the material is loaded
    void loaded(bool loaded);
    
private:
    Material(Engine* engine, const std::string& name);
    
    Engine* engine_;
    std::string name_;
    Color ambient_color_;
    Color diffuse_color_;
    Color specular_color_;
#pragma warning(disable:4251)
    TexturePtr diffuse_map_;
    TexturePtr specular_map_;
    TexturePtr normal_map_;
    ShaderPtr shader_;
#pragma warning(default:4251)
	bool loaded_;
    
    friend class Engine;
};

}