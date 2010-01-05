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

#include <Jet/Root.hpp>
#include <Jet/Impl/D3D9/Types.hpp>
#include <Jet/Impl/D3D9/EffectPool.hpp>
#include <vector>

#ifndef WINDOWS
#error This module requires Microsoft Windows
#endif

namespace Jet { namespace Impl { namespace D3D9 {
using namespace std;
using namespace std::tr1;
using namespace boost;

class RootReactor : public Root::Observer, public Loader::Observer, public Options::Observer {
public:
    typedef intrusive_ptr<RootReactor> Ptr; 
    RootReactor(Root::Ptr e);
    ~RootReactor();
    
    virtual void onVideoMode();
    virtual void onModelNew(Model::Ptr);
    virtual void onQuadNew(Quad::Ptr);
    virtual void onCloudNew(Cloud::Ptr);
    virtual void onFrame();
    virtual void onTextureNew(Texture::Ptr o);
    virtual void onCubemapNew(Cubemap::Ptr o);
    virtual void onMeshNew(Mesh::Ptr o);
    virtual void onShaderNew(Shader::Ptr o);
    virtual void onVideoLoadStatus();
    
    static LRESULT CALLBACK onMessage(HWND, UINT, WPARAM, LPARAM);
    
    inline const D3DXMATRIX& viewMatrix() const { return viewMatrix_; }
    inline const D3DXMATRIX& projectionMatrix() const { return projectionMatrix_; }
    inline IDirect3DDevice9* device() const { return device_; }
    inline HWND window() const { return window_; }

private:
    void initWindow();
    void initDevice();
    void checkOptions();
    void checkFormat();
    void checkAntialiasing();
    void checkShaderModel();
    void resetWindow();

    Root::Ptr root_;
    IDirect3D9* direct3d_;
    IDirect3DDevice9* device_;
    HWND window_;
    EffectPool::Ptr effectPool_;
    D3DPRESENT_PARAMETERS presentParams_;
    vector<Interface::Ptr> reactors_;
    
		//DWORD									m_multisample_quality;
		//D3DMULTISAMPLE_TYPE						m_multisample_type
    
    D3DXMATRIX viewMatrix_;
    D3DXMATRIX projectionMatrix_;
};

}}}
