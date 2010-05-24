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

require 'ActorSupport'

class 'Dagger' (ActorSupport)
state 'Dagger.Alive'
state 'Dagger.Dead'

function Dagger:__init(color)
    self.node = engine.root:node()
    ActorSupport.__init(self, Dagger)
    
    color = color or "Red"

    self.mesh = self.node:fracture_object() {
        mesh = "Dagger.obj",
        material = "Dagger"..color..".mtl",
        fracture_count = 1
    }
    
    self.engine_node = self.node:node() {
        position = Vector(0, 0, -1.7)
    }
    
    self.flame = self.engine_node:particle_system() {
        type = ParticleSystem.ET_POINT,
        quota = 500,
        texture = "IncandescentBlue.png",
        particle_life = Range(.1, .1),
        particle_size = Range(2.2, 2.2),
        particle_growth_rate = Range(-24, -24),
        life = -1,
        width = Range(0, 0),
        height = Range(0, 0),
        depth = Range(0, 0),
        emission_speed = Range(-28, -28),
        emission_angle = Range(0, 0),
        emission_direction = Vector(0, 0, 1),
        emission_rate = Range(200, 200),
    }
    
    self.body = self.node:rigid_body()
    self.body.mass = 1
    self.armor = 20
    
    self.actor.state = "Alive"
end

function Dagger.Dead:on_state_enter()
    self.explosion = self.explosion or Explosion()
    self.explosion.node.position = self.node.position
    self.explosion.actor.state = "Alive"

    self.flame.life = 0
    self.mesh.fracture_count = 2
    
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1)
    self.mesh:fracture(Plane(n.unit, Vector(0, 0, .3)))
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1)
    self.mesh:fracture(Plane(n.unit, Vector(0, 0, -.3)))
end

function Dagger.Dead:on_fracture(node)
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1) * 100
    node:rigid_body():apply_force(n)
    
    node:particle_system() {
        type = ParticleSystem.ET_ELLIPSOID,
        quota = 300,
        inherit_velocity = false,
        particle_growth_rate = Range(-10, -10),
        texture = "BurstGold.png",
        particle_life = Range(.2, 1),
        particle_size = Range(.2, .7),
        life = math.random(10, 20),
        width = Range(0, .3),
        height = Range(0, .3),
        depth = Range(0, .3),
        emission_speed = Range(0, 3),
        emission_rate = Range(.01, 2000)
    }
end

function Dagger.Alive:on_collision(node, position)
    self.armor = self.armor - 20
    self.collision_position = position

    if (self.armor <= 0) then
        self.actor.state = "Dead"
    end
end
