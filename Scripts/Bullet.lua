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

class 'Bullet' (ActorSupport)
state 'Bullet.Alive'
state 'Bullet.Dead'

function Bullet:__init()
    self.node = engine.root:node()
    ActorSupport.__init(self, Bullet)
    
    self.flame = self.node:particle_system() {
        type = ParticleSystem.ET_POINT,
        quota = 300,
        texture = "IncandescentBlue.png",
        particle_life = Range(1, 1),
        particle_size = Range(1.45, 1.45),
        particle_growth_rate = Range(-8, -8),
        life = -1,
        width = Range(0, 0),
        height = Range(0, 0),
        depth = Range(0, 0),
        emission_speed = Range(0, 0),
        emission_angle = Range(0, 0),
        emission_direction = Vector(0, 0, 1),
        emission_rate = Range(10, 10),
    }
    
    self.shape = self.node:collision_sphere()
    self.shape.radius = 0.4

    self.body = self.node:rigid_body()
    self.body.mass = 1

    
    self.actor.state = "Alive"
end

function Bullet.Alive:on_state_enter()
    self.node.visible = true
end

function Bullet.Alive:on_tick()
    local forward = self.body.linear_velocity.unit
    local up = forward.orthogonal
    local right = forward:cross(up)
    self.node.rotation = Quaternion(up, right, forward)
    self.body.angular_velocity = Vector()
end

function Bullet.Alive:on_collision(node, position)
    self.explosion = self.explosion or Explosion()
    self.explosion.actor.state = "Alive"
    self.explosion.node.position = position
    self.actor.state = "Dead"
end

function Bullet.Dead:on_state_enter()
    self.node.visible = false
end
