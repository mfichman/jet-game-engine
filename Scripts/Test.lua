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

require 'Module'
require 'Starship'
require 'Monkey'
require 'Rock'
require 'Box'

class 'Test' (Module)

function Test:__init()
    Module.__init(self)
    
    print("Hello")
    
    -- Set up lights
    print("Creating lights")
    self.light_node = engine.root:node("light")
    self.light = self.light_node:light("light") {
        ambient_color = Color(.3, .3, .3, 1),
        diffuse_color = Color(1, 1, 1, 1),
        specular_color = Color(1, 1, 1, 1),
        direction = Vector(0, 0, 1)
    }
    
    -- Set up the camera
    print("Creating camera")
    self.camera_node = engine.root:node("camera")
    self.camera_node.position = Vector(20, 0, 20)
    self.camera_node:look(Vector(0, 0, 0), Vector(0, 1, 0))
    self.camera = self.camera_node:camera() {
        active = true,
        field_of_view = 45,
        far_clipping_distance = 1000,
        near_clipping_distance = 0.1
    }
    
    
    -- Set up the plane
    print("Creating plane")
    self.plane_node = engine.root:node("plane")
    self.plane = self.plane_node:mesh_object("plane") {
        mesh = "Plane.obj",
        material = "Metal.mtl"
    }
    self.plane_node:rigid_body()
    
    -- Set up scene objects and apply some forces
    print("Creating objects")
    self.s0 = Starship(engine.root, "s0")
    self.s1 = Rock(engine.root, "s1")
    
    --self.s1.node.position = Vector(0, 0, 0)
    --self.s0.node.position = Vector(0, -10, 0)
    ---self.s0.body:apply_force(Vector(-15000, 30000, 0))
    
end

function Test:on_destroy()
    print("Goodbye")
end

time = 0.
function Test:on_update()
    --self.plane_node.rotation = Quaternion(Vector(0, 1, 0), time)
    time = time + 0.01
end


function Test:on_key_pressed(key, x, y)
    
    if (key == 'q') then
        engine.running = false
    elseif (key == 's') then
        engine.simulation_speed = 1.0/4.0
    elseif (key == 'r') then
        self.s1.node.position = Vector(5, 0, 0)
        self.s1.body.linear_velocity = Vector(0, 0, 0)
        self.s0.node.position = Vector(0, 5, 0)
        self.s0.body.linear_velocity = Vector(0, 0, 0)
        self.s0.body:apply_force(Vector(1500, -3000, -1000))
    elseif (key == 'n') then
        engine:option("normal_mapping_enabled", not engine:option("normal_mapping_enabled"))
    elseif (key == 'p') then
        engine:option("specular_mapping_enabled", not engine:option("specular_mapping_enabled"))
    elseif (key == 'h') then
        engine:option("shadows_enabled", not engine:option("shadows_enabled"))
    elseif (key == 'm') then
    
        if (engine:option("fullscreen")) then
            engine:option("display_width", 1024)
            engine:option("display_height", 768)
            engine:option("fullscreen", false)
        else
            engine:option("display_width", 1680)
            engine:option("display_height", 1050)
            engine:option("fullscreen", true)
        end
        
        engine:option("video_mode_synced", false)
    elseif (key == 'x') then
        self.plane.material.shader = "Default"
    elseif (key == 'y') then
        self.plane.material.shader = "Default2"
    end
end
