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
#include <sstream>
#include <stdexcept>
#include <cmath>

using namespace Jet::OpenGL;
using namespace Jet;
using namespace std;

Renderer::Renderer(Engine* engine) :
    engine_(engine),
    shadow_vars_(engine->component("shadows")),
    window_vars_(engine->component("window")) {
        
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
    
    GLsizei size = (GLsizei)shadow_vars_->value("texture_size");
    shadow_target_.reset(new RenderTarget(size, size, GL_DEPTH_COMPONENT));
    
    shader("Basic");
}

void Renderer::init_window() {
    uint32_t width = (uint32_t)window_vars_->value("width");
    uint32_t height = (uint32_t)window_vars_->value("height");
    string title = window_vars_->value("title");
    
    // Initialize the window
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    
    
    if (window_vars_->value("fullscreen")) {
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
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    
    typedef int (APIENTRY *swap_interval_t)(int);
    swap_interval_t glSwapInterval = (swap_interval_t)wglGetProcAddress("wglSwapIntervalEXT");
    glSwapInterval(0);
}

static GLfloat angle = 0.0f;

void Renderer::on_render() {
    
    glutMainLoopEvent();
    angle += 0.0005f;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (Iterator<const pair<NodePtr, ComponentPtr>> i = engine_->lights(); i; i++) {
        generate_shadow_map(*i);
        render_final(*i);
        break;
    }
    
    glutSwapBuffers();
}


void Renderer::generate_shadow_map(const std::pair<NodePtr, ComponentPtr>& light) {    
    // Set up the projection matrix for the light
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0f, 1.0f, 0.01f, 100.0f);
    
    // Set up the view matrix for the light
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    const Vector& light_pos = light.first->position();
    gluLookAt(light_pos.x, light_pos.y, light_pos.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Render to the front buffer
    shadow_target_->begin();
    glCullFace(GL_FRONT); // Only render the back faces to the depth buffer
    glDisable(GL_LIGHTING);
    render_shadow_casters();
    shadow_target_->end();
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
        
    // Initialize the texture matrix for transforming the coordinates
    Matrix light_bias(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );
    Matrix light_projection;
    Matrix light_modelview;
    glGetFloatv(GL_PROJECTION_MATRIX, light_projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, light_modelview);
    Matrix light_matrix = light_bias*light_projection*light_modelview;
    
    ShaderPtr basic = shader("Basic");
    basic->texture("shadow_map", shadow_target_->texture());
    basic->texture_matrix("shadow_map", light_matrix);
}

void Renderer::render_final(const std::pair<NodePtr, ComponentPtr>& light) {
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
    
    gluLookAt(15.0f*sinf(angle/2), 0.0f, 15.0f*cosf(angle/2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    //gluLookAt(10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    // Set light parameter
    const Color& diffuse = light.second->value("diffuse");
    const Color& specular = light.second->value("specular");
    const Color& ambient = light.second->value("ambient");
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    
    // Set light location/direction.
    const Vector& light_pos = light.first->position();
    static GLfloat lposition[4] = { light_pos.x, light_pos.y, light_pos.z };
    if (light.second->type() == "PointLight") {
        lposition[3] = 1.0f;
    } else {
        lposition[3] = 0.0f;
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    glEnable(GL_LIGHT0);
    glPopMatrix();
    
    // Render to the back buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShaderPtr basic = shader("Basic");
    basic->begin();
    render_visible_objects();
    basic->end();
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

    //glutSolidTorus(1.0f, 2.0f, 32, 32);
    glFrontFace(GL_CW);
    glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glTranslatef(5.0f, 0.0f, 0.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(5.0f, 0.0f, 0.0f);
    //glutSolidTeapot(1.0f);
    glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    
    glFrontFace(GL_CCW);
    for (Iterator<const pair<NodePtr, ComponentPtr>> i = engine_->renderables(); i; i++) {
        MeshBufferPtr buffer = mesh(i->second->value("mesh"));
        buffer->render();
    }
    glFrontFace(GL_CW);
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

MeshBuffer* Renderer::mesh(const std::string& name) {
    map<string, MeshBufferPtr>::iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        engine_->resource(name);
        MeshPtr mesh(engine_->mesh(name));
        MeshBufferPtr mesh_buffer(new MeshBuffer(mesh.get()));
        mesh_.insert(make_pair(name, mesh_buffer));
        return mesh_buffer.get();
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

