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

function math.clamp(val, min, max)
    if (val < min) then return min end
    if (val > max) then return max end
    return val;
end

function option(key, value)
    engine:option(key, value)
end

function search_folder(folder)
    engine:search_folder(folder)
end

function copy(src, dest)
    for k,v in pairs(src) do
        dest[k] = v
    end
end

function callable(o)
    local mt = getmetatable(o)
    mt.__call = function(o, table)
        copy(table, o)
        return o
    end
    return o
end

function bind(method, self)
    return function(...)
        self[method](self, ...)
    end
end

local luabind_class = class
function class(name)
    local i = name:gfind("%a+")
    local table = _G
    local class = nil
    local token = nil
    while true do
        token = i()
        
        if (not token) then
            break
        end
        if (class) then
            table = table[class]
        end
        class = token
    end
    
    local ret = luabind_class(name)
    table[class] = _G[name]
    return ret
end

function math.round(num, idp)
    local mult = 10^(idp or 0)
    return math.floor(num * mult + 0.5) / mult
end

local engine_material = engine.material;
function engine:material(name)
    name = name or ""
    return callable(engine_material(self, name))
end

local overlay_overlay = engine.screen.overlay;
function Overlay:overlay(name)
    name = name or ""
    return callable(overlay_overlay(self, name))
end

local node_node = engine.root.node;
function Node:node(name)
    name = name or ""
    return callable(node_node(self, name))
end

local node_collision_sphere = engine.root.collision_sphere;
function Node:collision_sphere(name)
    name = name or ""
    return callable(node_collision_sphere(self, name))
end

local node_mesh_object = engine.root.mesh_object;
function Node:mesh_object(name)
    name = name or ""
    return callable(node_mesh_object(self, name))
end

local node_fracture_object = engine.root.fracture_object;
function Node:fracture_object(name)
    name = name or ""
    return callable(node_fracture_object(self, name))
end

local node_particle_system = engine.root.particle_system;
function Node:particle_system(name)
    name = name or ""
    return callable(node_particle_system(self, name))
end

local node_light = engine.root.light;
function Node:light(name)
    name = name or ""
    return callable(node_light(self, name))
end

local node_camera = engine.root.camera;
function Node:camera(name)
    name = name or ""
    return callable(node_camera(self, name))
end

