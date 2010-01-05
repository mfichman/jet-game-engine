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
#include <Jet/Publisher.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Cubemap.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Shader.hpp>
#include <Jet/Module.hpp>
#include <Jet/Sound.hpp>
#include <Jet/Iterator.hpp>
#include <map>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class JETAPI Loader : public Interface {
public:
    class Observer;
    friend class Root;
	typedef intrusive_ptr<Loader> Ptr;  
    typedef Interface* (*ModuleLoadFn)(Root*);
    typedef Iterator<map<string, Texture::Ptr> > TextureItr;
    enum Status { statusUnloading, statusUnloaded, statusLoading, statusLoaded };

    // Attributes
    Texture::Ptr        textureNew(const string& o);
    Cubemap::Ptr        cubemapNew(const string& o);
    Mesh::Ptr           meshNew(const string& o);
    Shader::Ptr         shaderNew(const string& o);
    Module::Ptr         moduleNew(const string& o);
    Sound::Ptr          soundNew(const string& o);
    void                moduleDel(const string& o);

    Texture::Ptr        texture(const string& o) { return texture_[o]; }
    Cubemap::Ptr        cubemap(const string& o) { return cubemap_[o]; }
    Mesh::Ptr           mesh(const string& o) { return mesh_[o]; }
    Shader::Ptr         shader(const string& o) { return shader_[o]; }
    Module::Ptr         module(const string& o) { return module_[o]; }
    Sound::Ptr          sound(const string& o) { return sound_[o]; };
    
    inline TextureItr   textureItr() { return TextureItr(texture_); }
    
    inline Status       videoLoadStatus() const { return videoLoadStatus_; }
    void                videoLoadStatus(Status s);
    inline Status       audioLoadStatus() const { return audioLoadStatus_; }
    void                audioLoadStatus(Status s);
    

    // Utility
    Publisher<Observer>& publisher() const { return publisher_; }

private:
    Loader(Root* r);

    template <typename T>
    inline typename T::Ptr objectNew(
        const string&, 
        map<string, typename T::Ptr>&, 
        void (Loader::Observer::*)(typename T::Ptr)); 
    
    mutable Publisher<Observer> publisher_;
    map<string, Texture::Ptr> texture_;
    map<string, Cubemap::Ptr> cubemap_;
    map<string, Mesh::Ptr> mesh_;
    map<string, Shader::Ptr> shader_;
    map<string, Module::Ptr> module_;
    map<string, Sound::Ptr> sound_;
    Root* root_;
    Status videoLoadStatus_;
    Status audioLoadStatus_;
};

class Loader::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Loader::Observer> Ptr;

    virtual void onTextureNew(Texture::Ptr o) {}
    virtual void onCubemapNew(Cubemap::Ptr o) {}
    virtual void onMeshNew(Mesh::Ptr o) {}
    virtual void onShaderNew(Shader::Ptr o) {}
    virtual void onModuleNew(Module::Ptr o) {}
    virtual void onSoundNew(Sound::Ptr o) {}
    virtual void onVideoLoadStatus() {}
    virtual void onAudioLoadStatus() {}
};

}
