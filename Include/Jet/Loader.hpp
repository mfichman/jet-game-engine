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

#include <Jet/Types.hpp>
#include <Jet/Publisher.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Cubemap.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Shader.hpp>
#include <map>

namespace Jet {
using namespace std;
using namespace std::tr1;
class Root;

class Loader : public Interface {
public:
    class Listener;
    friend class Root;
	typedef intrusive_ptr<Loader> Ptr;

    // Attributes
    Texture::Ptr    textureNew(const string& o);
    Cubemap::Ptr    cubemapNew(const string& o);
    Mesh::Ptr       meshNew(const string& o);
    Shader::Ptr     shaderNew(const string& o);

    // Utility
    Publisher<Listener>& publisher() const { return publisher_; }

private:
    Loader() {}
    
    mutable Publisher<Listener> publisher_;
};

class Loader::Listener : public Interface {
public:
    typedef intrusive_ptr<Loader::Listener> Ptr;

    virtual void onTextureNew(Texture::Ptr o)=0;
    virtual void onCubemapNew(Cubemap::Ptr o)=0;
    virtual void onMeshNew(Mesh::Ptr o)=0;
    virtual void onShaderNew(Shader::Ptr o)=0;
};

}
