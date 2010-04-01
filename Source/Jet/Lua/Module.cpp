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
 
#include <Jet/Lua/Interpreter.hpp>
#include <Jet/Lua/ScriptFactory.hpp>
#include <Jet/Lua/ScriptLoader.hpp>
#include <Jet/Types.hpp>
#include <Jet/Engine.hpp>

using namespace Jet;
using namespace Jet::Lua;
using namespace boost;

extern "C" {
    JETAPI void load(Engine* engine);
}

void load(Engine* engine) {
    InterpreterPtr interpreter(new Interpreter());
    engine->module(interpreter.get());
    
    FactoryPtr script_factory(new ScriptFactory(interpreter.get()));
    engine->factory(".lua", script_factory.get());
    
    LoaderPtr script_loader(new ScriptLoader(interpreter.get()));
    engine->loader(".lua", script_loader.get());
    engine->loader(".luac", script_loader.get());
}
