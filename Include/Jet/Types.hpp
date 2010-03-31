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
    class Buffer;
    class Color;
    class Component;
    class Controller;
    class Engine;
    class Factory;
    class Loader;
    class Handler;
    class Material;
    class Mesh;
    class Node;
    class Object;
    class Params;
    class Quaternion;
    class Range;
    class Texcoord;
    class Texture;
    class Value;
    class Vector;
    class Vertex;

    typedef boost::intrusive_ptr<Buffer> BufferPtr;
    typedef boost::intrusive_ptr<Component> ComponentPtr;
    typedef boost::intrusive_ptr<Controller> ControllerPtr;
    typedef boost::intrusive_ptr<Engine> EnginePtr;
    typedef boost::intrusive_ptr<Factory> FactoryPtr;
    typedef boost::intrusive_ptr<Handler> HandlerPtr;
    typedef boost::intrusive_ptr<Loader> LoaderPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    typedef boost::intrusive_ptr<Node> NodePtr;
    typedef boost::intrusive_ptr<Object> ObjectPtr;
    typedef boost::intrusive_ptr<Texture> TexturePtr;

    typedef float real_t;
    typedef double number_t;
    typedef void (*load_function_t)(Engine* engine);
    enum ValueType { VT_NIL, VT_STRING, VT_COLOR, VT_NUMBER, VT_QUATERNION, VT_VECTOR, VT_RANGE };
}
