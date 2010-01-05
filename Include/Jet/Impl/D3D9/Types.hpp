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

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define D3D9_DEBUG_INFO
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <sstream>

namespace Jet { namespace Impl { namespace D3D9 {
using namespace std;
using namespace std::tr1;

//typedef intrusive_ptr<IDirect3DSurface9> SurfacePtr;
//typedef intrusive_ptr<IDirect3DTexture9> TexturePtr;
//typedef intrusive_ptr<IDirect3DCubeTexture9> CubemapPtr;
//typedef intrusive_ptr<ID3DXMesh> MeshPtr;
//typedef intrusive_ptr<ID3DXEffect> EffectPtr;

struct FrameParameters {
	D3DXCOLOR diffuseLight_;
	D3DXCOLOR ambientLight_;
	D3DXCOLOR specularLight_;
	D3DXVECTOR3	lightDirection_;
	D3DXVECTOR3	cameraEye_;
	D3DXVECTOR3	cameraTarget_;
	D3DXVECTOR3	cameraUp_;
	float time_;
	float bufferHeight_;
	D3DXMATRIX viewProj_;
};

struct ScreenVertex {
	D3DXVECTOR4 pos_;
	D3DXVECTOR2 tex_;
	static const DWORD FVF;
};

struct TextureRect {
	float leftu;
	float topv;
	float rightu;
	float bottomv;
};

//------------------------------------------------------------------------------
#define HR(x) {														\
	HRESULT hr = x;													\
	if (FAILED(hr)) {												\
		std::ostringstream os;									    \
		os << DXGetErrorDescription(hr) << ": ";          			\
		os << __FILE__ << ":" << __LINE__;							\
		throw domain_error(os.str());								\
	}																\
}

//------------------------------------------------------------------------------
#define RELEASE(x)                                                  \
    if (x) {                                                        \
        x->Release();                                               \
        x = 0;                                                      \
    }

}}}
