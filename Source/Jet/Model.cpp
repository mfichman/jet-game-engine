/*
 * Copyright (c) 2008 Matt Fichman
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

#include <Jet/Model.hpp>

using namespace Jet;

void 
Model::scale(const Vector& s) {
    if (scale_ != s) {
        scale_ = s;
        publisher_.notify(&Listener::onScale);
    }
}
void 
Model::texture(TextureIndex j, const string& t) {
    if (texture_[j] != t) {
        texture_[j] = t;
        publisher_.notify(&Listener::onTexture, j);
    }
}

void 
Model::cubemap(CubemapIndex j, const string& t) {
    if (cubemap_[j] != t) {
        cubemap_[j] = t;
        publisher_.notify(&Listener::onCubemap, j);
    }
}

void 
Model::shader(const string& s) { 
    if (shader_ != s) {
        shader_ = s;    
        publisher_.notify(&Listener::onShader);
    }
}

void 
Model::mesh(const string& m) {
    if (mesh_ != m) {
        mesh_ = m;
        publisher_.notify(&Listener::onMesh);
    }
}
