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

require 'ActorState'

class 'BulletActive' (ActorState)
class 'BulletInactive' (ActorState)

-- This function creates a blue energy bullet node that creates an
-- explosion when it hits something
function Bullet()
	-- Create a new node
    local node = engine.root:node()
    
    -- Create a blue particle effect for the bullet
    local flame = node:particle_system()
    flame.type = ParticleSystem.ET_POINT
    flame.quota = 300
    flame.texture = "IncandescentBlue.png"
    flame.particle_life = Range(1, 1)
    flame.particle_size = Range(1.45, 1.45)
    flame.particle_growth_rate = Range(-8, -8)
    flame.life = -1
    flame.width = Range(0, 0)
    flame.height = Range(0, 0)
    flame.depth = Range(0, 0)
    flame.emission_speed = Range(0, 0)
    flame.emission_angle = Range(0, 0)
    flame.emission_direction = Vector(0, 0, 1)
    flame.emission_rate = Range(10, 10)
        
    -- Create a sphere for detecting collisions with the
    -- node/particle system
    local sphere = node:collision_sphere()
    sphere.radius = 0.4
    
    -- Create an actor to handle state transitions for the node
    node.actor:actor_state("Active", BulletActive(node))
    node.actor:actor_state("Inactive", BulletInactive(node))
    node.actor.state = "Inactive"
    
    -- Give the sphere mass and a rigid body
    node.rigid_body.mass = 1
    
    return node
end

function BulletActive:__init(node)
	self.node = node
end

function BulletActive:on_state_enter()
    self.node.visible = true
end

function BulletActive:on_tick()
	-- Create an orthogonal basis using the velocity vector 
	-- as the forward vector
    local forward = self.node.rigid_body.linear_velocity.unit
    local up = forward.orthogonal
    local right = forward:cross(up)
    
    -- Set the rotation and the angular velocity
    self.node.rotation = Quaternion(up, right, forward)
    --self.node.rigid_body.angular_velocity = Vector()
end

function BulletActive:on_collision(node, position)
    -- Create an explosion object if one doesn't exist, and
    -- set its position to the collision contact point
    self.explosion = self.explosion or Explosion()
    self.explosion.node.position = position
    self.explosion.actor.state = "Active"
    
    -- Switch states to inactive
    self.node.actor.state = "Inactive"
end

function BulletInactive:__init(node)
	self.node = node
end

function BulletInactive:on_state_enter()
    self.node.visible = false
end
