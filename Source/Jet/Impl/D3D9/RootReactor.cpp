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

#include <Jet/Impl/D3D9/RootReactor.hpp>
#include <Jet/Impl/D3D9/TextureReactor.hpp>
#include <stdexcept>
#include <sstream>

#define WINDOWED_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
#define FULLSCREEN_STYLE WS_POPUP

using namespace Jet;
using namespace Jet::Impl::D3D9;

//------------------------------------------------------------------------------
extern "C" JETAPI Interface*
moduleLoad(Root* r) {  
    return new RootReactor(r);
}

//------------------------------------------------------------------------------
RootReactor::RootReactor(Root::Ptr e) :
    root_(e) {
    
    if (!direct3d_) logic_error("Could not launch graphics system.");

    root_->publisher().observerAdd(this);
    root_->options()->publisher().observerAdd(this);
    root_->loader()->publisher().observerAdd(this);
    
    initWindow();
    initDevice();
}

//------------------------------------------------------------------------------
RootReactor::~RootReactor() {

    root_->publisher().observerDel(this);
    root_->options()->publisher().observerDel(this);
    root_->loader()->publisher().observerDel(this);
    
    effectPool_ = 0;
    device_->Release();
    direct3d_->Release();
    DestroyWindow(window_);

}

//------------------------------------------------------------------------------
void 
RootReactor::onVideoMode() {

    root_->loader()->videoLoadStatus(Loader::statusUnloading);
    resetWindow();
    checkOptions();
    device_->Reset(&presentParams_);
    root_->loader()->videoLoadStatus(Loader::statusLoading);
}
    
//------------------------------------------------------------------------------
void 
RootReactor::onModelNew(Model::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onQuadNew(Quad::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onCloudNew(Cloud::Ptr) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onFrame() {

}
    
//------------------------------------------------------------------------------
void 
RootReactor::onTextureNew(Texture::Ptr o) {
    reactors_.push_back(new TextureReactor(o, this));
}

//------------------------------------------------------------------------------
void 
RootReactor::onCubemapNew(Cubemap::Ptr o) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onMeshNew(Mesh::Ptr o) {

}

//------------------------------------------------------------------------------
void 
RootReactor::onShaderNew(Shader::Ptr o) {

}

//------------------------------------------------------------------------------
LRESULT CALLBACK
RootReactor::onMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//------------------------------------------------------------------------------
void
RootReactor::initWindow() {
    
    // We need the "module handle" (ie, exe or dll process handle) to
	// create a window.
	HINSTANCE handle = GetModuleHandle(NULL);

	// Specify window options
	WNDCLASS wclass;
	wclass.style			= CS_HREDRAW | CS_VREDRAW;
	wclass.lpfnWndProc		= &RootReactor::onMessage;
	wclass.cbClsExtra		= 0;
	wclass.cbWndExtra		= 0;
	wclass.hInstance		= handle;
	wclass.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wclass.hCursor			= LoadCursor(0, IDC_ARROW);
	wclass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wclass.lpszMenuName		= 0;
	wclass.lpszClassName	= "JetEngineWindow";

	// Register the options
	if (!RegisterClass(&wclass)) {
		logic_error("Could not create window.");
	}

	// Create a window of the appropriate size, and adjust the window rectangle
	// so that our viewing area is 800x600 (NOT including the borders)
	const VideoMode& mode = root_->options()->videoMode();
	
	if (VideoMode::modeWindowed == mode.windowMode_) {
	    RECT rect = {0, 0, mode.width_, mode.height_};	
	    AdjustWindowRect(&rect, WINDOWED_STYLE, false);
	    window_ = CreateWindow(
	                "JetEngineWindow", "Extreme Asteroids", 
	                WINDOWED_STYLE, 0, 0, rect.right - rect.left, 
	                rect.bottom - rect.top, NULL, NULL, handle, NULL);
	} else {
	    window_ = CreateWindow(
	                "JetEngineWindow", "Extreme Asteroids", 
	                FULLSCREEN_STYLE, 0, 0, GetSystemMetrics(SM_CXSCREEN),
	                GetSystemMetrics(SM_CYSCREEN), NULL, NULL, handle, NULL);
	}
	
	if (!window_) {
		logic_error("Could not create window.");
	}

	// Show the window and update our options.
	ShowWindow(window_, SW_SHOW);
	UpdateWindow(window_);
}

//------------------------------------------------------------------------------
void
RootReactor::initDevice() {

    direct3d_ = Direct3DCreate9(D3D_SDK_VERSION);

    // Default presentation parameters
    presentParams_.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParams_.hDeviceWindow = window_;
    presentParams_.AutoDepthStencilFormat = D3DFMT_D24S8;
    presentParams_.Flags = 0;
    presentParams_.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    presentParams_.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    
    // Check for hardware vertex processing
    D3DCAPS9 caps;
    HR(direct3d_->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));
    DWORD vertexFlags = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
        vertexFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
        if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
            vertexFlags |= D3DCREATE_PUREDEVICE;
        }
    } else {
        vertexFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    
    // Check optional parameters
    checkOptions();
    
    // Create the device
    HR(direct3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_, vertexFlags, &presentParams_, &device_));
}

//------------------------------------------------------------------------------
void
RootReactor::resetWindow() {
    const VideoMode& mode = root_->options()->videoMode();

    if (VideoMode::modeWindowed == mode.windowMode_) {        
        RECT rect = {0, 0, mode.width_, mode.height_};	
	    AdjustWindowRect(&rect, WINDOWED_STYLE, false);
        SetWindowLongPtr(window_, GWL_STYLE, WINDOWED_STYLE);
        SetWindowPos(
            window_, HWND_TOP, 0, 0, rect.right - rect.left, 
            rect.bottom - rect.top, SWP_NOZORDER | SWP_SHOWWINDOW);  
    } else { 
        SetWindowLongPtr(window_, GWL_STYLE, FULLSCREEN_STYLE);
        SetWindowPos(
            window_, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
            GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_SHOWWINDOW); 
    }
}

//------------------------------------------------------------------------------
void
RootReactor::checkOptions() {
    const VideoMode& mode = root_->options()->videoMode();
    
    presentParams_.BackBufferWidth = mode.width_;
    presentParams_.BackBufferHeight = mode.height_;
    presentParams_.BackBufferCount = 1;   
    presentParams_.EnableAutoDepthStencil = true; // ! m_hdr_enabled
    
    checkFormat();
    checkShaderModel();
    checkAntialiasing();
}

//------------------------------------------------------------------------------
void
RootReactor::checkFormat() {
    const VideoMode& mode = root_->options()->videoMode();
    
    // Set back buffer and device format
    if (VideoMode::modeWindowed == mode.windowMode_) {
        D3DDISPLAYMODE mode;
        HR(direct3d_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));
        presentParams_.BackBufferFormat = mode.Format;
        presentParams_.Windowed = true; 
        
    } else {
        presentParams_.BackBufferFormat = D3DFMT_X8R8G8B8;
        presentParams_.Windowed = false;
    }
    
    // Check device type
    HR(direct3d_->CheckDeviceType(
        D3DADAPTER_DEFAULT, 
        D3DDEVTYPE_HAL, 
        presentParams_.BackBufferFormat, 
        presentParams_.BackBufferFormat, 
        presentParams_.Windowed));
        
    // Check depth stencil match
    HR(direct3d_->CheckDepthStencilMatch(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        presentParams_.BackBufferFormat,
        presentParams_.BackBufferFormat,
        presentParams_.AutoDepthStencilFormat));
}

//------------------------------------------------------------------------------
void
RootReactor::checkShaderModel() {
    const VideoMode& mode = root_->options()->videoMode();
    
    // Check for shader model
    if (VideoMode::shadersEnabled == mode.shaderMode_) {
        D3DCAPS9 caps;
        HR(direct3d_->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));
        if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
            logic_error("Hardware does not support Shader Model 2.0");
        }
    }
}
    
//------------------------------------------------------------------------------
void
RootReactor::checkAntialiasing() {
    const VideoMode& mode = root_->options()->videoMode();
    
    // Check for antialiasing
    if (VideoMode::antialiasEnabled == mode.antialiasing_) {
        presentParams_.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    
        HR(direct3d_->CheckDeviceMultiSampleType(
            D3DADAPTER_DEFAULT, 
            D3DDEVTYPE_HAL, 
            presentParams_.BackBufferFormat, 
            presentParams_.Windowed,
            presentParams_.MultiSampleType,
            &presentParams_.MultiSampleQuality))
            
        presentParams_.MultiSampleQuality--;
    } else {
        presentParams_.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentParams_.MultiSampleQuality = 0;
    }
}

//------------------------------------------------------------------------------
void
RootReactor::onVideoLoadStatus() {
    Loader::Ptr loader = root_->loader();

    if (Loader::statusLoading == loader->videoLoadStatus()) {
        for (Loader::TextureItr i = loader->textureItr(); i; i++) {
            (*i)->loadStatus(Resource::statusLoading);
        }
    
        loader->videoLoadStatus(Loader::statusLoaded);
    } else if (Loader::statusUnloading == loader->videoLoadStatus()) {
        //for (Loader::TextureItr i = loader->textureItr(); i; i++) {
        //    (*i)->loadStatus(Resource::statusUnloading);
        //}
    
    
        loader->videoLoadStatus(Loader::statusUnloaded);
    }    
}
