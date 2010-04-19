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
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Shader.hpp>
#include <Jet/Material.hpp>

namespace Jet { namespace Core {

//! Specifies the color and texture properties of an object.
//! @class Material
//! @brief Specifies the color and texture properties of an object.
class Material : public Jet::Material {
public:
    
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
	
	//! Returns the specular shininess.
	inline real_t shininess() const {
		return shininess_;
	}
	
	//! Returns true if the material receives shadows.
	inline bool receive_shadows() const {
		return receive_shadows_;
	}
	
	//! Returns the resource state.
	inline ResourceState state() const {
		return state_;
	}
	
	//! Returns true if the material is enabled.
	inline bool enabled() const {
		return enabled_;
	}

	//! Returns the name.
	inline const std::string& name() const {
		return name_;
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
    inline void diffuse_map(Jet::Texture* texture) {
        diffuse_map_ = static_cast<Texture*>(texture);
    }
    
    //! Sets the specular texture map.
    //! @param texture the specular texture map
    inline void specular_map(Jet::Texture* texture) {
        specular_map_ = static_cast<Texture*>(texture);
    }
    
    //! Sets the normal texture map.
    //! @param texture the normal texture map
    inline void normal_map(Jet::Texture* texture) {
        normal_map_ = static_cast<Texture*>(texture);
    }
    
    //! Sets the shader used to render this material.  If the shader is null,
    //! then the material will be rendered using the default shader or fixed
    //! function pipeline.
    //! @param shader the shader
    inline void shader(Jet::Shader* shader) {
        shader_ = static_cast<Shader*>(shader);
		if (!shader_) {
			// If the shader was set to null, turn on the default shader
			Material::shader("Default");
		}
    }
	
	//! Sets the specular shininess.
	inline void shininess(real_t shininess) {
		shininess_ = shininess;
	}
	
	//! True if the material receives shadows.
	inline void receive_shadows(bool receive) {
		receive_shadows_ = receive;
	}
    
    //! Sets the diffuse texture map by name.
    //! @param name texture map name
    inline void diffuse_map(const std::string& name) {
		diffuse_map_ = static_cast<Texture*>(engine_->texture(name));
	}
    
    //! Sets the specular texture map by name.
    //! @param name texture map name
    inline void specular_map(const std::string& name) {
		specular_map_ = static_cast<Texture*>(engine_->texture(name));
	}
    
    //! Sets the normal map by name.
    //! @param name the normal map name
    inline void normal_map(const std::string& name) {
		normal_map_ = static_cast<Texture*>(engine_->texture(name));
	}
    
    //! Sets the shader by name.  If the shader is null, then the material will
    //! be rendered using the default shader or fixed function pipeline.
    //! @param name the shader name
    inline void shader(const std::string& name) {
		shader_ = static_cast<Shader*>(engine_->shader(name));
	}
	
	//! Sets the resource state.
	void state(ResourceState state);
	
	//! Binds this material.
	void enabled(bool enabled);
    
private:
    inline Material(Engine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(UNLOADED),
		enabled_(false),
		shininess_(40.0f),
		receive_shadows_(true) {
			
		shader("Default");
	}
	
	void read_material_data();
    
    Engine* engine_;
    std::string name_;
	ResourceState state_;
    Color ambient_color_;
    Color diffuse_color_;
    Color specular_color_;
    TexturePtr diffuse_map_;
    TexturePtr specular_map_;
    TexturePtr normal_map_;
    ShaderPtr shader_;
	real_t shininess_;
	bool enabled_;
	bool receive_shadows_;
    
    friend class Engine;
};

}}