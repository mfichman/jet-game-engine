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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <cstdint>
#include <string>
#include <map>

#ifdef WINDOWS
#define JETIMPORT							__declspec(dllimport)
#define JETEXPORT                			__declspec(dllexport)
#else
#define JETIMPORT __attribute__ ((visibility ("default")))
#define JETEXPORT __attribute__ ((visibility ("default")))
#endif

#ifdef JETDLL
#define JETAPI JETEXPORT
#pragma warning (disable:4251)
#else
#define JETAPI JETIMPORT
#endif

namespace Jet {
    class AudioSource;
    class AudioModule;
    class Color;
    class Controller;
    class GraphicsModule;
    class LogicModule;
    class Material;
    class MeshObject;
    class Mesh;
    class NetworkMonitor;
    class NetworkModule;
    class ParticleSystem;
    class PhysicsModule;
    class Quad;
    class Quaternion;
    class RigidBody;
    class SceneComponent;
    class SceneNode;
    class Vector;

    typedef float real_t;
    typedef std::pair<real_t, real_t> range_t;
    typedef std::map<std::string, std::string> table_t;
    
    enum CollisionMode { CM_BOX, CM_SPHERE, CM_MESH };
    enum EmitterType { ET_ELLIPSOID, ET_POINT, ET_BOX };
    enum CoordSystem { CT_WORLD, CT_LOCAL };
};

