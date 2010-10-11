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

class 'DaggerActive' (ActorState)
class 'DaggerInactive' (ActorState)

function Dagger(color)
	-- Create a new node
	local node = engine.root:node()
   
    -- Create a new fracture mesh for the ship
    local mesh = node:fracture_object()
    mesh.mesh = "Dagger.obj"
    mesh.material = "Dagger"..(color or "Red")..".mtl"
    mesh.fracture_count = 1
    
    -- Node for the engine flame (slight offset from center along z)
    local engine = node:node()
    engine.position = Vector(0, 0, -1.7)
    
    local flame = engine:particle_system("flame")
    flame.type = ParticleSystem.ET_POINT
    flame.quota = 500
    flame.texture = "IncandescentBlue.png"
    flame.particle_life = Range(.1, .1)
    flame.particle_size = Range(2.2, 2.2)
    flame.particle_growth_rate = Range(-24, -24)
    flame.life = -1
    flame.width = Range(0, 0)
    flame.height = Range(0, 0)
    flame.depth = Range(0, 0)
    flame.emission_speed = Range(-28, -28)
    flame.emission_angle = Range(0, 0)
    flame.emission_direction = Vector(0, 0, 1)
    flame.emission_rate = Range(200, 200)
    
    -- Create an actor to handle state transitions for the node
    node.actor:actor_state("Active", DaggerActive(node))
    node.actor:actor_state("Inactive", DaggerInactive(node))
    node.actor.state = "Active"
    
    -- Initialize the rigid body
    node.rigid_body.mass = 1
    
    node.flame = flame
    node.mesh = mesh
    
    return node
end

function DaggerInactive:__init(node)
	self.node = node
end

function DaggerInactive:on_state_enter()
	-- Create an explosion object at the current position
    self.explosion = self.explosion or Explosion()
    self.explosion.position = self.node.position
    self.explosion.actor.state = "Active"

    self.node.flame.life = 0
    self.node.mesh.fracture_count = 2
    
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1)
    self.node.mesh:fracture(Plane(n.unit, Vector(0, 0, .3)))
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1)
    self.node.mesh:fracture(Plane(n.unit, Vector(0, 0, -.3)))
end

function DaggerInactive:on_fracture(node)
	-- Apply a random outward force on this piece of the fracture
    local n = Vector(math.random()*2-1, math.random()*2-1, math.random()*2-1) * 100
    node.rigid_body:apply_force(n)
    
    -- Create a new particle system on the node that is fracturing away
    node:particle_system() {
        type = ParticleSystem.ET_ELLIPSOID,
        quota = 300,
        inherit_velocity = false,
        particle_growth_rate = Range(-10, -10),
        texture = "BurstGold.png",
        particle_life = Range(.2, 1),
        particle_size = Range(.4,  .7),
        life = math.random(10, 20),
        width = Range(0, .6),
        height = Range(0, .6),
        depth = Range(0, .6),
        emission_speed = Range(0, 3),
        emission_rate = Range(.01, 2000)
    }
end

function DaggerActive:__init(node)
	self.node = node
end

function DaggerActive:on_collision(node, position)

	-- Switch states to inactive
	self.node.actor.state = "Inactive"
end

function DaggerActive:on_tick()
	local max_force = 35
	local max_speed = 19
	
	local mass = self.node.rigid_body.mass
	local uuid = self.node.network_monitor.player_uuid
	
	if (true) then --engine.input:key_down(uuid, 'i')) then
	
		-- Auto-correct if the thruster is on
        local forward = self.node.matrix.forward*max_speed
        local direction = forward - self.node.rigid_body.linear_velocity
        
        -- Correct for differences in the direction of the velocity
        -- vector (this is the auto-correct activated after collision or
        -- during a turn)
        local difference = direction.length
        
        -- Select the magnitude of the force applied.  This is always
        -- less than the total force the thrusters can apply
        local magnitude = math.min(max_force, difference*100)
        
    
        if (math.abs(magnitude) < 0.01) then
            -- Do the final correction to get the speed where we want it
            self.node.rigid_body.linear_velocity = forward
        else
            -- Apply the velocity correction by using a force in the
            -- velocity difference direction
            self.node.rigid_body:apply_force(direction.unit * magnitude)
        end
	end
		
	-- Handle each turn individually if the key is pressed, and
    -- add total angular velocity at the end
	local mouse = engine.input:mouse_position(uuid)
	local yaw = mouse.x
	local pitch = mouse.y
	local roll = 0
	
	-- Check for roll input
	if (engine.input:key_down(uuid, 'u')) then
		roll = roll - 1
		print("UU")
	end
	if (engine.input:key_down(uuid, 'o')) then
		roll = roll + 1
	end

    -- Calculate horizontal circular motion
    local hmagnitude = -max_force * yaw
    local hradius = mass * math.pow(max_speed, 2) / hmagnitude
    local hangular = Vector(0, max_speed/hradius, 0)
    hangular = self.node.rotation * hangular
    
    -- Calculate the vertical circular motion
    local vmagnitude = max_force * pitch
    local vradius = mass * math.pow(max_speed, 2) / vmagnitude
    local vangular = Vector(max_speed/vradius, 0, 0)
    vangular = self.node.rotation * vangular
    
    local zmagnitude = max_force * roll
    local zradius = mass * math.pow(max_speed, 2) / zmagnitude
    local zangular = Vector(0, 0, max_speed/zradius)
    zangular = self.node.rotation * zangular
    
    self.node.rigid_body.angular_velocity = hangular + vangular + zangular
end