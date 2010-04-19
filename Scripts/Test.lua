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

class 'Test' (Module)

function Test:__init()
    Module.__init(self)
    
    -- Set up lights
    self.light_node = engine.root:node("light")
    self.light = self.light_node:light("light")
    self.light.ambient_color = Color(.3, .3, .3, 1)
    self.light.diffuse_color = Color(1, 1, 1, 1)
    self.light.specular_color = Color(1, 1, 1, 1)
    self.light.direction = Vector(1, 0, 0)
    
    -- Set up the camera
    self.camera_node = engine.root:node("camera")
    self.camera_node.position = Vector(15, 0, 15)
    self.camera_node:look(Vector(0, 0, 0), Vector(0, 1, 0))
    self.camera = self.camera_node:camera()
    self.camera.active = true
    
    -- Set up the plane
    self.plane_node = engine.root:node("plane")
    self.plane = self.plane_node:mesh_object("plane")
    self.plane.mesh = "plane"
    self.plane.material = "plane"
    self.plane_node:rigid_body()
    
    -- Set up scene objects and apply some forces
    local s0 = Starship(engine.root, "s0")
    local s1 = Monkey(engine.root, "s1")
    
    s1.node.position = Vector(0, 0, 0)
    s0.node.position = Vector(0, -10, 0)
    s0.body:apply_force(Vector(-15000, 15000, 0))
end


function Test:on_key_pressed(key, x, y)
    print(key, x, y)
end
