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
	inline Material(Engine* engine, const std::string& name) :
		engine_(engine),
		name_(name),
		state_(UNLOADED),
		shininess_(40.0f),
		enabled_(false),
		receive_shadows_(true),
		double_sided_(false),
		diffuse_map_loc_(-1),
		specular_map_loc_(-1),
		normal_map_loc_(-1),
		shadow_map_loc_(-1),
		shadow_distance_loc_(-1),
		diffuse_map_enabled_(-1),
		specular_map_enabled_(-1),
		normal_map_enabled_(-1),
		shadow_map_enabled_(-1) {
			
		shader("Default");
	}
    
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
	inline float shininess() const {
		return shininess_;
	}
	
	//! Returns true if the material receives shadows.
	inline bool receive_shadows() const {
		return receive_shadows_;
	}
	
	//! REturns true if the material is double-sided.
	inline bool double_sided() const {
		return double_sided_;
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
    void shader(Jet::Shader* shader);
	
	//! Sets the specular shininess.
	inline void shininess(float shininess) {
		shininess_ = shininess;
	}
	
	//! True if the material receives shadows.
	inline void receive_shadows(bool receive) {
		receive_shadows_ = receive;
	}
	
	//! True if the material is double-sided.
	inline void double_sided(bool double_sided) {
		double_sided_ = double_sided;
	}
    
    //! Sets the diffuse texture map by name.
    //! @param name texture map name
    inline void diffuse_map(const std::string& name) {
		diffuse_map(engine_->texture(name));
	}
    
    //! Sets the specular texture map by name.
    //! @param name texture map name
    inline void specular_map(const std::string& name) {
		specular_map(engine_->texture(name));
	}
    
    //! Sets the normal map by name.
    //! @param name the normal map name
    inline void normal_map(const std::string& name) {
		normal_map(engine_->texture(name));
	}
    
    //! Sets the shader by name.  If the shader is null, then the material will
    //! be rendered using the default shader or fixed function pipeline.
    //! @param name the shader name
    inline void shader(const std::string& name) {
		shader(engine_->shader(name));
	}
	
	//! Sets the resource state.
	void state(ResourceState state);
	
	//! Binds this material.
	void enabled(bool enabled);
    
private:	
	void read_material_data();
	void begin_shader();
	void begin_fixed_pipeline();
    
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
	float shininess_;
	bool enabled_;
	bool receive_shadows_;
	bool double_sided_;
	
	
	// Shader uniform variable handles
	int32_t diffuse_map_loc_;
	int32_t specular_map_loc_;
	int32_t normal_map_loc_;
	int32_t shadow_map_loc_;
	int32_t shadow_distance_loc_;
	
	int32_t diffuse_map_enabled_;
	int32_t specular_map_enabled_;
	int32_t normal_map_enabled_;
	int32_t shadow_map_enabled_;
};

}}
