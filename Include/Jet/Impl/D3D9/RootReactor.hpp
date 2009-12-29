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
#include <vector>

#ifndef WINDOWS
#error This module requires Microsoft Windows
#endif

namespace Jet { namespace Impl { namespace D3D9 {
using namespace std;
using namespace std::tr1;
using namespace boost;

class RootReactor : public Root::Observer, public Video::Observer {
public:
    typedef intrusive_ptr<RootReactor> Ptr; 
    RootReactor(Root::Ptr e);
    ~RootReactor();
    
    void onActorNew(Actor::Ptr) {}
    void onModelNew(Model::Ptr);
    void onEntityNew(Entity::Ptr) {}
    void onCameraNew(Camera::Ptr) {}
    void onQuadNew(Quad::Ptr);
    void onListenerNew(Listener::Ptr) {}
    void onSpeakerNew(Speaker::Ptr) {}
    void onCloudNew(Cloud::Ptr);
    void onTime();
    void onQuality();
    void onAntialiasing();
    void onBloom();
    void onState();

    inline const D3DXMATRIX& viewMatrix() const { return viewMatrix_; }
    inline const D3DXMATRIX& projectionMatrix() const { return projectionMatrix_; }
    inline const DevicePtr device() const { return device_; }

private:
    Root::Ptr root_;

    Direct3DPtr direct3d_;
    DeviePtr device_;
    D3DPRESENT_PARAMS presentParams;
    
		//DWORD									m_multisample_quality;
		//D3DMULTISAMPLE_TYPE						m_multisample_type
    
    D3DXMATRIX viewMatrix_;
    D3DXMATRIX projectionMatrix_;
};

}}}
