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

#include <Jet/Core/RenderSystem.hpp>
#include <Jet/Core/RenderTarget.hpp>
#include <Jet/Core/Light.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Matrix.hpp>

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace Jet;
using namespace std;
using namespace boost;

Core::RenderSystem::RenderSystem(Engine* engine) :
    engine_(engine) {
}

void Core::RenderSystem::init_window() {
    int32_t width = (int32_t)any_cast<real_t>(engine_->option("display_width"));
    int32_t height = (int32_t)any_cast<real_t>(engine_->option("display_height"));
    string title = any_cast<string>(engine_->option("window_title"));
	bool fullscreen = any_cast<bool>(engine_->option("fullscreen"));
	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(string("SDL initialization failed: ") + SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	uint32_t flags = SDL_OPENGL;
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	
	if (!SDL_SetVideoMode(width, height, 0, flags)) {
		throw runtime_error(string("SDL initialization failed: ") + SDL_GetError());
	}

	// Initialize extensions	
	if (GLEW_OK != glewInit()) {
        throw runtime_error("GLEW initialization failed");
    }
    if (!glewIsSupported("GL_VERSION_2_0")) {
        throw runtime_error("OpenGL 2.0 not supported");
    }
}

void Core::RenderSystem::init_default_states() {
    // Initialize some common s
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // We always use CCW culling
    glCullFace(GL_BACK);
    
    GLfloat width = any_cast<real_t>(engine_->option("display_width"));
    GLfloat height = any_cast<real_t>(engine_->option("display_height"));
    glViewport(0, 0, (uint32_t)width, (uint32_t)height);
    
	
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, width/height, 0.1f, 1000.0f);
    
    // Turn of VSYNC so the game can run at full frame rate
#ifdef WINDOWS
    //typedef int (APIENTRY *swap_interval_t)(int);
    //swap_interval_t glSwapInterval = (swap_interval_t)wglGetProcAddress("wglSwapIntervalEXT");
    //glSwapInterval(0);
#endif
}

void Core::RenderSystem::on_init() {
	init_window();
    init_default_states();
	
	GLuint width = (GLuint)any_cast<real_t>(engine_->option("display_width"));
	GLuint height = (GLuint)any_cast<real_t>(engine_->option("display_height"));

	GLuint size = (GLuint)any_cast<real_t>(engine_->option("shadow_texture_size"));
    shadow_target_.reset(new RenderTarget(size, size, true, 1));
    color_target_.reset(new RenderTarget(width, height, false, 2));
	bloom_target1_.reset(new RenderTarget(width/8, height/8, false, 1));
	bloom_target2_.reset(new RenderTarget(width/8, height/8, false, 1));
}

void Core::RenderSystem::on_render() {	
	if (!engine_->camera()) {
		return;
	}
    
    // Render the scene once for each light
    for (vector<LightPtr>::iterator i = lights_.begin(); i != lights_.end(); i++) {
        generate_shadow_map(i->get());
        render_final(i->get());
		// TODO: render more than one light
        break;
    }
	
	SDL_GL_SwapBuffers();
}

void Core::RenderSystem::on_post_update() {
	mesh_objects_.clear();
	lights_.clear();
	
	generate_render_list(static_cast<Core::Node*>(engine_->root()));
}

void Core::RenderSystem::generate_shadow_map(Light* light) {    

    // Set up the projection matrix for the light
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-12.0f, 12.0f, -12.0f, 12.0f, 0.1f, 1000.0f);
    
    // Set up the view matrix for the light
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (light->type() == POINT_LIGHT) {
        const Vector& position = light->parent()->position();
        gluLookAt(position.x, position.y, position.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    } else {
        const Vector& direction = light->direction();
        gluLookAt(100*direction.x, 100*direction.y, 100*direction.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }    
    
    // Render to the front buffer
    shadow_target_->enabled(true);
    glCullFace(GL_FRONT); // Only render the back faces to the depth buffer
    glDisable(GL_LIGHTING);
    render_shadow_casters();
    shadow_target_->enabled(false);
    glCullFace(GL_BACK); 
    glEnable(GL_LIGHTING);
        
    // Initialize the texture matrix for transforming the shadow map
    // coordinates
    Matrix light_bias(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    Matrix light_projection;
    Matrix light_modelview;
    glGetFloatv(GL_PROJECTION_MATRIX, light_projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, light_modelview);
    glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(light_bias);
    glMultMatrixf(light_projection);
    glMultMatrixf(light_modelview);
    
    // Pop the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void Core::RenderSystem::render_final(Light* light) {
    
    // Modelview matrix setup
	Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());
	const Matrix& matrix = camera->parent()->matrix();
	Vector eye = matrix * Vector(0.0f, 0.0f, 0.0f);
	Vector at = matrix * Vector(0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, 0.0f, 1.0f, 0.0f);
        
    // Set light parameter
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light->diffuse_color());
    glLightfv(GL_LIGHT0, GL_SPECULAR, light->specular_color());
    glLightfv(GL_LIGHT0, GL_AMBIENT, light->ambient_color());
    
    // Set light location/direction.
	if (light->type() == POINT_LIGHT) {
        const Vector& position = light->parent()->position();
        GLfloat lposition[4] = { position.x, position.y, position.z, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    } else {
        const Vector& direction = light->direction();
        GLfloat lposition[4] = { direction.x, direction.y, direction.z, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lposition);
	}
		
	// Bind the shadow sampler to the shadow texture
	shadow_target_->sampler(SHADOW_MAP_SAMPLER);
    
    // Render to the back buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_visible_objects();
	

	/* TODO: Finish gaussian filter
	// Render the scene to the highpass and color buffers
	color_target_->enabled(true);
    render_visible_objects();
	color_target_->enabled(false);

	// Clear the texture matrix back to the identity
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE0);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//color_target_->sampler(0, 0);
	color_target_->sampler(0, 1);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	render_fullscreen_quad();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);*/
}


void Core::RenderSystem::generate_render_list(Core::Node* node) {
	
	if (node->parent()) {
		node->matrix(node->parent()->matrix() * Matrix(node->rotation(), node->position()));
	} else {
		node->matrix(Matrix(node->rotation(), node->position()));
	}
    
	node->update();

	// Iterate through all sub objects and add them to the appropriate
	// render list as necessary
    for (Iterator<ObjectPtr> i = node->objects(); i; i++) {
        const type_info& type = typeid(**i);
        if (typeid(Core::Node) == type) {
            generate_render_list(static_cast<Core::Node*>(i->get()));
        } else if (typeid(Core::MeshObject) == type) {
			Core::MeshObject* mesh_object = static_cast<Core::MeshObject*>(i->get());
			if (mesh_object->material() && mesh_object->mesh()) {
				mesh_objects_.push_back(mesh_object);
			}
        } else if (typeid(Core::Light) == type) {
			lights_.push_back(static_cast<Core::Light*>(i->get()));
		}
    }
}

void Core::RenderSystem::render_shadow_casters() {

	// Render each visible mesh.  Only meshes block light;
	// other geometry does not
    for (vector<Core::MeshObjectPtr>::iterator i = mesh_objects_.begin(); i != mesh_objects_.end(); i++) {		
		Core::MeshObjectPtr mesh_object = *i;
		if (mesh_object->cast_shadows()) {

			// Transform the modelview matrix using the node's transformation
			// matrix
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(mesh_object->parent()->matrix());
			
			// Render the object with no materials/shaders for speed
			mesh_object->mesh()->render(0);
		
			// Pop the modelview matrix off the stack
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
}

void Core::RenderSystem::render_visible_objects() {
    
	Core::MaterialPtr material;
	
	// Render all MeshObjects
	for (vector<Core::MeshObjectPtr>::iterator i = mesh_objects_.begin(); i != mesh_objects_.end(); i++) {
		Core::MeshObjectPtr mesh_object = *i;
		
		// If the material is different from the current material, then disable
		// the old material and enable the new material
		if (mesh_object->material() != material) {
			if (material) {
				material->enabled(false);
			}
			material = mesh_object->material();
			material->enabled(true);
		}
		
		// Transform the modelview and texture matrices (for shadow mapping)
		// using the node's transformation matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(mesh_object->parent()->matrix());
		glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glMultMatrixf(mesh_object->parent()->matrix());
		
		// Enable the material and render the mesh
		mesh_object->mesh()->render(material->shader());
    
		// Pop the modelview and texture matrices off the stack
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
	}
	
	// Disable the last material
	if (material) {
		material->enabled(false);
	}
}

void Core::RenderSystem::render_fullscreen_quad() {	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glColor3f(1.0f, 1.0f, 1.0f);	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    
}
