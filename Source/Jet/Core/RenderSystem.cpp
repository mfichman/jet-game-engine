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
#include <Jet/BoundingBox.hpp>
#include <Jet/Frustum.hpp>

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
    engine_(engine),
	active_material_(0) {
		
	engine_->listener(this);	
	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(string("SDL initialization failed: ") + SDL_GetError());
	}
}
	
Core::RenderSystem::~RenderSystem() {
	//SDL_Quit();
}

void Core::RenderSystem::init_window() {
	std::cout << "Initializing render system" << std::endl;
    GLuint width = (GLuint)engine_->option<real_t>("display_width");
    GLuint height = (GLuint)engine_->option<real_t>("display_height");
    string title = engine_->option<string>("window_title");
	bool fullscreen_enabled = engine_->option<bool>("fullscreen_enabled");
	bool vsync_enabled = engine_->option<bool>("vsync_enabled");
	bool fsaa_enabled = engine_->option<bool>("fsaa_enabled");
	GLuint fsaa_samples = (GLuint)engine_->option<real_t>("fsaa_samples");
	
	if (fsaa_enabled) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, fsaa_samples);
		glEnable(GL_MULTISAMPLE);
	}
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync_enabled);
	
	uint32_t flags = SDL_OPENGL;
	if (fullscreen_enabled) {
		flags |= SDL_FULLSCREEN;
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	
	if (!SDL_SetVideoMode(width, height, 0, flags)) {
		throw runtime_error(string("SDL initialization failed: ") + SDL_GetError());
	}
    glViewport(0, 0, (uint32_t)width, (uint32_t)height);
}

void Core::RenderSystem::init_extensions() {
	
	// Initialize extensions	
	if (GLEW_OK != glewInit()) {
        throw runtime_error("GLEW initialization failed");
    }

	// If OpenGL 2.0 is not supported, we have to check extensions individually, and
	// copy the function pointers that GLEW set up for us.  Most extensions that
	// were incorporated into OpenGL 2.0 have an equivalent ARB board extension in
	// pre-2.0 version.
    if (!glewIsSupported("GL_VERSION_2_0")) {

		// Vertex buffer objects.  This functionality is required.
		if (!glewIsSupported("GL_ARB_vertex_buffer_object")) {
			throw runtime_error("Unsupported graphics hardware");
		} else {
			glGenBuffers = glGenBuffersARB;
			glBindBuffer = glBindBufferARB;
			glBufferData = glBufferDataARB;
			glDeleteBuffers = glDeleteBuffersARB;
		}

		// Multitexturing.  This functionality is required.
		if (!glewIsSupported("GL_ARB_multitexture")) {
			throw runtime_error("Unsupported graphics hardware");
		} else {
			glActiveTexture = glActiveTextureARB;
		}

		//! Frame buffer objects.  This functionality is optional.
		if (!glewIsSupported("GL_EXT_framebuffer_object GL_ARB_draw_buffers")) {
			engine_->option("shadows_enabled", false);
		} else {
			glGenFramebuffers = glGenFramebuffersEXT;
			glBindFramebuffer = glBindFramebufferEXT;
			glFramebufferTexture2D = glFramebufferTexture2DEXT;
			glGenRenderbuffers = glGenRenderbuffersEXT;
			glDrawBuffers = glDrawBuffersARB;
			glBindRenderbuffer = glBindRenderbufferEXT;
			glRenderbufferStorage = glRenderbufferStorageEXT;
			glFramebufferRenderbuffer = glFramebufferRenderbufferEXT;
			glCheckFramebufferStatus = glCheckFramebufferStatusEXT;
			glDeleteFramebuffers = glDeleteFramebuffersEXT;
			glDeleteRenderbuffers = glDeleteRenderbuffersEXT;
			glEnableVertexAttribArray = glEnableVertexAttribArrayARB;
			glDisableVertexAttribArray = glDisableVertexAttribArrayARB;
			glVertexAttribPointer = glVertexAttribPointerARB;
		}
		
		// Shaders.  This functionality is optional.
		if (!glewIsSupported("GL_ARB_vertex_program GL_ARB_fragment_program GL_ARB_shader_objects GL_ARB_shading_language_100")) {
			engine_->option("shaders_enabled", false);
			engine_->option("shadows_enabled", false);
		} else {
			glCreateShader = glCreateShaderObjectARB;
			glCreateProgram = glCreateProgramObjectARB;
			glShaderSource = glShaderSourceARB;
			glCompileShader = glCompileShaderARB;
			glAttachShader = glAttachObjectARB;
			glLinkProgram = glLinkProgramARB;
			glGetShaderInfoLog = glGetInfoLogARB;
			glGetAttribLocation = glGetAttribLocationARB;
			glDeleteProgram = glDeleteObjectARB;
			glDeleteShader = glDeleteObjectARB;
			glUseProgram = glUseProgramObjectARB;
			glUniform1i = glUniform1iARB;
			glGetUniformLocation = glGetUniformLocationARB;
		}
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
}

void Core::RenderSystem::on_init() {
	init_window();
	init_extensions();
    init_default_states();
		
	// Initialize shadow target
	if (engine_->option<bool>("shadows_enabled")) {
		GLuint size = (GLuint)engine_->option<real_t>("shadow_texture_size");
    	shadow_target_.reset(new RenderTarget(size, size, true, 1));
	}
	
	// Initialize particle buffer
	particle_buffer_.reset(new ParticleBuffer(engine_));
	
	
	engine_->option("video_mode_synced", true);
	//GLuint width = (GLuint)engine_->option<real_t>("display_width");
	//GLuint height = (GLuint)engine_->option<real_t>("display_height");
    //color_target_.reset(new RenderTarget(width, height, false, 2));
	//bloom_target1_.reset(new RenderTarget(width/8, height/8, false, 1));
	//bloom_target2_.reset(new RenderTarget(width/8, height/8, false, 1));
}

void Core::RenderSystem::on_update() {
}

void Core::RenderSystem::on_render() {	
	if (!engine_->camera()) {
		return;
	}

	// Clear the list of active mesh objects and lights
	mesh_objects_.clear();
	fracture_objects_.clear();
	particle_systems_.clear();
	lights_.clear();
	generate_render_list(static_cast<Core::Node*>(engine_->root()));
	
	// Sort the meshes by material
	sort(mesh_objects_.begin(), mesh_objects_.end(), &RenderSystem::compare_mesh_objects);
	sort(fracture_objects_.begin(), fracture_objects_.end(), &RenderSystem::compare_fracture_objects);

	// If the video mode has been marked as changed, then switch modes
	if (!engine_->option<bool>("video_mode_synced")) {
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapBuffers();
		
		// Demote all resources to the LOADED state, because the OpenGL context
		// is about to be destroyed
		for (Iterator<pair<const string, Jet::MeshPtr> > i = engine_->meshes(); i; i++) {
			i->second->state(LOADED);
		}
		for (Iterator<pair<const string, Jet::TexturePtr> > i = engine_->textures(); i; i++) {
			i->second->state(LOADED);
		}
		for (Iterator<pair<const string, Jet::ShaderPtr> > i = engine_->shaders(); i; i++) {
			i->second->state(LOADED);
		}
		shadow_target_.reset();
		particle_buffer_.reset();
		on_init();
	}
	
	// If shadows are enabled, make sure a render target is ready for
	// receiving them.  Otherwise, clear the render target.
	bool shaders_enabled = engine_->option<bool>("shaders_enabled");
	bool shadows_enabled = engine_->option<bool>("shadows_enabled");
	if (shaders_enabled && shadows_enabled) {
		if (!shadow_target_) {
			GLuint size = (GLuint)engine_->option<real_t>("shadow_texture_size");
			shadow_target_.reset(new RenderTarget(size, size, true, 1));
		}
	} else {
		shadow_target_.reset();
	}
    
    // Render the scene once for each light
    for (vector<LightPtr>::iterator i = lights_.begin(); i != lights_.end(); i++) {
		if (shaders_enabled && shadows_enabled) {

			generate_shadow_map(i->get());
		}
        render_final(i->get());
		// TODO: render more than one light
        break;
    }
	
	// Swap back buffer to front
	SDL_GL_SwapBuffers();
}

void Core::RenderSystem::generate_shadow_map(Light* light) {    
	render_pass_ = SHADOW_PASS;
	Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());

	
	// Generate an orthogonal basis (rotation matrix) for the
	// directional light
	Vector forward = light->direction().unit();
	Vector up = forward.orthogonal();
	Vector right = forward.cross(up);
	Matrix matrix(right, up, forward);
	
	// Transform the view frustum into light space and calculate
	// the bounding box 
	BoundingBox bounds(matrix * camera->shadow_frustum());
	
	// Set up the projection matrix for the directional light
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(bounds.min_x, bounds.max_x, bounds.min_y, bounds.max_y, bounds.min_z, bounds.max_z);
    
	// Set up the view matrix for the directional light
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(matrix);
    
    // Render to the front buffer
    shadow_target_->enabled(true);
    glDisable(GL_LIGHTING);
    render_shadow_casters();
    shadow_target_->enabled(false);
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
	
	
}

void Core::RenderSystem::render_final(Light* light) {
	render_pass_ = MAIN_PASS;
   
	Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());
	Matrix matrix = camera->parent()->matrix();
	real_t width = engine_->option<real_t>("display_width");
	real_t height = engine_->option<real_t>("display_height");
	
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera->field_of_view(), width/height, camera->near_clipping_distance(), camera->far_clipping_distance());
	
	// Modelview matrix setup
	Vector eye = matrix.origin();
	Vector at = matrix * Vector(0.0f, 0.0f, 1.0f);
	Vector up = matrix.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
        
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
        const Vector& direction = light->direction().unit();
        GLfloat lposition[4] = { direction.x, direction.y, direction.z, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lposition);
	}
	glEnable(GL_LIGHT0);
		
	// Bind the shadow sampler to the shadow texture
	if (shadow_target_) {
		shadow_target_->sampler(SHADOW_MAP_SAMPLER);
	}
    
    // Render to the back buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_visible_mesh_objects();
	render_visible_fracture_objects();
	render_visible_particle_systems();
	

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
	
	//glDisable(GL_CULL_FACE);
	
	// Render each visible mesh.  Only meshes block light;
	// other geometry does not
    for (vector<Core::FractureObjectPtr>::iterator i = fracture_objects_.begin(); i != fracture_objects_.end(); i++) {		
		Core::FractureObjectPtr fracture_object = *i;
		if (fracture_object->cast_shadows()) {

			// Transform the modelview matrix using the node's transformation
			// matrix
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(fracture_object->parent()->matrix());
		
			// Render the object with no materials/shaders for speed
			fracture_object->render(0);
		
			// Pop the modelview matrix off the stack
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
	
	//glEnable(GL_CULL_FACE);
}

void Core::RenderSystem::render_visible_mesh_objects() {
	
	// Render all MeshObjects
	for (vector<Core::MeshObjectPtr>::iterator i = mesh_objects_.begin(); i != mesh_objects_.end(); i++) {
		Core::MeshObject* mesh_object = i->get();
		
		// Enable the material and render the mesh
		active_material(mesh_object->material());
		push_modelview_matrix(mesh_object->parent()->matrix());
		mesh_object->mesh()->render(mesh_object->material()->shader());
		pop_modelview_matrix();

	}
	
	// Disable the last material
	active_material(0);
}

void Core::RenderSystem::render_visible_fracture_objects() {
	
	glDisable(GL_CULL_FACE);
	
	// Render all FractureObjects
	for (vector<Core::FractureObjectPtr>::iterator i = fracture_objects_.begin(); i != fracture_objects_.end(); i++) {
		Core::FractureObject* fracture_object = i->get();
		
		// Enable the material and render the mesh
		active_material(fracture_object->material());
		push_modelview_matrix(fracture_object->parent()->matrix());
		fracture_object->render(fracture_object->material()->shader());
		pop_modelview_matrix();

	}
	
	glEnable(GL_CULL_FACE);
	
	// Disable the last material
	active_material(0);
}


void Core::RenderSystem::render_visible_particle_systems() {
	
	for (vector<Core::ParticleSystemPtr>::iterator i = particle_systems_.begin(); i != particle_systems_.end(); i++) {
		Core::ParticleSystem* particle_system = i->get();
		
		// Render the particle system using the buffer
		particle_system->render(particle_buffer_.get());
	}
	particle_buffer_->flush();
	particle_buffer_->shader(0);
	particle_buffer_->texture(0);
}

inline void Core::RenderSystem::active_material(Material* material) {
	// If the material is different from the current material, then disable
	// the old material and enable the new material
	if (active_material_ != material) {
		if (active_material_) {
			active_material_->enabled(false);
		}
		active_material_ = material;
		if (active_material_) {
			active_material_->enabled(true);
		}
	}
}

inline void Core::RenderSystem::push_modelview_matrix(const Matrix& matrix) {
	// Transform the modelview and texture matrices (for shadow mapping)
	// using the node's transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix);
	glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glMultMatrixf(matrix);
}

inline void Core::RenderSystem::pop_modelview_matrix() {
	// Pop the modelview and texture matrices off the stack
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
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


void Core::RenderSystem::generate_render_list(Core::Node* node) {

	// Iterate through all sub objects and add them to the appropriate
	// render list as necessary
    for (Iterator<ObjectPtr> i = node->objects(); i; i++) {
        const type_info& type = typeid(**i);
        if (typeid(Core::Node) == type) {
			// Recursively add nodes
            generate_render_list(static_cast<Core::Node*>(i->get()));
			
        } else if (typeid(Core::MeshObject) == type) {
			// Add mesh objects that have a valid material and mesh
			Core::MeshObject* mesh_object = static_cast<Core::MeshObject*>(i->get());
			if (mesh_object->material() && mesh_object->mesh()) {
				mesh_objects_.push_back(mesh_object);
			}
			
        } else if (typeid(Core::FractureObject) == type) {
			// Add fracture objects with a valid material and mesh
			Core::FractureObject* fracture_object = static_cast<Core::FractureObject*>(i->get());
			if (fracture_object->material() && fracture_object->mesh()) {
				fracture_objects_.push_back(fracture_object);
			}
			
		} else if (typeid(Core::Light) == type) {
			// Add all lights
			lights_.push_back(static_cast<Core::Light*>(i->get()));
			
		} else if (typeid(Core::ParticleSystem) == type) {
			// Add particle systems with a valid texture
			Core::ParticleSystem* particle_system = static_cast<Core::ParticleSystem*>(i->get());
			if (particle_system->texture()) {
				particle_systems_.push_back(particle_system);
			}
		}
    }
}

bool Core::RenderSystem::compare_mesh_objects(MeshObjectPtr o1, MeshObjectPtr o2) {
    return o1->material() < o2->material();
}

bool Core::RenderSystem::compare_fracture_objects(FractureObjectPtr o1, FractureObjectPtr o2) {
    return o1->material() < o2->material();
}

bool Core::RenderSystem::compare_particle_systems(ParticleSystemPtr o1, ParticleSystemPtr o2) {
	return o1->texture() < o2->texture();
}
