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

namespace Jet {
using namespace std;
using namespace std::tr1;
class Root;

class Quad : public Interface {
public:
    class Listener;
    friend class Root;
    typedef RangedOrdinal<int, 0, 3> VertexIndex;
    typedef RangedOrdinal<int, 0, 3> TextureIndex;
    typedef intrusive_ptr<Quad> Ptr;

    // Attributes
    inline Vector           scale() const { return scale_; }
    void                    scale(const Vector& s);
    inline const string& 	texture(TextureIndex i) const { return texture_[i]; }
    void 		            texture(TextureIndex i, const string& t);
    inline Vertex           vertex(VertexIndex i) { return vertex_[i]; }
    void                    vertex(VertexIndex i, const Vertex& v);
    inline Coord            texCoordScale() const { return texCoordScale_; }
    void                    texCoordScale(Coord c);
    
    // Components
    Object::Ptr             object() const { return object_; }
    Renderable::Ptr         renderable() const { return renderable_; }

    // Utility
    inline Publisher<Listener>& publisher() const { return publisher_; }

protected:
    Quad() : object_(new Object) {}

    mutable Publisher<Listener> publisher_;
    Object::Ptr object_;
    Renderable::Ptr renderable_;
    Vector scale_; 
    string texture_[TextureIndex::maxValue];
    Vertex vertex_[VertexIndex::maxValue];
    Coord texCoordScale_;      
};

class Quad::Listener : public Interface {
public:
    virtual void onScale()=0;
    virtual void onTexture(TextureIndex i)=0;
    virtual void onVertex(VertexIndex i)=0;
    virtual void onTexCoordScale()=0;
};

}
