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

#include <Jet/Core/Types.hpp>
#include <Jet/Object.hpp>
#include <luabind/luabind.hpp>
#include <stdexcept>

namespace Jet { namespace Core {
    
//! This class represents a cooperative script task.  It is similar to a
//! thread.
//! @class ScriptTask
//! @brief Controls a script thread
class ScriptTask : public Object {
public:
    //! Creates a new script task.
    //! @param env the lua environment
    //! @param ref lua object representing the task
    inline ScriptTask(Engine* engine, int object_ref) :
        engine_(engine),
        alive_(true),
        resume_time_(0.0f) {
            
        thread_ = lua_newthread(engine_->script_system()->env());
        ref_ = lua_ref(engine_->script_system()->env(), LUA_REGISTRYINDEX);
        lua_getref(thread_, object_ref);
        lua_unref(thread_, object_ref);
        
        lua_getfield(thread_, -1, "on_run");
        lua_pushvalue(thread_, -2);
        lua_remove(thread_, -3);
    }
    
    //! Destroys the task
    inline ~ScriptTask() {
        lua_unref(thread_, ref_);
    }
    
    //! Returns the time that this task should resume
    inline float resume_time() const {
        return resume_time_;
    }
    
    //! Returns true if the task is alive
    inline bool alive() const {
        return alive_;
    }
    
    //! Resumes the task
    inline void resume() {
        int ret = lua_resume(thread_, 1);
        if (!ret) {
            alive_ = false;
        } else if (LUA_YIELD != ret) {
            alive_ = false;
            throw std::runtime_error(lua_tostring(thread_, -2));
        } else {
            // Calculate the time this thread should resume
            // Add a small bias (0.00001f) so that even a thread
            // that resumes immediately will yield to other threads
            // and wait until the next frame
            resume_time_ = (float)(engine_->frame_time() + lua_tonumber(thread_, -1));
            resume_time_ += 0.000001f;
        }
        lua_pop(thread_, lua_gettop(thread_));
    }
    
private:
    Engine* engine_;
    lua_State* thread_; 
    int ref_;
    bool alive_;
    float resume_time_;
};



}}