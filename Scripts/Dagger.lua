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
    
    self.spark_template = {
        type = ParticleSystem.BOX_EMITTER,
        quota = 100,
        texture = "Electricity.png",
        particle_life = Range(.1, .1),
        particle_size = Range(3, 4),
        life = -1,
        width = Range(0, .35),
        height = Range(0, .35),
        depth = Range(0, .35),
        emission_speed = Range(0, 0),
        emission_direction = Vector(0, 1, 0),
        emission_angle = Range(0, 0),
        emission_rate = Range(1, 70)
    }
    
    self.body = self.node:rigid_body()
    self.body.mass = 10.0
end

function Dagger:on_fracture(node)
    self.node:particle_system("sparks")(self.spark_template)
    self.node:particle_system("sparks").life = math.random() * 3 + 2
    
    local bounding_box = node:fracture_object("fracture").bounding_box
    local particle_system = node:particle_system("sparks")(self.spark_template)

    particle_system {
        life = math.random() * 3 + 2,
        width = Range(0, bounding_box.width/4),
        height = Range(0, bounding_box.height/4),
        depth = Range(0, bounding_box.depth/4)
    }
    
    

end
