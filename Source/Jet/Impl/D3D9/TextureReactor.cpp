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

#include <Jet/Impl/D3D9/TextureReactor.hpp>
#include <Jet/Impl/D3D9/RootReactor.hpp>

using namespace Jet;
using namespace Jet::Impl::D3D9;

//------------------------------------------------------------------------------  
TextureReactor::TextureReactor(Texture::Ptr m, RootReactor* r) :
    texture_(m),
    rootReactor_(r),
    handle_(0),
    file_(0),
    mapping_(0) {

    texture_->publisher().observerAdd(this); 
    onLoadStatus(); 
    
    // Open the file!
    if (!(file_ = CreateFileA(texture_->name().c_str(), GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 0))) {
        
        domain_error("Failed to read '" + texture_->name() + "'");
    }
    
    // Create a file mapping object
    if (!(mapping_ = CreateFileMappingA(file_, 0, PAGE_READONLY, 0, 0, 0))) {
        domain_error("Failed to map '" + texture_->name() + "'");
    }
    
    // Map the file to memory
    if (!(data_ = MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0))) {
        domain_error("Failed to map '" + texture_->name() + "'");
    }
    
}

//------------------------------------------------------------------------------
TextureReactor::~TextureReactor() {
    texture_->publisher().observerDel(this);
    texture_->loadStatus(Texture::statusUnloading);
    
    if (file_) {
        CloseHandle(file_);
    }
    
    if (data_) {
        UnmapViewOfFile(data_);
    }
    
    if (mapping_) {
        CloseHandle(mapping_);
    }
}

//------------------------------------------------------------------------------
void 
TextureReactor::onLoadStatus() {
    if (Resource::statusLoading == texture_->loadStatus()) {    
        texture_->handle(0);
        RELEASE(handle_);
        
        cout << "Loading " << texture_->name() << endl;
        
        D3DXIMAGE_INFO info;
        DWORD size = GetFileSize(file_, 0);
        HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
            rootReactor_->device(), data_, size, D3DX_DEFAULT, D3DX_FROM_FILE,
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, &info, NULL, &handle_);
            
            
            
        /*HRESULT hr = D3DXCreateTextureFromFileExA(
            rootReactor_->device(), texture_->name().c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, &info, NULL, &handle_); */
            
        if (FAILED(hr)) {
            ostringstream os;
            os << "Failed to load '" << texture_->name() << "': ";
            os << DXGetErrorDescription(hr) << endl;
            throw domain_error(os.str());
        };
            
        texture_->size(Dimensions(info.Width, info.Height));
        texture_->handle(static_cast<void*>(handle_));
        texture_->loadStatus(Resource::statusLoaded);
        
        cout << "Loaded " << texture_->name() << endl;
        
    } else if (Resource::statusUnloading == texture_->loadStatus()) {
        cout << "Unloading" << endl;
    
        texture_->handle(0);
        RELEASE(handle_);
        texture_->loadStatus(Resource::statusUnloaded);
    }
}
