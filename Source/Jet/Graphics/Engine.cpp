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

#include <Jet/Graphics/Engine.hpp>
#include <Jet/Graphics/Texture.hpp>
#include <Jet/Graphics/Cubemap.hpp>
#include <Jet/Graphics/Mesh.hpp>
#include <Jet/Graphics/Shader.hpp>
#include <Jet/Graphics/Quad.hpp>
#include <Jet/Graphics/Model.hpp>
#include <Jet/Graphics/TextBox.hpp>

using namespace Jet;
using namespace Jet::Graphics;
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;

shared_ptr<Texture>
Engine::textureCreate(const string& name) {
    Texture::Ptr t(new Texture(name), bind(&Engine::textureDestroy, this, _1));
    texture_[name] = t.get();
    
    for (ListenerItr i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->onTextureCreate(t);
    }
    
    return t;
}

shared_ptr<Cubemap>
Engine::cubemapCreate(const string& name) {
    Cubemap::Ptr t(new Cubemap(name), bind(&Engine::cubemapDestroy, this, _1));
    cubemap_[name] = t.get();
    
    for (ListenerItr i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->onCubemapCreate(t);
    }
    
    return t;
}

shared_ptr<Mesh>
Engine::meshCreate(const string& name) {
    Mesh::Ptr t(new Mesh(name), bind(&Engine::meshDestroy, this, _1));
    mesh_[name] = t.get();
    
    for (ListenerItr i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->onMeshCreate(t);
    }
    

    return t;
}

shared_ptr<Shader>
Engine::shaderCreate(const string& name) {
    Shader::Ptr t(new Shader(name), bind(&Engine::shaderDestroy, this, _1));
    shader_[name] = t.get();
    
    for (ListenerItr i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->onShaderCreate(t);
    }
    
    return t;
}

shared_ptr<Quad>
Engine::quadCreate() {
    Quad::Ptr t(new Quad());
    quad_.push_back(t.get());
    return t;
}

shared_ptr<Model>
Engine::modelCreate() {
    Model::Ptr t(new Model());
    model_.push_back(t.get());
    return t;
}

shared_ptr<TextBox>
Engine::textBoxCreate() {
    TextBox::Ptr t(new TextBox());
    textBox_.push_back(t.get());
    return t;
}

void
Engine::textureDestroy(Texture* t)
{

}

void
Engine::cubemapDestroy(Cubemap* c)
{

}

void
Engine::meshDestroy(Mesh* m)
{

}

void
Engine::shaderDestroy(Shader* s)
{

}

void
Engine::quadDestroy(Quad* s)
{

}

void
Engine::modelDestroy(Model* m)
{

}

void
Engine::textBoxDestroy(TextBox* t)
{

}


