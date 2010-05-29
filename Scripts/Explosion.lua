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

class 'ExplosionActive' (ActorState)
class 'ExplosionInactive' (ActorState)

function Explosion()
    local node = engine.root:node()
    
    -- Create some particle systems
    local explosion = node:particle_system()
    explosion.type = ParticleSystem.ET_ELLIPSOID
    explosion.quota = 800
    explosion.texture = "BurstGold.png"
    explosion.particle_life = Range(.7, .7)
    explosion.particle_size = Range(9, 9)
    explosion.life = .12
    explosion.width = Range(.3, 4)
    explosion.height = Range(.3, 4)
    explosion.depth = Range(.3, 4)
    explosion.emission_speed = Range(.25, .25)
    explosion.emission_rate = Range(250, 250)
 
    local sparks = node:particle_system()
    sparks.type = ParticleSystem.ET_ELLIPSOID
    sparks.quota = 200
    sparks.texture = "BurstGold.png"
    sparks.particle_growth_rate = Range(-10, -10)
    sparks.particle_life = Range(1, 1.3)
    sparks.particle_size = Range(.5, 1)
    sparks.life = 3
    sparks.width = Range(3, 4)
    sparks.height = Range(3, 4)
    sparks.depth = Range(3, 4)
    sparks.emission_speed = Range(0, 8)
    sparks.emission_rate = Range(500, 500)
    
    local smoke = node:particle_system()
    smoke.type = ParticleSystem.ET_ELLIPSOID
    smoke.quota = 2
    smoke.texture = "SmokeWhite.png"
    smoke.particle_life = Range(3, 4)
    smoke.particle_size = Range(9, 10)
    smoke.life = .1
    smoke.width = Range(1, 3)
    smoke.height = Range(1, 3)
    smoke.depth = Range(1, 3)
    smoke.emission_speed = Range(.6, .6)
    smoke.emission_rate = Range(300, 300)
   
	-- Positional audio for the explosion
    local audio = node.audio_source
    audio:sound(0, "Grenade1.wav")
    audio:sound(1, "Grenade2.wav")
    audio:sound(2, "Grenade3.wav")
    audio:sound(3, "Grenade4.wav")
    
        
    node.sparks = sparks
    node.explosion = explosion
    node.smoke = smoke
    
    -- Create an actor
    node.actor:actor_state("Active", ExplosionActive(node))
    node.actor:actor_state("Inactive", ExplosionInactive(node))
    node.actor.state = "Active"
    
    return node
end

function ExplosionActive:__init(node)
	self.node = node
end

function ExplosionActive:on_state_enter()
    self.node.sparks.life = .1
    self.node.explosion.life = .12
    self.node.smoke.life = .1
    --self.node.audio:state(math.random(0, 3), AudioSource.PS_PLAY)
end

function ExplosionActive:on_tick()
    if (self.node.sparks.life <= 0) then
        self.node.actor.state = "Inactive"
    end
end

function ExplosionInactive:__init(node)
	self.node = node
end


