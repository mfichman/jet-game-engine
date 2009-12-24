/*
 * Copyright (c) 2009 Matt Fichman
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

#include <Jet/Quad.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void 
Quad::scale(const Vector& s) {
    if (s != scale_) {
        scale_ = s;
        publisher_.notify(&Observer::onScale);
    }
}

//------------------------------------------------------------------------------
void 		            
Quad::texture(TextureIndex i, const string& t) {
    if (t != texture_[i]) {
        texture_[i] = t;
        publisher_.notify(&Observer::onTexture, i);
    }
}

//------------------------------------------------------------------------------
void                    
Quad::vertex(VertexIndex i, const Vertex& v) {
    if (v != vertex_[i]) {
        vertex_[i] = v;
        publisher_.notify(&Observer::onVertex, i);
    }
}

//------------------------------------------------------------------------------
void                    
Quad::texCoordScale(Coord c) {
    if (c != texCoordScale_) {
        texCoordScale_ = c;
        publisher_.notify(&Observer::onTexCoordScale);
    }

}
