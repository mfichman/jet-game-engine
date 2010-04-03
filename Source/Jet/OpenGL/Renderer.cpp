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
#include <Jet/Engine.hpp>
#include <Jet/Matrix.hpp>
#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace Jet::OpenGL;
using namespace Jet;
using namespace std;

Renderer::Renderer(Engine* engine) :
    engine_(engine),
    shadow_vars_(engine->component("shadows")),
    window_vars_(engine->component("window")),
    shadow_map_(0),
    shadow_buffer_(0) {
        
    GLint argc = 0;
    glutInit(&argc, 0);
    init_window();
    
    if (GLEW_OK != glewInit()) {
        throw runtime_error("GLEW initialization failed");
    }
    if (!glewIsSupported("GL_VERSION_2_0")) {
        throw runtime_error("OpenGL 2.0 not supported");
    }
    
    
    init_default_states();
    init_matrices();
    begin_shadow_mapping();
    
    shader("Basic");
}

void Renderer::init_window() {
    uint32_t width = (uint32_t)window_vars_->value("width");
    uint32_t height = (uint32_t)window_vars_->value("height");
    string title = window_vars_->value("title");
    
    // Initialize the window
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutCreateWindow(title.c_str());
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
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    
    typedef int (APIENTRY *swap_interval_t)(int);
    swap_interval_t glSwapInterval = (swap_interval_t)wglGetProcAddress("wglSwapIntervalEXT");
    glSwapInterval(0);
}

void Renderer::init_matrices() {
    // Projection matrix setup


}

void Renderer::begin_shadow_mapping() {
    if (!shadow_vars_->value("enabled")) {
        return;
    }
    
    assert(!shadow_map_);
    assert(!shadow_buffer_);
    
    GLsizei texture_size = (GLsizei)shadow_vars_->value("texture_size");
    
    // Initialize the shadow map texture
    glGenTextures(1, &shadow_map_);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
    
    // GL_LINEAR does not make sense for depth textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Clamp to edge to remove artifacts on the edges of the shadow map
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // GL_DEPTH_COMPONENT gives you the max precision available    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texture_size, texture_size, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Generate the frame buffer objects
	glGenFramebuffers(1, &shadow_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer_);
    glDrawBuffer(GL_NONE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_, 0);
    
    // Check for support
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        throw runtime_error("Frame buffer configuration failed");
    }
    
    // Switch to the back buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawBuffer(GL_BACK);
}

void Renderer::end_shadow_mapping() {
    if (shadow_map_) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &shadow_map_);
    }
    if (shadow_buffer_) {
        glDeleteFramebuffers(1, &shadow_buffer_);
    }
    shadow_map_ = 0;
    shadow_buffer_ = 0;
}


static GLfloat angle = 0.0f;

void Renderer::on_render() {
    
    glutMainLoopEvent();
    angle += 0.005f;
    
    generate_shadow_map();    
    render_final();
    
    glutSwapBuffers();
}


void Renderer::generate_shadow_map() {
    GLsizei texture_size = (GLsizei)shadow_vars_->value("texture_size");
    
    // Set up the projection matrix for the light
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0f, 1.0f, 0.01f, 100.0f);
    glViewport(0, 0, texture_size, texture_size);
    
    // Set up the view matrix for the light
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Set light parameters
    static GLfloat lambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lambient);
    glDisable(GL_LIGHT0);
    
    // Initialize the texture matrix for transforming the coordinates
    GLfloat light_projection[16];
    GLfloat light_modelview[16];
    GLfloat light_bias[16] = {
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    };
    glGetFloatv(GL_PROJECTION_MATRIX, light_projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, light_modelview);
    glMatrixMode(GL_TEXTURE);
    glActiveTexture(GL_TEXTURE4);
    glLoadMatrixf(light_bias);
    glMultMatrixf(light_projection);
    glMultMatrixf(light_modelview);
    
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer_);
    glDrawBuffer(GL_NONE);
    glCullFace(GL_FRONT); // Only render the back faces to the depth buffer
    
    glClear(GL_DEPTH_BUFFER_BIT);
    render_shadow_casters();
    
    // Switch back to the default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glCullFace(GL_BACK);
    
    glEnable(GL_LIGHT0);
}

void Renderer::render_final() {
    GLfloat width = (GLfloat)window_vars_->value("width");
    GLfloat height = (GLfloat)window_vars_->value("height");
    
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, width/height, 0.1, 1000.0);
    glViewport(0, 0, (uint32_t)width, (uint32_t)height);
    
    // Moelview matrix setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //gluLookAt(5.0f*sinf(angle), 0.0f, 5.0f*cosf(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    gluLookAt(10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Set light parameters
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    static GLfloat lambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static GLfloat ldiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLfloat lspecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLfloat lposition[4] = { 10.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    glEnable(GL_LIGHT0);
    glPopMatrix();
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
    glEnable(GL_TEXTURE_2D);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShaderPtr basic = shader("Basic");
    basic->begin();
    render_visible_objects();
    basic->end();
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::render_teapots() {
    glMatrixMode(GL_MODELVIEW);
    // Set up matrials
    static GLfloat mambient[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    static GLfloat mdiffuse[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    static GLfloat mspecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mdiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mspecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);   
    
   
    
    // Render geometry
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-3.0f, -10.0f, -10.0f);
    glVertex3f(-3.0f, -10.0f, 10.0f);
    glVertex3f(-3.0f, 10.0f, 10.0f);
    glVertex3f(-3.0f, 10.0f, -10.0f);
    glEnd();
    
    
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glRotatef(angle*10, 0.0f, 1.0f, 0.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(angle*10, 0.0f, 1.0f, 0.0f);
    glFrontFace(GL_CCW);
    //glutSolidSphere(2.0f, 32, 32);

    glutSolidTorus(1.0f, 2.0f, 32, 32);
    glFrontFace(GL_CW);
    glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 0.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 0.0f);
    glutSolidTeapot(0.5f);
    glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

void Renderer::render_shadow_casters() {
    render_teapots();
}

void Renderer::render_visible_objects() {
    render_teapots();
}

Shader* Renderer::shader(const std::string& name) {
    map<string, ShaderPtr>::iterator i = shader_.find(name);
    if (i == shader_.end()) {
        for (Iterator<const string> i = engine_->folders(); i; i++) {
            try {
                string path = (*i) + "/" + name;
                ShaderPtr shader(new Shader(path));
                return shader_.insert(make_pair(name, new Shader(path))).first->second.get();
            } catch (range_error&) {
            }
        }
        throw range_error("Shader not found: " + name);
    } else {
        return i->second.get();
    }
}

// Use flat shading for speed: glShadeModel(GL_FLAT)
// Disable color writes: glColorMask(0, 0, 0, 0)
// Draw only the back faces: glCullFace(FL_FRONT)

    //glDrawBuffer(GL_BACK);
    //glReadBuffer(GL_BACK);
    
        //glDrawBuffer(GL_NONE); // Prevent writing to color buffer (for perf.)
    //glReadBuffer(GL_NONE);

