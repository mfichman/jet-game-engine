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

local function table_syntax(o)
    local mt = getmetatable(o)
    mt.__call = function(o, table)
        copy(table, o)
        return o
    end
    return o
end


local engine_material = engine.material;
function engine:material(name)
    return table_syntax(engine_material(self, name))
end

local node_mesh_object = engine.root.mesh_object;
function Node:mesh_object(name)
    return table_syntax(node_mesh_object(self, name))
end

local node_fracture_object = engine.root.fracture_object;
function Node:fracture_object(name)
    return table_syntax(node_fracture_object(self, name))
end

local node_particle_system = engine.root.particle_system;
function Node:particle_system(name)
    return table_syntax(node_particle_system(self, name))
end

local node_light = engine.root.light;
function Node:light(name)
    return table_syntax(node_light(self, name))
end

local node_camera = engine.root.camera;
function Node:camera(name)
    return table_syntax(node_camera(self, name))
end

