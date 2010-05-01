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

class 'Explosion' (Actor)

function Explosion:__init(node, name)
    Actor.__init(self, node, name)
    
    self.explosion = self.node:particle_system("explosion") {
        type = ParticleSystem.ELLIPSOID_EMITTER,
        quota = 100,
        texture = "BurstGold.png",
        particle_life = Range(.35, .35),
        particle_size = Range(9, 9),
        life = .05,
        width = Range(0, 5),
        height = Range(0, 5),
        depth = Range(0, 5),
        emission_speed = Range(.8, .8),
        emission_direction = Vector(0, 1, 0),
        emission_angle = Range(0, 0),
        emission_rate = Range(300, 300)
    }
    
    self.sparks = self.node:particle_system("sparks") {
        type = ParticleSystem.ELLIPSOID_EMITTER,
        quota = 8,
        texture = "BurstGold.png",
        particle_life = Range(0.5, 0.5),
        particle_size = Range(0.8, 0.8),
        life = .01,
        width = Range(0, 0),
        height = Range(0, 0),
        depth = Range(0, 0),
        emission_speed = Range(30, 40),
        emission_direction = Vector(0, 1, 0),
        emission_angle = Range(0, 0),
        emission_rate = Range(1000, 1000)
    }
end

function Explosion:reset()
    self.sparks.life = .01
    self.explosion.life = .05
end
