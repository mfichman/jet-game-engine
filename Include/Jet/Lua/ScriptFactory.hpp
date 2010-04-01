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

#include <Jet/Lua/Types.hpp>
#include <Jet/Factory.hpp>
#include <lua/lua.hpp>

namespace Jet { namespace Lua {

//! This class loads controllers from Lua script files.
//! @class ScriptFactory
//! @brief Loads controllers from Lua script files.
class ScriptFactory : public Factory {
public:
    //! Creates a new loader.
    //! @param interpreter the parent interpreter
    ScriptFactory(Interpreter* interpreter);

    //! Destructor.
    virtual ~ScriptFactory() {}

    //! Creates a new controller representing a Lua script.
    //! @param type the type of controller to create
    virtual Object* object(const std::string& type);
    
    //! Returns the Lua environment.
    lua_State* interpreter() const {
        return env_;
    }

private:
    InterpreterPtr interpreter_;
    lua_State* env_;
};

}}
