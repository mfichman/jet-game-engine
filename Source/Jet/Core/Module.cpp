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

#include <Jet/Core/MaterialLoader.hpp>
#include <Jet/Core/MeshLoader.hpp>
#include <Jet/Core/TextureLoader.hpp>
#include <Jet/Types.hpp>
#include <Jet/Engine.hpp>

using namespace Jet;
using namespace Jet::Core;

extern "C" {
    JETAPI void load(Engine* engine);
}

void load(Engine* engine) {
    ResourceLoaderPtr mtl_loader(new MaterialLoader(engine));
    ResourceLoaderPtr obj_loader(new MeshLoader(engine));
    ResourceLoaderPtr texture_loader(new TextureLoader(engine));

    engine->resource_loader(".mtl", mtl_loader.get());
    engine->resource_loader(".MTL", mtl_loader.get());
    engine->resource_loader(".obj", obj_loader.get());
    engine->resource_loader(".OBJ", obj_loader.get());
    engine->resource_loader(".png", texture_loader.get());
    engine->resource_loader(".tga", texture_loader.get());
    engine->resource_loader(".jpg", texture_loader.get());
    engine->resource_loader(".bmp", texture_loader.get());
}
