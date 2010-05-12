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

#include <Jet/Core/Material.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Core/MaterialLoader.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>

using namespace Jet;
using namespace std;


void Core::Material::state(ResourceState state) {

	// Leaving the UNLOADED state
	if (UNLOADED == state_) {
		read_material_data();
	}

	state_ = state;
}

void Core::Material::read_material_data() {
	static const std::string ext = ".mtl";
    size_t pos = name_.rfind(ext);
	if (pos == string::npos || (name_.length() - pos) != ext.length()) {    
		return;
    }
	
	// Find the path to the file, and then use the material loader.
	string file = engine_->resource_path(name_);
	MaterialLoader(this, file);
}

void Core::Material::shader(Jet::Shader* shader) {
	shader_ = static_cast<Core::Shader*>(shader);
	if (!shader_) {
		// If the shader was set to null, turn on the default shader
		Material::shader("Default");
	} else if (engine_->option<bool>("shaders_enabled")) {
		shader_->state(SYNCED);
		diffuse_map_loc_ = shader_->uniform_location("diffuse_map");
		specular_map_loc_ = shader_->uniform_location("specular_map");
		normal_map_loc_ = shader_->uniform_location("normal_map");
		shadow_map0_loc_ = shader_->uniform_location("shadow_map0");
		shadow_map1_loc_ = shader_->uniform_location("shadow_map1");
		shadow_map2_loc_ = shader_->uniform_location("shadow_map2");
		shadow_map3_loc_ = shader_->uniform_location("shadow_map3");
		shadow_z_loc_ = shader_->uniform_location("shadow_z");
		diffuse_map_enabled_ = shader_->uniform_location("diffuse_map_enabled");
		specular_map_enabled_ = shader_->uniform_location("specular_map_enabled");
		normal_map_enabled_ = shader_->uniform_location("normal_map_enabled");
		shadow_map_enabled_ = shader_->uniform_location("shadow_map_enabled");
		shadow_distance_loc_ = shader_->uniform_location("shadow_distance");
	}
}

void Core::Material::enabled(bool enabled) {
	if (enabled == enabled_) {
		return;
	}
	
	if (enabled) {
		// Make sure the material is properly loaded into memory
		state(SYNCED);
		
		if (engine_->option<bool>("shaders_enabled")) {
			// Enable the material using the shader.
			begin_shader();
		} else {
			// Enable the material using the fixed pipeline
			begin_fixed_pipeline();		
		}
		
		// Double-sided materials should have BOTH sides of the triangles
		// rendered.  This decreases performance, but is needed for some
		// things (like fracture meshes).
		if (double_sided_) {
			glDisable(GL_CULL_FACE);
		}
		
		// Set up material colors	
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color());
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color());
		glMaterialf(GL_FRONT, GL_SHININESS, shininess());
	} else {
		
		// Reset culling to default
		if (double_sided_) {
			glEnable(GL_CULL_FACE);
		}
		
		if (engine_->option<bool>("shaders_enabled")) {
			// Disable the shader for this material
			shader_->enabled(false);
		} else {
			// Disable texturing
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
		}
	}
	
	enabled_ = enabled;
}

void Core::Material::begin_shader() {
	// Enable material shader
	shader_->enabled(true);
	
	// Set up texture samplers.  Samplers are enabled if the corresponding
	// texture in the material is non-null.
	if (diffuse_map_) {
		diffuse_map_->sampler(DIFFUSE_MAP_SAMPLER);
		glUniform1i(diffuse_map_loc_, DIFFUSE_MAP_SAMPLER);
		glUniform1i(diffuse_map_enabled_, true);
	} else {
		glUniform1i(diffuse_map_enabled_, false);
	}

	// Specular mapping must be enabled to use a specular map in the shader
	if (specular_map_ && engine_->option<bool>("specular_mapping_enabled")) {
		specular_map_->sampler(SPECULAR_MAP_SAMPLER);
		glUniform1i(specular_map_loc_, SPECULAR_MAP_SAMPLER);
		glUniform1i(specular_map_enabled_, true);
	} else {
		glUniform1i(specular_map_enabled_, false);
	}
	
	// Normal mapping must be enabled to use a normal map in the shader
	if (normal_map_ && engine_->option<bool>("normal_mapping_enabled")) {
		normal_map_->sampler(NORMAL_MAP_SAMPLER);
		glUniform1i(normal_map_loc_, NORMAL_MAP_SAMPLER);
		glUniform1i(normal_map_enabled_, true);
	} else {
		glUniform1i(normal_map_enabled_, false);
	}
	
	if (engine_->option<bool>("shadows_enabled")) {
		// Calculate the shadow far z distances for CSM
		Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());
		size_t cascades = (size_t)engine_->option<float>("shadow_cascades");
		float alpha = engine_->option<float>("shadow_correction"); 
		float n = camera->near_clipping_distance();
		float f = min(camera->far_clipping_distance(), engine_->option<float>("shadow_distance"));
		float shadow_z[4];
		for (size_t i = 0; i < cascades; i++) {
			float r = (float)(i+1)/(float)cascades;
			shadow_z[i] = alpha*n*pow(f/n, r) + (1-alpha)*(n+r*(f-n));
		}
		
		// Enable the shadow map sampler only if this object should receive
		// shadows
		glUniform1i(shadow_map_enabled_, (bool)receive_shadows_);		
		glUniform1i(shadow_map0_loc_, SHADOW_MAP_SAMPLER+0);
		glUniform1i(shadow_map1_loc_, SHADOW_MAP_SAMPLER+1);
		glUniform1i(shadow_map2_loc_, SHADOW_MAP_SAMPLER+2);
		glUniform1i(shadow_map3_loc_, SHADOW_MAP_SAMPLER+3);
		glUniform1fv(shadow_z_loc_,  4, shadow_z);
		glUniform1f(shadow_distance_loc_, engine_->option<float>("shadow_distance"));
	} else {
		glUniform1i(shadow_map_enabled_, false);
	}
}

void Core::Material::begin_fixed_pipeline() {
	// Disable all textures except for the texture we need for
	// the diffuse texture map.
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glDisable(GL_TEXTURE_2D);
	
	// Only use diffuse texture mapping
	if (diffuse_map_) {
		diffuse_map_->sampler(0);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	} else {
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

