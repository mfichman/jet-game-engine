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

namespace Jet { namespace Core {
    class ActionQueue;
    class AudioSource;
    class AudioSystem;
    class Camera;
    class Engine;
    class Font;
    class FractureObject;
    class FractalPlanet;
    class Geometry;
    class InputSystem;
    class Light;
    class Material;
    class Mesh;
    class MeshObject;
    class NetworkSystem;
    class Node;
    class Overlay;
    class ParticleBuffer;
    class ParticleSystem;
    class PhysicsSystem;
    class QuadChain;
    class QuadSet;
    class RenderSystem;
    class RenderTarget;
    class RigidBody;
    class ScriptActor;
    class ScriptSystem;
    class ScriptTask;
    class Shader;
    class Socket;
    class SocketWriter;
    class SocketReader;
    class Sound;
    class Texture;
    

    typedef boost::intrusive_ptr<ActionQueue> ActionQueuePtr;
    typedef boost::intrusive_ptr<AudioSource> AudioSourcePtr;
    typedef boost::intrusive_ptr<AudioSystem> AudioSystemPtr;
    typedef boost::intrusive_ptr<Camera> CameraPtr;
    typedef boost::intrusive_ptr<Engine> EnginePtr;
    typedef boost::intrusive_ptr<Font> FontPtr;
    typedef boost::intrusive_ptr<FractureObject> FractureObjectPtr;
    typedef boost::intrusive_ptr<FractalPlanet> FractalPlanetPtr;
    typedef boost::intrusive_ptr<Geometry> GeometryPtr;
    typedef boost::intrusive_ptr<InputSystem> InputSystemPtr;
    typedef boost::intrusive_ptr<Light> LightPtr;
    typedef boost::intrusive_ptr<Material> MaterialPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    typedef boost::intrusive_ptr<MeshObject> MeshObjectPtr;
    typedef boost::intrusive_ptr<NetworkSystem> NetworkSystemPtr;
    typedef boost::intrusive_ptr<Node> NodePtr;
    typedef boost::intrusive_ptr<Overlay> OverlayPtr;
    typedef boost::intrusive_ptr<ParticleSystem> ParticleSystemPtr;
    typedef boost::intrusive_ptr<ParticleBuffer> ParticleBufferPtr;
    typedef boost::intrusive_ptr<PhysicsSystem> PhysicsSystemPtr;
    typedef boost::intrusive_ptr<QuadChain> QuadChainPtr;
    typedef boost::intrusive_ptr<RenderSystem> RenderSystemPtr;
    typedef boost::intrusive_ptr<RenderTarget> RenderTargetPtr;
    typedef boost::intrusive_ptr<RigidBody> RigidBodyPtr;
    typedef boost::intrusive_ptr<ScriptSystem> ScriptSystemPtr;
    typedef boost::intrusive_ptr<ScriptActor> ScriptControllerPtr;
    typedef boost::intrusive_ptr<ScriptTask> ScriptTaskPtr;
    typedef boost::intrusive_ptr<Shader> ShaderPtr;
    typedef boost::intrusive_ptr<Socket> SocketPtr;
    typedef boost::intrusive_ptr<SocketReader> SocketReaderPtr;
    typedef boost::intrusive_ptr<SocketWriter> SocketWriterPtr;
    typedef boost::intrusive_ptr<Sound> SoundPtr;
    typedef boost::intrusive_ptr<Texture> TexturePtr;
    typedef boost::intrusive_ptr<QuadSet> QuadSetPtr;

    enum TextureSampler {
        DIFFUSE_MAP_SAMPLER = 0,
        SPECULAR_MAP_SAMPLER = 1,
        NORMAL_MAP_SAMPLER = 2,
        SHADOW_MAP_SAMPLER = 3
    };
    enum SocketType { SERVER, CLIENT, MULTICAST, DATAGRAM, STREAM };
}}
