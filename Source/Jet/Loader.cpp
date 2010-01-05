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

#include <Jet/Loader.hpp>
#include <map>
#include <iostream>

#ifdef WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace Jet;
using namespace std;

//------------------------------------------------------------------------------
template <typename T>
typename T::Ptr Loader::objectNew(
    const string& name, 
    map<string, typename T::Ptr>& m,
    void (Loader::Observer::*fn)(typename T::Ptr)) {

    typename map<string, typename T::Ptr>::iterator i = m.find(name);
    if (i == m.end()) {
        typename T::Ptr t = new T(name);
        m[name] = t;
        publisher_.notify(fn, t);
        return t;
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
Loader::Loader(Root* r) : 
    root_(r),
    videoLoadStatus_(statusUnloaded),
    audioLoadStatus_(statusUnloaded) {
}

//------------------------------------------------------------------------------
Texture::Ptr 
Loader::textureNew(const string& o) {
    return objectNew<Texture>(o, texture_, &Observer::onTextureNew);
}

//------------------------------------------------------------------------------
Cubemap::Ptr
Loader::cubemapNew(const string& o) {
    return objectNew<Cubemap>(o, cubemap_, &Observer::onCubemapNew);
}

//------------------------------------------------------------------------------
Mesh::Ptr       
Loader::meshNew(const string& o) {
    return objectNew<Mesh>(o, mesh_, &Observer::onMeshNew);
}

//------------------------------------------------------------------------------
Shader::Ptr     
Loader::shaderNew(const string& o) {
    return objectNew<Shader>(o, shader_, &Observer::onShaderNew);
}

//------------------------------------------------------------------------------
Sound::Ptr
Loader::soundNew(const string& o) {
    return objectNew<Sound>(o, sound_, &Observer::onSoundNew);
}

//------------------------------------------------------------------------------
Module::Ptr
Loader::moduleNew(const string& o) {
    map<string, Module::Ptr>::iterator i = module_.find(o);
    if (i == module_.end()) {

#ifdef WINDOWS
        HMODULE handle = LoadLibrary(o.c_str());
        if (!handle) {
            throw range_error("Could not load module " + o);
        }
        ModuleLoadFn load = (ModuleLoadFn)GetProcAddress(handle, "moduleLoad");
        if (!load) {
            throw range_error("Could not load module " + o + ": no 'moduleLoad' symbol");
        }
#else
        void *handle = dlopen(o.c_str(), RTLD_LAZY);
        if (!handle) {
            throw range_error("Could not load module " + o + ": " + dlerror());
        } 
        ModuleLoadFn load = (ModuleLoadFn)dlsym(handle, "moduleLoad");
        if (!handle) {
            throw range_error("Could not load module " + o + ": " + dlerror());
        }
#endif
        Module::Ptr module = new Module(o, load(root_), (void*)handle);
        
        module_[o] = module;
        publisher_.notify(&Observer::onModuleNew, module);
        return module;
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
void
Loader::moduleDel(const string& o) {
    map<string, Module::Ptr>::iterator i = module_.find(o);

   if (i != module_.end()) {
#ifdef WINDOWS
        HMODULE handle = (HMODULE)i->second->handle();
        module_.erase(i);
        if (!FreeLibrary(handle)) {
            throw range_error("Could not unload module " + o);
        }
#else   
        void* handle = i->second->handle();
        module_.erase(i);
        if (dlclose(handle)) {
            throw range_error(dlerror());
        }        
#endif
    } else {
        throw range_error("Module is not loaded");
    }

}

//------------------------------------------------------------------------------
void
Loader::videoLoadStatus(Status s) {
    if (s != videoLoadStatus_) {
        videoLoadStatus_ = s;
        publisher_.notify(&Observer::onVideoLoadStatus);
    }
}

//------------------------------------------------------------------------------
void
Loader::audioLoadStatus(Status s) {
    if (s != audioLoadStatus_) {
        audioLoadStatus_ = s;
        publisher_.notify(&Observer::onAudioLoadStatus);
    }
}
