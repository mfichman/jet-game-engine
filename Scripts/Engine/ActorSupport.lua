-- Copyright (c) 2010 Matt Fichman
--
-- Permission is hereby granted, free of charge, to any person obtaining a 
-- copy of this software and associated documentation files (the "Software"),
-- to deal in the Software without restriction, including without limitation 
-- the rights to use, copy, modify, merge, publish, distribute, sublicense, 
-- and/or sell copies of the Software, and to permit persons to whom the 
-- Software is furnished to do so, subject to the following conditions:
-- 
-- The above copyright notice and this permission notice shall be included in 
-- all copies or substantial portions of the Software.
-- 
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
-- FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
-- IN THE SOFTWARE.

class 'ActorSupport'

local registry = {}

function ActorSupport:__init(class)
    
    -- Initialize states for the actor
    for k,v in pairs(registry[class]) do
        self.actor = self.actor or self.node.actor
        
        local o = {}
        setmetatable(o, o)
        o.__index = function(t, k)
            return rawget(t, k) or self[k] or v[k]
        end
        
        self.actor:actor_state(k, o)
    end
end

function state(name)
    local i = name:gfind("%a+")
    
    local classname = i()
    local statename = i()
    
    local class = _G[classname]
    class[statename] = {}
    
    local state = class[statename]
    
    state.on_update = function() end
    state.on_render = function() end
    state.on_collision = function()  end
    state.on_destroy = function() end
    state.on_fracture = function()  end
    state.on_tick = function() end
    state.on_state_enter = function() end
    state.on_state_exit = function() end
    
    registry[class] = registry[class] or {}
    registry[class][statename] = state
end

