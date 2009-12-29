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

#include <Jet/Impl/D3D9/Types.hpp>
#include <Jet/Impl/D3D9/RootReactor.hpp>

namespace Jet { Impl { D3D9 {
using namespace std;
using namespace std::tr1;
using namespace boost;


class EffectPool {
public:
    EffectPool(RootReactor::Ptr e);
    inline FrameParams& frameParams() const { return frameParams_; }
    inline EffectPtr basicEffect() const { return basicEffect_; }
    inline EffectPtr postProcessingEffect() const { return postProcessEffect_; }

    inline D3DXHANDLE objectParamsHandle() const { return objectParamsHandle_; }
    inline D3DXHANDLE materialHandle() const { return materialHandle_; }
    inline D3DXHANDLE miscParamsHandle() const { return miscParamsHandle_; }
    inline D3DXHANDLE extraDataHandle() const { return extraDataHandle_; }

private:
    RootReactor::Ptr rootReactor_;
	FrameParameters	frameParams_;
    EffectPoolPtr effectPool_;
    EffectPtr basicEffect_;
    EffectPtr postProcessEffect_;
    D3DXHANDLE frameParamsHandle_;
    D3DXHANDLE objectParamsHandle_;
    D3DXHANDLE materialHandle_;
    D3DXHANDLE miscParamsHandle_;
    D3DXHANDLE extraDataHandle_;
};

}}};
