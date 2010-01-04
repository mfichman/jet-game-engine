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
#pragma once
 
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Publisher.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;
class Root;

class JETAPI Quad : public Interface {
public:
    class Observer;
    friend class Root;
    typedef RangedOrdinal<int, 0, 3> VertexIndex;
    typedef intrusive_ptr<Quad> Ptr;
    enum State { stateDisabled, stateEnabled };

    // Attributes
    inline Vertex               vertex(VertexIndex i) { return vertex_[i]; }
    void                        vertex(VertexIndex i, const Vertex& v);
    inline TexCoord             texCoordScale() const { return texCoordScale_; }
    void                        texCoordScale(TexCoord c);
    inline const string& 	    texture() const { return texture_; }
    void 		                texture(const string& t);
    inline const Vector&        scale() const { return scale_; }
    void                        scale(const Vector& s);
    inline const string& 	    shader() const { return shader_; }
    void 		                shader(const string& s);
    inline State                state() const { return state_; }
    void                        state(State v);
    inline void                 operator()(Object::Functor& f) { f(this); }

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; }

private:
    Quad() {}

    mutable Publisher<Observer> publisher_;

    Vertex vertex_[VertexIndex::maxValue];
    TexCoord texCoordScale_; 
    string texture_;
    Vector scale_;
    string shader_; 
    State state_;
};

class Quad::Observer : public Interface {
public:
    typedef intrusive_ptr<Quad::Observer> Ptr;

    virtual void onVertex(VertexIndex i) {}
    virtual void onTexCoordScale() {}
    virtual void onTexture() {}
    virtual void onScale() {}
    virtual void onShader() {}
    virtual void onState() {}   
};

}
