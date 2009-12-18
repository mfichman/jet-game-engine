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
#pragma once
 
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Renderable.hpp>
#include <Jet/Publisher.hpp>
#include <list>

namespace Jet {
using namespace std;
using namespace std::tr1;
class Root;

class Model : public Interface {
public:   
    class Listener;
    friend class Root;
    typedef intrusive_ptr<Model> Ptr;
    typedef RangedOrdinal<int, 0, 3> TextureIndex;
    typedef RangedOrdinal<int, 0, 1> CubemapIndex;

    // Attributes
    inline Vector           scale() const { return scale_; }
    void                    scale(const Vector& s);
    inline const string& 	texture(TextureIndex i) const { return texture_[i]; }
    void 		            texture(TextureIndex i, const string& t);
    inline const string& 	cubemap(CubemapIndex i) const { return cubemap_[i]; }
    void 		            cubemap(CubemapIndex i, const string& t);
    inline const string& 	shader() const { return shader_; }
    void 		            shader(const string& s);
    inline const string&    mesh() const { return mesh_; }
    void                    mesh(const string& m);

    // Components
    inline Renderable::Ptr  renderable() const { return renderable_; }
    inline Object::Ptr      object() const { return object_; }    

    // Utility
    inline Publisher<Listener>& publisher() const { return publisher_; }

private:
    Model() : object_(new Object) {}

    mutable Publisher<Listener> publisher_;
    Renderable::Ptr renderable_;
    Object::Ptr     object_;
    Vector          scale_;
    string          texture_[TextureIndex::maxValue];
    string          cubemap_[CubemapIndex::maxValue]; 
    string          shader_;   
    string          mesh_;
};

class Model::Listener : public Interface {
public:
    typedef intrusive_ptr<Model::Listener> Ptr;

    virtual void onScale()=0;
    virtual void onTexture(TextureIndex i)=0;
    virtual void onCubemap(CubemapIndex i)=0;
    virtual void onShader()=0;
    virtual void onMesh()=0;
};

}
