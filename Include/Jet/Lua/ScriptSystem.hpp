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
#include <Jet/Core/Engine.hpp>
#include <Jet/Script.hpp>
#include <queue>
#include <vector>

namespace Jet { namespace Lua {

//! This class loads the Lua interpreter and initializes Luabind.
//! @class Interpreter
//! @brief Initializes the Lua interpreter.
class ScriptSystem : public Script, public EngineListener {
public:
    //! Creates a new loader.
    ScriptSystem(Core::Engine* engine);

    //! Destructor
    virtual ~ScriptSystem();
    
    //! Returns the Lua environment.
    inline lua_State* env() const {
        return env_;
    }
    
    //! Returns memory used by Lua.
    inline size_t memory_usage() const {
        return lua_gc(env_, LUA_GCCOUNT, 0);
    }

private:
    void on_tick() {}
    void on_init();
    void on_update();
    void on_render() {}
    
    void init_value_type_bindings();
    void init_entity_type_bindings();
    
    static int adopt_actor(lua_State* env);
    static int adopt_widget(lua_State* env);
    static int adopt_module(lua_State* env);
    static int adopt_task(lua_State* env);
    static int on_error(lua_State* env);
    
    class Compare {
    public:
        bool operator()(TaskPtr t0, TaskPtr t1);
    };
    
    Core::Engine* engine_;
    std::priority_queue<TaskPtr, std::vector<TaskPtr>, Compare> task_;
    lua_State* env_;
};


}}
