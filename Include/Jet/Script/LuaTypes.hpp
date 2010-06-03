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

#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Quaternion.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/any.hpp>
#include <luabind/luabind.hpp>
#include <lua/lua.hpp>

namespace Jet { 
    class LuaActorState;
    class LuaScriptSystem;
    class LuaTask;
    class LuaModule;

    typedef boost::intrusive_ptr<LuaScriptSystem> LuaScriptSystemPtr;
    typedef boost::intrusive_ptr<LuaActorState> LuaActorStatePtr;
    typedef boost::intrusive_ptr<LuaTask> LuaTaskPtr;
    typedef boost::intrusive_ptr<LuaModule> LuaModulePtr;
}


namespace luabind {
    
    template <>
    struct default_converter<boost::any> : native_converter_base<boost::any> {
        static int compute_score(lua_State* env, int index) {
            return 0;
        }
        
        boost::any from(lua_State* env, int index) {
			boost::optional<Jet::Vector> o1;
			boost::optional<Jet::Quaternion> o2;
            switch (lua_type(env, index)) {
                case LUA_TNIL: return boost::any();
				case LUA_TSTRING: return boost::any(std::string(lua_tostring(env, index)));
                case LUA_TNUMBER: return boost::any((float)lua_tonumber(env, index));
#ifdef WINDOWS
#pragma warning(disable:4800)
#endif
                case LUA_TBOOLEAN: return boost::any((bool)lua_toboolean(env, index));
#ifdef WINDOWS
#pragma warning(default:4800)
#endif
                case LUA_TUSERDATA:
					o1 = object_cast_nothrow<Jet::Vector>(object(from_stack(env, index)));
                    if (o1) {
                        return o1.get();
                    }
					o2 = object_cast_nothrow<Jet::Quaternion>(object(from_stack(env, index)));
                    if (o2) {
                        return o2.get();
                    }

                default: return boost::any();
            }          
        }
        
        void to(lua_State* env, boost::any const& any) {
            if (typeid(float) == any.type()) {
                lua_pushnumber(env, boost::any_cast<float>(any));
			} else if (typeid(std::string) == any.type()) {
				lua_pushstring(env, boost::any_cast<std::string>(any).c_str());
            } else if (typeid(bool) == any.type()) {
                lua_pushboolean(env, boost::any_cast<bool>(any));
			} else if (typeid(Jet::Vector) == any.type()) {
				object o(env, boost::any_cast<Jet::Vector>(any));
                o.push(env);
			} else if (typeid(Jet::Quaternion) == any.type()) {
				object o(env, boost::any_cast<Jet::Quaternion>(any));
                o.push(env);
            } else {
                lua_pushnil(env);
            }
        }
    };
    
    template <>
    struct default_converter<boost::any const&> : default_converter<boost::any> {
        
    };
}