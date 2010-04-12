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

/*
#include <Jet/Lua/ScriptController.hpp>
#include <Jet/Node.hpp>
#include <cassert>
    
using namespace Jet::Lua;
using namespace Jet;
using namespace std;

    
ScriptController::ScriptController(lua_State* env, const std::string& type) :
    env_(env),
    type_(type),
    ref_(0) {
    
    
}

ScriptController::~ScriptController() {
    if (ref_) {
        lua_unref(env_, ref_);
    }
}

void ScriptController::init() {
    assert(!table_ && "Table should be null, but it is already initialized");
    luabind::object globals = luabind::globals(env_);
    luabind::object constructor = globals[type_];
    constructor();
    
    // Create a reference and store the object
    table_ = luabind::object(luabind::from_stack(env_, -1));
    
    lua_pushlightuserdata(env_, this);
    lua_pushcclosure(env_, &ScriptController::index, 1);
    lua_setfield(env_, -2, "__index");
    lua_pushlightuserdata(env_, this);
    lua_pushcclosure(env_, &ScriptController::newindex, 1);
    lua_setfield(env_, -2, "__newindex");
    
    setmetatable(table_, table_);
    ref_ = lua_ref(env_, LUA_GLOBALSINDEX);
}

void ScriptController::on_event(Node* node, const std::string& name, const Params& params) {
    table_.push(env_);
    string function = "on_" + name;
    lua_getfield(env_, -1, function.c_str());
    lua_remove(env_, -2);
    
    // Push the arguments to the function
    luabind::object object;
    for (Iterator<const Value> i = params; i; i++) {
        switch (i->type()) {
            case VT_STRING: object = luabind::object(env_, string(*i)); break;
            case VT_NUMBER: object = luabind::object(env_, number_t(*i)); break;
            case VT_COLOR: object = luabind::object(env_, Color(*i)); break;
            case VT_VECTOR: object = luabind::object(env_, Vector(*i)); break;
            case VT_QUATERNION: object = luabind::object(env_, Quaternion(*i)); break;
            case VT_RANGE: object = luabind::object(env_, Range(*i)); break;
        }
        object.push(env_);
    }
    
    // Call the function and check for errors
    if (lua_pcall(env_, params.size(), 0, 0)) {
        string message(lua_tostring(env_, -1));
        throw runtime_error(message);
    }
}

void ScriptController::on_update(Node* node) {
    table_["on_update"]();
}

void ScriptController::on_render(Node* node) {
    table_["on_render"]();
}

void ScriptController::on_destroy(Node* node) {
    table_["on_destroy"]();
}

ScriptController* ScriptController::clone() const {
    return new ScriptController(env_, type_);
}

int ScriptController::index(lua_State* env) {
    try {
        ScriptController* self = static_cast<ScriptController*>(lua_touserdata(env, lua_upvalueindex(1)));
        string key(lua_tostring(env, 2));
        
        Object* component = self->node_->component(key);
        if (component) {
            // Create a new table that works like a delegate for the
            // component
            lua_newtable(env);
            lua_pushlightuserdata(env, component);
            lua_pushcclosure(env, &ScriptController::component_index, 1);
            lua_setfield(env, -2, "__index");
            lua_pushlightuserdata(env, component);
            lua_pushcclosure(env, &ScriptController::component_newindex, 1);
            lua_setfield(env, -2, "__newindex");
            lua_pushvalue(env, -1);
            lua_setmetatable(env, -1);
            
            // Cache the delegate for future calls.
            lua_pushvalue(env, -1);
            lua_setfield(env, 1, key.c_str());
            return 1;
        } else {
            // Otherwise, return nil
            return 0;
        }
        
    } catch (std::exception& ex) {
        lua_pushstring(env, ex.what());
        lua_error(env);
    }
    
    return 0;
}

int ScriptController::newindex(lua_State* env) {
    return 0;
}


int ScriptController::component_index(lua_State* env) {
    try {
        Object* self = static_cast<Object*>(lua_touserdata(env, lua_upvalueindex(1)));
        string key(lua_tostring(env, 2));
        luabind::object value(luabind::from_stack(env, -1));
        
        // Push the value from the component onto the stack, and return it
        if (luabind::type(value) == LUA_TSTRING) {
            self->value(key, luabind::object_cast<string>(value));
        } else if (luabind::type(value) == LUA_TNUMBER) {
            self->value(key, luabind::object_cast<number_t>(value));
        } else if (luabind::type(value) == LUA_TTABLE) {
            ValueType type = luabind::object_cast<ValueType>(value["__type"]);
            switch (type) {
                case VT_COLOR: self->value(key, luabind::object_cast<Color>(value)); break;
                case VT_VECTOR: self->value(key, luabind::object_cast<Vector>(value)); break;
                case VT_QUATERNION: self->value(key, luabind::object_cast<Quaternion>(value)); break;
                case VT_RANGE: self->value(key, luabind::object_cast<Range>(value)); break;
            }
        } else if (luabind::type(value) == LUA_TNIL) {
            self->value(key, Value());
        }
        
    } catch (std::exception& ex) {
        lua_pushstring(env, ex.what());
        lua_error(env);
    }
    
    return 0;
}

int ScriptController::component_newindex(lua_State* env) {
    try {
        Object* self = static_cast<Object*>(lua_touserdata(env, lua_upvalueindex(1)));
        string key(lua_tostring(env, 2));
        
        // Push the value from the component onto the stack, and return it
        const Value& value = self->value(key);
        luabind::object object;
        switch (value.type()) {
            case VT_STRING: object = luabind::object(env, string(value)); break;
            case VT_NUMBER: object = luabind::object(env, number_t(value)); break;
            case VT_COLOR: object = luabind::object(env, Color(value)); break;
            case VT_VECTOR: object = luabind::object(env, Vector(value)); break;
            case VT_QUATERNION: object = luabind::object(env, Quaternion(value)); break;
            case VT_RANGE: object = luabind::object(env, Range(value)); break;
        }
        object.push(env);
        return 1;
        
    } catch (std::exception& ex) {
        lua_pushstring(env, ex.what());
        lua_error(env);
    }
    
    return 0;
}*/