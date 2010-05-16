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
#include <Jet/Core/Overlay.hpp>
#include <Jet/Core/RenderTarget.hpp>
#include <Jet/Core/Light.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Matrix.hpp>
#include <Jet/Box.hpp>
#include <Jet/Frustum.hpp>

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define putenv _putenv
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
    GLuint width = (GLuint)engine_->option<float>("display_width");
    GLuint height = (GLuint)engine_->option<float>("display_height");
    string title = engine_->option<string>("window_title");
	bool fullscreen_enabled = engine_->option<bool>("fullscreen_enabled");
	bool vsync_enabled = engine_->option<bool>("vsync_enabled");
	bool fsaa_enabled = engine_->option<bool>("fsaa_enabled");
	GLuint fsaa_samples = (GLuint)engine_->option<float>("fsaa_samples");
	
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
	putenv("SDL_VIDEO_WINDOW_POS=center");
	
	uint32_t flags = SDL_OPENGL;
	if (fullscreen_enabled) {
		flags |= SDL_FULLSCREEN;
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	
	
	if (!SDL_SetVideoMode(width, height, 0, flags)) {
		throw runtime_error(string("SDL initialization failed: ") + SDL_GetError());
	}
    glViewport(0, 0, (uint32_t)width, (uint32_t)height);

	
	engine_->overlay()->width((float)width);
	engine_->overlay()->height((float)height);
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
			glBufferSubData = glBufferSubDataARB;
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
			glUniform1iv = glUniform1ivARB;
			glUniform1f = glUniform1fARB;
			glUniform1fv = glUniform1fvARB;
			glGetUniformLocation = glGetUniformLocationARB;
		}
    }	
}

void Core::RenderSystem::init_default_states() {
    // Initialize some common s
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
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
	
	// Initialize particle buffer
	particle_buffer_.reset(new ParticleBuffer(engine_));
	
	engine_->option("video_mode_synced", true);
	//GLuint width = (GLuint)engine_->option<float>("display_width");
	//GLuint height = (GLuint)engine_->option<float>("display_height");
    //color_target_.reset(new RenderTarget(width, height, false, 2));
	//bloom_target1_.reset(new RenderTarget(width/8, height/8, false, 1));
	//bloom_target2_.reset(new RenderTarget(width/8, height/8, false, 1));
}

void Core::RenderSystem::check_video_mode() {
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
		shadow_target_.clear();
		particle_buffer_.reset();
		on_init();
	}
	
	// If shadows are enabled, make sure a render target is ready for
	// receiving them.  Otherwise, clear the render target.
	if (engine_->option<bool>("shaders_enabled") && engine_->option<bool>("shadows_enabled")) {
		if (shadow_target_.empty()) {
			size_t cascades = min((size_t)MAX_SHADOW_CASCADES, (size_t)engine_->option<float>("shadow_cascades"));
			GLuint size = (GLuint)engine_->option<float>("shadow_texture_size");

			for (size_t i = 0; i < cascades; i++) {
				shadow_target_.push_back(new RenderTarget(size, size, true, 1));

				// These states are used to enable percentage closer filtering for the 
				// shadow map sampler
				glBindTexture(GL_TEXTURE_2D, shadow_target_.back()->texture(0));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
				glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	} else {
		shadow_target_.clear();
	}
}

void Core::RenderSystem::on_render() {	
	if (!engine_->camera()) {
		return;
	}
	
	// Update the video mode if it has changed
	check_video_mode();

	// Clear the list of active mesh objects and lights
	mesh_objects_.clear();
	particle_systems_.clear();
	lights_.clear();
	generate_render_list(static_cast<Core::Node*>(engine_->root()));
	
	// Sort the meshes by material
	sort(mesh_objects_.begin(), mesh_objects_.end(), &RenderSystem::compare_mesh_objects);
	sort(particle_systems_.begin(), particle_systems_.end(), &RenderSystem::compare_particle_systems);
    
    // Render the scene once for each light
	bool shaders_enabled = engine_->option<bool>("shaders_enabled");
	bool shadows_enabled = engine_->option<bool>("shadows_enabled");
    for (vector<LightPtr>::iterator i = lights_.begin(); i != lights_.end(); i++) {
		if (shaders_enabled && shadows_enabled) {
			generate_shadow_map(i->get());
		}
        render_final(i->get());
		// TODO: render more than one light
        break;
    }

	// Render overlays on top of the screen (may need to clear z-buffer...)
	render_overlays();
	
	// Swap back buffer to front
	SDL_GL_SwapBuffers();
}

void Core::RenderSystem::generate_shadow_map(Light* light) {    
	Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());
	
	// Generate an orthogonal basis (rotation matrix) for the
	// directional light
	Vector forward = light->direction().unit();
	Vector up = forward.orthogonal();
	Vector right = forward.cross(up);
	Matrix matrix(right, -up, forward);

	size_t cascades = min((size_t)MAX_SHADOW_CASCADES, (size_t)engine_->option<float>("shadow_cascades"));
	float alpha = engine_->option<float>("shadow_correction"); 
	float n = camera->near_clipping_distance();
	float f = min(camera->far_clipping_distance(), engine_->option<float>("shadow_distance"));
	for (size_t i = 0; i < cascades; i++) {
		// Get the near and far clipping planes
		float r0 = (float)i/(float)cascades;
		float r1 = (float)(i+1)/(float)cascades;
		float near_dist = alpha*n*pow(f/n, r0) + (1-alpha)*(n+r0*(f-n));
		float far_dist = alpha*n*pow(f/n, r1) + (1-alpha)*(n+r1*(f-n));

		// Transform the view frustum into light space and calculate
		// the bounding box 
		Box bounds(matrix * camera->frustum(near_dist, far_dist));
		
		// Causes the shadow map cascades to overlap
		//bounds.max_z += 5.0f;
		
		// Set up the projection matrix for the directional light
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(bounds.min_x, bounds.max_x, bounds.min_y, bounds.max_y, bounds.min_z, bounds.max_z);
	    
		// Set up the view matrix for the directional light
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(matrix);	
		
		// Material 
		glMaterialfv(GL_FRONT, GL_AMBIENT, Color(1.0f, 1.0f, 1.0f, 1.0f));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Color(1.0f, 1.0f, 1.0f, 1.0f));
		glMaterialfv(GL_FRONT, GL_SPECULAR, Color(0.0f, 0.0f, 0.0f, 0.0f));
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	    
		// Render to the front buffer
		shadow_target_[i]->enabled(true);
		glDisable(GL_LIGHTING);
		glCullFace(GL_FRONT);
		render_shadow_casters();
		shadow_target_[i]->enabled(false);
		glEnable(GL_LIGHTING);
		glCullFace(GL_BACK);
			
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
		glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER + i);
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(light_bias);
		glMultMatrixf(light_projection);
		glMultMatrixf(light_modelview);
	}
	
	
}

void Core::RenderSystem::render_final(Light* light) {	
	Core::CameraPtr camera = static_cast<Core::Camera*>(engine_->camera());
	Matrix matrix = camera->parent()->matrix();
	float width = engine_->option<float>("display_width");
	float height = engine_->option<float>("display_height");
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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
	for (size_t i = 0; i < shadow_target_.size(); i++) {
		shadow_target_[i]->sampler(SHADOW_MAP_SAMPLER + i);
	}
    
    // Render to the back buffer.
	render_visible_mesh_objects();
	render_visible_particle_systems();
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

void Core::RenderSystem::render_visible_mesh_objects() {
	Core::Material* material = 0;
	bool shaders_enabled = engine_->option<bool>("shaders_enabled");
	
	// Render all MeshObjects
	for (vector<Core::MeshObjectPtr>::iterator i = mesh_objects_.begin(); i != mesh_objects_.end(); i++) {
		Core::MeshObject* mesh_object = i->get();
		Core::Mesh* mesh = mesh_object->mesh();
		const Matrix& matrix = mesh_object->parent()->matrix();
		
		// Switch materials if necessary
		if (material != mesh_object->material()) {
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
		glMultMatrixf(matrix);

		// This is a hack to pass the model matrix to the shader.  Really,
		// a uniform variable should be used, but it's a pain to set the
		// uniform variable for the shader currently.
		if (shaders_enabled) {
			glActiveTexture(GL_TEXTURE0);
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(matrix);
		}
		
		// Enable the material and render the mesh
		mesh->render(material->shader());
		    
		// Pop the modelview and texture matrices off the stack
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	
	// Disable the last material
	if (material) {
		material->enabled(false);
	}

	// Reset the 0 texture matrix to identity
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
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

void Core::RenderSystem::render_overlays() {
	float width = engine_->option<float>("display_width");
	float height = engine_->option<float>("display_height");
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	static_cast<Core::Overlay*>(engine_->overlay())->render();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	
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
	
	// Do not render invisible nodes or their children
	if (!node->visible()) {
		return;
	}

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

bool Core::RenderSystem::compare_particle_systems(ParticleSystemPtr o1, ParticleSystemPtr o2) {
	return o1->texture() < o2->texture();
}
