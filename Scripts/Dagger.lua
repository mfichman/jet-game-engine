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

require 'Actor'

class 'Dagger' (Actor)

function Dagger:__init(node, name)
    Actor.__init(self, node, name)

    self.mesh = self.node:fracture_object("mesh") {
        mesh = "Dagger.obj",
        material = "Dagger.mtl",
        fracture_count = 3
    }
    
    self.smoke = self.node:particle_system("smoke") {
        quota = 100,
        texture = "Fire.png",
        --shader=?
        life = Range(.5, .5),
        width = Range(0, 2),
        height = Range(0, 2),
        depth = Range(0, 2),
        emission_speed = Range(1, 10),
        emission_direction = Vector(0, 1, 0),
        emission_angle = Range(0, 180),
        emission_rate = 1000
    }
    
    self.body = self.node:rigid_body()
    self.body.mass = 10.0
end
