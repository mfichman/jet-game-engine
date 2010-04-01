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

#include <Jet/Lua/ScriptLoader.hpp>
#include <Jet/Lua/Interpreter.hpp>

using namespace Jet::Lua;
using namespace Jet;
using namespace std;
  
ScriptLoader::ScriptLoader(Interpreter* interpreter) :
    interpreter_(interpreter),
    env_(interpreter->env()) {
        
}

void ScriptLoader::resource(const std::string& path) {
    // Check to see if the file has been loaded yet
    set<string>::iterator i = file_.find(path);
    if (i == file_.end()) {
        if (luaL_dofile(env_, path.c_str())) {
            string message(lua_tostring(env_, -1));
            throw runtime_error(message);
        }
        file_.insert(path);
    }
}