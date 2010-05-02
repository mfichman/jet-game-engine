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
        fracture_count = 2
    }
    
    self.spark_template = {
        type = ParticleSystem.BOX_EMITTER,
        quota = 100,
        texture = "Electricity.png",
        particle_life = Range(.3, .3),
        particle_size = Range(1, 3),
        life = -1,
        width = Range(0, .3),
        height = Range(0, 1.1),
        depth = Range(0, .2),
        emission_speed = Range(0, 0),
        emission_direction = Vector(0, 1, 0),
        emission_angle = Range(0, 0),
        emission_rate = Range(0.7, 60)
    }

    self.body = self.node:rigid_body()
    self.body.mass = 10.0
end

function Dagger:on_fracture(node)
    self.node:particle_system("sparks")(self.spark_template)
    self.node:particle_system("sparks").life = math.random() * 3 + 2
    
    node:particle_system("sparks")(self.spark_template)
    node:particle_system("sparks").life = math.random() * 3 + 2
end
