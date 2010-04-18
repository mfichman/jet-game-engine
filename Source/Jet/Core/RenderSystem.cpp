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
#include <GL/freeglut.h>

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
        
    GLint argc = 0;
    glutInit(&argc, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
}

void Core::RenderSystem::init_window() {
    int32_t width = any_cast<int32_t>(engine_->option("display_width"));
    int32_t height = any_cast<int32_t>(engine_->option("display_height"));
    string title = any_cast<string>(engine_->option("window_title"));

    // If in full screen mode, then use GLUT game mode
	if (any_cast<bool>(engine_->option("fullscreen"))) {
        stringstream ss;
        ss << width << "x" << height << ":32@60" << endl;
        glutGameModeString(ss.str().c_str());
        glutEnterGameMode();
    } else {
		glutInitWindowSize(width, height);
        glutCreateWindow(title.c_str());
    }
	
	if (GLEW_OK != glewInit()) {
        throw runtime_error("GLEW initialization failed");
    }
    if (!glewIsSupported("GL_VERSION_2_0")) {
        throw runtime_error("OpenGL 2.0 not supported");
    }
}

void Core::RenderSystem::init_default_states() {
    // Initialize some common options
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // We always use CCW culling
    glCullFace(GL_BACK);
    
    GLfloat width = (GLfloat)any_cast<GLint>(engine_->option("display_width"));
    GLfloat height = (GLfloat)any_cast<GLint>(engine_->option("display_height"));
    glViewport(0, 0, (uint32_t)width, (uint32_t)height);
    
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, width/height, 0.1f, 1000.0f);
    
    // Turn of VSYNC so the game can run at full frame rate
    typedef int (APIENTRY *swap_interval_t)(int);
    swap_interval_t glSwapInterval = (swap_interval_t)wglGetProcAddress("wglSwapIntervalEXT");
    glSwapInterval(0);
}

void Core::RenderSystem::on_init() {
    init_window();
    init_default_states();

	GLsizei size = any_cast<GLsizei>(engine_->option("shadow_texture_size"));
    shadow_target_.reset(new RenderTarget(size, size, GL_DEPTH_COMPONENT));
    
    // Bind the shadow sampler to the shadow texture
    glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
    glBindTexture(GL_TEXTURE_2D, shadow_target_->texture());
}

void Core::RenderSystem::on_render() {
	
    glutMainLoopEvent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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
    
    glutSwapBuffers();
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
    //gluPerspective(30.0f, 1.0f, 0.01f, 1000.0f);
    //glOrtho(2048.0, 2048.0, 2048.0, 2048.0, 0.01, 1000.0f);
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
    glEnable(GL_LIGHT0);
    
    // Render to the back buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_visible_objects();
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