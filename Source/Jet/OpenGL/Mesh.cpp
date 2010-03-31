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

#include <Jet/OpenGL/Mesh.hpp>
#include <Jet/Mesh.hpp>

using namespace Jet::OpenGL;

Mesh::Mesh(Jet::Mesh* mesh) :
    nvertices_(mesh->vertex_count()) {
    
    glGenBuffers(1, &vbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glBufferData(GL_ARRAY_BUFFER, nvertices_*sizeof(Vertex), mesh->vertex_data(), GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbuffer_);
}

void Mesh::render() const {
    // GLuint b = getBinormalLoc

    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableVertexAttribArray(g)

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(GLfloat)));
    //glVertexAttribPointer(b, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(6*sizeof(GLfloat)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(9*sizeof(GLfloat)));

    // TODO: Draw indexed, not raw
    glDrawArrays(GL_TRIANGLES, 0, nvertices_);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableVertexAttributeArray(b);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}






