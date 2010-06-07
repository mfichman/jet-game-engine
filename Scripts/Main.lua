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

require 'StartScreen'
require 'Task'
require 'Stats'

-- Set up lights
print("Creating lights")
light_node = engine.root:node();
light = light_node:light("light") {
    ambient_color = Color(.6, .6, .6, 1),
    diffuse_color = Color(1, 1, 1, 1),
    specular_color = Color(1, 1, 1, 1),
    direction = Vector(1, 0, 1)
}

-- Set up the camera
print("Creating camera")
camera_node = engine.root:node("camera")
camera_node.position = Vector(35, 0, 35)
camera_node:look(Vector(-5, -5, 5), Vector(0, 1, 0))
--camera_node:look(Vector(0, 0, 0), Vector(0, 0, 1))
camera = camera_node:camera("camera") {
    active = true,
    field_of_view = 45,
    far_clipping_distance = 1000,
    near_clipping_distance = 0.1
}

-- Set up FPS counter
Stats(engine.screen, "stats")

local quad_set = engine.root:quad_set("test_quad")
quad_set:quad(0, Quad(Vector(0, 0, -1), Vector(0, 1, 0), 20, 20))
quad_set.texture = "Electricity.png"

StartScreen()