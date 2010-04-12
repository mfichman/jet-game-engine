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

#include <cstdint>
#include <utility>
#include <string>
#include <boost/intrusive_ptr.hpp>

#ifdef WINDOWS
#define JETIMPORT __declspec(dllimport)
#define JETEXPORT __declspec(dllexport)
#else
#define JETIMPORT __attribute__ ((visibility ("default")))
#define JETEXPORT __attribute__ ((visibility ("default")))
#endif

#ifdef JETDLL
#define JETAPI JETEXPORT
#else
#define JETAPI JETIMPORT
#endif

namespace Jet {
    class AudioSource;
    class Color;
    class Engine;
    class EngineListener;
    class Light;
    class Matrix;
    class Material;
    class Mesh;
    class MeshObject;
    class Node;
    class NodeListener;
    class Object;
    class ObjectFactory;
    class ParticleSystem;
    class QuadChain;
    class Quaternion;
    class Range;
    class ResourceLoader;
    class RigidBody;
    class RigidBodyListener;
    class Shader;
    class Texcoord;
    class Texture;
    class Quad;
    class QuadSet;
    class Vector;
    class Vertex;

    typedef boost::intrusive_ptr<AudioSource> AudioSourcePtr;
    typedef boost::intrusive_ptr<Engine> EnginePtr;
    typedef boost::intrusive_ptr<EngineListener> EngineListenerPtr;
    typedef boost::intrusive_ptr<Light> LightPtr;
    typedef boost::intrusive_ptr<Material> MaterialPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    typedef boost::intrusive_ptr<MeshObject> MeshObjectPtr;
    typedef boost::intrusive_ptr<Node> NodePtr;
    typedef boost::intrusive_ptr<NodeListener> NodeListenerPtr;
    typedef boost::intrusive_ptr<Object> ObjectPtr;
    typedef boost::intrusive_ptr<ObjectFactory> ObjectFactoryPtr;
    typedef boost::intrusive_ptr<ParticleSystem> ParticleSystemPtr;
    typedef boost::intrusive_ptr<QuadChain> QuadChainPtr;
    typedef boost::intrusive_ptr<ResourceLoader> ResourceLoaderPtr;
    typedef boost::intrusive_ptr<RigidBody> RigidBodyPtr;
    typedef boost::intrusive_ptr<RigidBodyListener> RigidBodyListenerPtr;
    typedef boost::intrusive_ptr<Shader> ShaderPtr;
    typedef boost::intrusive_ptr<Texture> TexturePtr;
    typedef boost::intrusive_ptr<QuadSet> QuadSetPtr;

    typedef float real_t;
    typedef void (*load_function_t)(Engine* engine);
    
    enum LightType { POINT_LIGHT, DIFFUSE_LIGHT };
    enum ParticleSystemType { BOX_EMITTER, ELLIPSOID_EMITTER };
    enum ModificationFlags {
        POSITION=1,
        ROTATION=2,
        LINEAR_VELOCITY=4,
        ANGULAR_VELOCITY=8,
        FORCE=16
    };
}
