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

#include <Jet/OpenGL/Renderer.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/Engine.hpp>
#include <Jet/Matrix.hpp>
#include <Jet/Light.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace Jet::OpenGL;
using namespace Jet;
using namespace std;
using namespace boost;

Renderer::Renderer(Engine* engine) :
    engine_(engine) {
        
    GLint argc = 0;
    glutInit(&argc, 0);
    init_window();
    init_default_states();
    
    if (GLEW_OK != glewInit()) {
        throw runtime_error("GLEW initialization failed");
    }
    if (!glewIsSupported("GL_VERSION_2_0")) {
        throw runtime_error("OpenGL 2.0 not supported");
    }
    
    GLsizei size = any_cast<GLsizei>(engine->option("shadow_texture_size"));
    shadow_target_.reset(new RenderTarget(size, size, GL_DEPTH_COMPONENT));
    
    // Bind the shadow sampler to the shadow texture
    glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_SAMPLER);
    glBindTexture(GL_TEXTURE_2D, shadow_target_->texture());
}

void Renderer::init_window() {
    int32_t width = any_cast<int32_t>(engine_->option("display_width"));
    int32_t height = any_cast<int32_t>(engine_->option("display_height"));
    string title = any_cast<string>(engine_->option("window_title"));
    
    // Initialize the window
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    
    // If in full screen mode, then use GLUT game mode
	if (any_cast<bool>(engine_->option("fullscreen"))) {
        stringstream ss;
        ss << width << "x" << height << ":32@60" << endl;
        glutGameModeString(ss.str().c_str());
        glutEnterGameMode();
    } else {
        glutCreateWindow(title.c_str());
    }
}

void Renderer::init_default_states() {
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

static GLfloat angle = 0.0f;

void Renderer::on_render() {
    
    glutMainLoopEvent();
    angle += 0.0005f;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the scene once for each light
    for (Iterator<LightPtr> i = engine_->active_lights(); i; i++) {
        generate_shadow_map(i->get());
        render_final(i->get());
        break;
    }
    
    glutSwapBuffers();
}


void Renderer::generate_shadow_map(Light* light) {    
    // Set up the projection matrix for the light
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(65.0f, 1.0f, 0.01f, 100.0f);
    
    // Set up the view matrix for the light
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    const Vector& light_pos = light->parent()->position();
    gluLookAt(light_pos.x, light_pos.y, light_pos.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Render to the front buffer
    shadow_target_->begin();
    glCullFace(GL_FRONT); // Only render the back faces to the depth buffer
    glDisable(GL_LIGHTING);
    render_shadow_casters();
    shadow_target_->end();
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
    glActiveTexture(GL_TEXTURE0);
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(light_bias);
    glMultMatrixf(light_projection);
    glMultMatrixf(light_modelview);
    
    // Pop the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void Renderer::render_final(Light* light) {
    
    // Moelview matrix setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(15.0f*sinf(angle/2), 0.0f, 15.0f*cosf(angle/2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        
    // Set light parameter
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light->diffuse_color());
    glLightfv(GL_LIGHT0, GL_SPECULAR, light->specular_color());
    glLightfv(GL_LIGHT0, GL_AMBIENT, light->ambient_color());
    
    // Set light location/direction.
    const Vector& light_pos = light->parent()->position();
    static GLfloat lposition[4] = { light_pos.x, light_pos.y, light_pos.z };
	if (light->type() == POINT_LIGHT) {
        lposition[3] = 1.0f;
    } else {
        lposition[3] = 0.0f;
    }
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    glEnable(GL_LIGHT0);
    
    // Render to the back buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_visible_objects();
}

void Renderer::render_teapots() {

}

void Renderer::render_shadow_casters() {
    
    // Render geometry
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.0f, -10.0f, -10.0f);
    glTexCoord2f(0.0, 1.0f);
    glVertex3f(-3.0f, -10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.0f, 10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.0f, 10.0f, -10.0f);
    glEnd();
    
    // Render each visible mesh object once.  TODO: only render objects with
    // shadow-casting enabled
    for (Iterator<MeshObjectPtr> i = engine_->visible_mesh_objects(); i; i++) {
        
        if ((*i)->cast_shadows()) {
            OpenGL::MeshPtr mesh = Renderer::mesh((*i)->mesh());
            mesh->render(0);
        }
    }
}

void Renderer::render_visible_objects() {
	// Render everything using the default shader for now
	OpenGL::ShaderPtr shader = Renderer::shader(engine_->shader("Basic"));
    shader->begin();
    
    // Render geometry
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
	glVertexAttrib3f(shader->tangent(), 0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 1.0f);
    glVertex3f(-3.0f, -10.0f, 10.0f);
	glVertexAttrib3f(shader->tangent(), 0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3.0f, -10.0f, -10.0f);
	glVertexAttrib3f(shader->tangent(), 0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3.0f, 10.0f, -10.0f);
	glVertexAttrib3f(shader->tangent(), 0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3.0f, 10.0f, 10.0f);
    glEnd();
    
    // Render each mesh in turn
    for (Iterator<MeshObjectPtr> i = engine_->visible_mesh_objects(); i; i++) {
		
        // Material set up
        MaterialPtr material = (*i)->material();		
        if (material->diffuse_map()) {
            OpenGL::TexturePtr diffuse = texture(material->diffuse_map());
            diffuse->bind(DIFFUSE_MAP_SAMPLER);
        }
        if (material->specular_map()) {
            OpenGL::TexturePtr specular = texture(material->specular_map());
            specular->bind(SPECULAR_MAP_SAMPLER);
        }
        if (material->normal_map()) {
            OpenGL::TexturePtr normal = texture(material->normal_map());
            normal->bind(NORMAL_MAP_SAMPLER);
        }
        glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient_color());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse_color());
        glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular_color());
        glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

        // Set the transformation matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf((*i)->parent()->matrix());
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glActiveTexture(GL_TEXTURE0);
		glMultMatrixf((*i)->parent()->matrix());

        // Render the mesh
		OpenGL::MeshPtr mesh = Renderer::mesh((*i)->mesh());
        mesh->render(shader.get());
        
        // Pop the matrix
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    
    shader->end();
}

OpenGL::Shader* Renderer::shader(Jet::Shader* desc) {
	OpenGL::ShaderPtr shader = static_cast<OpenGL::Shader*>(desc->impl());
    if (shader) {
        return shader.get();
    } else {
        return new OpenGL::Shader(engine_, desc);
    }
}

OpenGL::Mesh* Renderer::mesh(Jet::Mesh* desc) {
    OpenGL::MeshPtr mesh = static_cast<OpenGL::Mesh*>(desc->impl());
    if (mesh) {
        return mesh.get();
    } else {
        return new OpenGL::Mesh(desc);
    }
}

OpenGL::Texture* Renderer::texture(Jet::Texture* desc) {
    OpenGL::TexturePtr texture = static_cast<OpenGL::Texture*>(desc->impl());
    if (texture) {
        return texture.get();
    } else {
        return new OpenGL::Texture(desc);
    }
}

/*
void Renderer::permute_shaders(const string& name) {
    
    //! Search for the shader source on the path
    string path;
    bool found = false;
    for (Iterator<const string> i = engine_->folders(); i; i++) {
        path = (*i) + "/" + name;
        ifstream in((path + ".frag.glsl").c_str());
        if (in.good()) {
            found = true;
            break;
        }
    }
    
    if (!found) {
       throw runtime_error("Shader not found: " + name);
    }
    
    // Load a shader for each combination of the preprocessor definitions
    size_t noptions = 4;
    size_t ncombos = 1 << noptions;
    
    for (uint32_t i = 0; i < ncombos; i++) {
        vector<string> defines;
		for (uint32_t j = 0; j < noptions; j++) {
            size_t opt = 1 << j;
			if (opt & i) {
				defines.push_back(shader_option(opt));
			}
		}
		ShaderPtr shader(new Shader(path, defines));
		shader_.insert(make_pair(make_pair(name, i), shader));
    }
}*/


// Use flat shading for speed: glShadeModel(GL_FLAT)
// Disable color writes: glColorMask(0, 0, 0, 0)
// Draw only the back faces: glCullFace(FL_FRONT)

    //glDrawBuffer(GL_BACK);
    //glReadBuffer(GL_BACK);
    
        //glDrawBuffer(GL_NONE); // Prevent writing to color buffer (for perf.)
    //glReadBuffer(GL_NONE);

