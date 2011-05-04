/*
 * Copyright (c) 2010 Matt Fichman
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOVT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BVT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AVTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OVT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <boost/intrusive_ptr.hpp>

#define MAX_SHADOW_CASCADES 4U

namespace Jet {
    class CoreCamera;
    class CoreCollisionSphere;
    class CoreEngine;
    class CoreFractureObject;
    class CoreLight;
    class CoreMeshObject;
    class CoreNode;
    class CoreOverlay;
    class CoreParticleSystem;
    class CoreQuadChain;
    class CoreQuadSet;    

    typedef boost::intrusive_ptr<CoreCamera> CoreCameraPtr;
    typedef boost::intrusive_ptr<CoreCollisionSphere> CoreCollisionSpherePtr;
    typedef boost::intrusive_ptr<CoreEngine> CoreEnginePtr;
    typedef boost::intrusive_ptr<CoreFractureObject> CoreFractureObjectPtr;
    typedef boost::intrusive_ptr<CoreLight> CoreLightPtr;
    typedef boost::intrusive_ptr<CoreMeshObject> CoreMeshObjectPtr;
    typedef boost::intrusive_ptr<CoreNode> CoreNodePtr;
    typedef boost::intrusive_ptr<CoreOverlay> CoreOverlayPtr;
    typedef boost::intrusive_ptr<CoreParticleSystem> CoreParticleSystemPtr;
    typedef boost::intrusive_ptr<CoreQuadChain> CoreQuadChainPtr;
    typedef boost::intrusive_ptr<CoreQuadSet> CoreQuadSetPtr;
}
