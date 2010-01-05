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
    handle_(0) {

    texture_->publisher().observerAdd(this); 
}

//------------------------------------------------------------------------------
TextureReactor::~TextureReactor() {
    texture_->publisher().observerDel(this);
    texture_->loadStatus(Texture::statusUnloading);
}

//------------------------------------------------------------------------------
void 
TextureReactor::onLoadStatus() {
    if (Resource::statusLoading == texture_->loadStatus() && !handle_) {           
        cout << "Loading " << texture_->name() << endl;            
            
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXCreateTextureFromFileExA(
            rootReactor_->device(), texture_->name().c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, &info, NULL, &handle_); 
            
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
