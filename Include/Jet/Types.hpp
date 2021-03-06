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

#if defined(_WIN32) && !defined(WINDOWS)
#define WINDOWS
#endif

#ifdef WINDOWS
#include <cstdint>
#include <cstdlib>
#include <crtdbg.h>
#else
#include <stdint.h>
#endif

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
    class Actor;
    class ActorState;
	class Address;
    class Audio;
    class AudioSource;
    class Box;
    class Camera;
    class Color;
    class CollisionSphere;
    class Cubemap;
    class Engine;
    class EngineListener;
    class Font;
    class FractureObject;
    class FractalPlanet;
    class Frustum;
    class NetworkMatch;
    class Geometry;
    class Input;
	class InputState;
    class Light;
    class Matrix;
    class Material;
    class Mesh;
    class MeshObject;
    class Module;
    class Network;
    class NetworkMonitor;
    class Node;
    class Object;
    class Overlay;
    class OverlayListener;
    class Particle;
    class ParticleSystem;
    class Player;
    class Plane;
    class Point;
    class Physics;
    class Quad;
    class QuadChain;
    class QuadSet;
    class Quaternion;
    class Range;
    class Graphics;
    class RigidBody;
    class Script;
    class Signal;
    class Shader;
    class Sound;
    class Texcoord;
    class Texture;
    class Vector;
    class Vertex;

    typedef boost::intrusive_ptr<Actor> ActorPtr;
    typedef boost::intrusive_ptr<ActorState> ActorStatePtr;
    typedef boost::intrusive_ptr<Audio> AudioPtr;
    typedef boost::intrusive_ptr<AudioSource> AudioSourcePtr;
    typedef boost::intrusive_ptr<Camera> CameraPtr;
    typedef boost::intrusive_ptr<CollisionSphere> CollisionSpherePtr;
    typedef boost::intrusive_ptr<Cubemap> CubemapPtr;
    typedef boost::intrusive_ptr<Engine> EnginePtr;
    typedef boost::intrusive_ptr<EngineListener> EngineListenerPtr;
    typedef boost::intrusive_ptr<Font> FontPtr;
    typedef boost::intrusive_ptr<FractureObject> FractureObjectPtr;
    typedef boost::intrusive_ptr<FractalPlanet> FractalPlanetPtr;
    typedef boost::intrusive_ptr<Geometry> GeometryPtr;
    typedef boost::intrusive_ptr<Input> InputPtr;
    typedef boost::intrusive_ptr<Light> LightPtr;
    typedef boost::intrusive_ptr<Material> MaterialPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    typedef boost::intrusive_ptr<MeshObject> MeshObjectPtr;
    typedef boost::intrusive_ptr<Module> ModulePtr;
    typedef boost::intrusive_ptr<Network> NetworkPtr;
    typedef boost::intrusive_ptr<NetworkMonitor> NetworkMonitorPtr;
    typedef boost::intrusive_ptr<Node> NodePtr;
    typedef boost::intrusive_ptr<Object> ObjectPtr;
    typedef boost::intrusive_ptr<Overlay> OverlayPtr;
    typedef boost::intrusive_ptr<OverlayListener> OverlayListenerPtr;
    typedef boost::intrusive_ptr<ParticleSystem> ParticleSystemPtr;
    typedef boost::intrusive_ptr<Physics> PhysicsPtr;
    typedef boost::intrusive_ptr<QuadChain> QuadChainPtr;
    typedef boost::intrusive_ptr<RigidBody> RigidBodyPtr;
    typedef boost::intrusive_ptr<Graphics> GraphicsPtr;
    typedef boost::intrusive_ptr<Script> ScriptPtr;
    typedef boost::intrusive_ptr<Shader> ShaderPtr;
    typedef boost::intrusive_ptr<Sound> SoundPtr;
    typedef boost::intrusive_ptr<Texture> TexturePtr;
    typedef boost::intrusive_ptr<QuadSet> QuadSetPtr;
 
    enum LightType { LT_POINT, LT_DIRECTIONAL };
    enum EmitterType { ET_BOX, ET_ELLIPSOID, ET_POINT };
    enum PlaybackState { PS_PLAY, PS_STOP };
    enum ResourceState { RS_UNLOADED, RS_CACHED, RS_LOADED };
    enum SyncMode { SM_STATIC, SM_DYNAMIC };
    enum LayoutMode { LM_RELATIVE, LM_ABSOLUTE };
    enum Alignment { AL_TOP, AL_CENTER, AL_BOTTOM, AL_LEFT, AL_RIGHT };
    enum NetworkState { NS_DISCOVER, NS_HOST, NS_CLIENT, NS_RUNNING, NS_DISABLED };
}
