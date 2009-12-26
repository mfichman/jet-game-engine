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
Texture::Ptr 
Loader::textureNew(const string& o) {
    map<string, Texture::Ptr>::iterator i = texture_.find(o);
    if (i == texture_.end()) {
        Texture::Ptr t = new Texture(o);
        texture_[o] = t;
        publisher_.notify(&Observer::onTextureNew, t);
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
Cubemap::Ptr
Loader::cubemapNew(const string& o) {
    map<string, Cubemap::Ptr>::iterator i = cubemap_.find(o);
    if (i == cubemap_.end()) {
        Cubemap::Ptr c = new Cubemap(o);
        cubemap_[o] = c;
        publisher_.notify(&Observer::onCubemapNew, c);
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
Mesh::Ptr       
Loader::meshNew(const string& o) {
    map<string, Mesh::Ptr>::iterator i = mesh_.find(o);
    if (i == mesh_.end()) {
        Mesh::Ptr m = new Mesh(o);
        mesh_[o] = m;
        publisher_.notify(&Observer::onMeshNew, m);
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
Shader::Ptr     
Loader::shaderNew(const string& o) {
    map<string, Shader::Ptr>::iterator i = shader_.find(o);
    if (i == shader_.end()) {
        Shader::Ptr s = new Shader(o);
        shader_[o] = s;
        publisher_.notify(&Observer::onShaderNew, s);
    } else {
        return i->second;
    }
}

#include <cerrno>
#include <cstring>
//------------------------------------------------------------------------------
Module::Ptr
Loader::moduleNew(const string& o) {
    map<string, Module::Ptr>::iterator i = module_.find(o);
    if (i == module_.end()) {

#ifdef WINDOWS
        HMODULE handle = LoadLibrary(o.c_str());
        if (!handle) {
            throw std::range_error("Could not find module " + o);
        }
        ModuleLoadFn load = (ModuleLoadFn)GetProcAddress(handle, "moduleLoad");
        if (!load) {
            throw std::range_error("Could not find module load function for " + o);
        }
#else
        void *handle = dlopen(o.c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::range_error("Could not find module " + o);
        } 
        ModuleLoadFn load = (ModuleLoadFn)dlsym(handle, "moduleLoad");
        if (!handle) {
            throw std::range_error("Could not find module load function for " + o);
        }
#endif
        Module::Ptr module = new Module(o, load(root_), (void*)handle);
        module_[o] = module;
        publisher_.notify(&Observer::onModuleNew, module);
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
void
Loader::moduleDel(const string& o) {
    map<string, Module::Ptr>::iterator i = module_.find(o);
 
cout << ";dlkf;lk" << endl;
   if (i != module_.end()) {
#ifdef WINDOWS

#else
        cout << "shutdown" << endl;
        
        void* handle = i->second->handle();
        module_.erase(i);
        dlclose(handle);        
#endif
    }

}
