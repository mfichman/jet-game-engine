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

class 'Bullet' (Actor)

function Bullet:__init(node, name)
    Actor.__init(self, node, name)

    self.flame = self.node:particle_system() {
        type = ParticleSystem.ET_POINT,
        quota = 300,
        texture = "IncandescentGold.png",
        particle_life = Range(.1, .1),
        particle_size = Range(.45, .45),
        particle_growth_rate = Range(0, 0),
        life = -1,
        width = Range(0, 0),
        height = Range(0, 0),
        depth = Range(0, 0),
        emission_speed = Range(0, 0),
        emission_angle = Range(0, 0),
        emission_direction = Vector(0, 0, 1),
        emission_rate = Range(800, 800),
    }
    
    self.shape = self.node:collision_sphere()
    self.shape.radius = 1

    self.body = self.node:rigid_body()
    self.body.mass = .1
end

