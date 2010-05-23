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

class 'Explosion' (ActorSupport)
--class 'Explosion.Alive' (ActorState)
--class 'Explosion.Dead' (ActorState)

state 'Explosion.Alive'
state 'Explosion.Dead'

function Explosion:__init()

    self.node = engine.root:node()
    ActorSupport.__init(self, Explosion)
    
    
    self.explosion = self.node:particle_system() {
        type = ParticleSystem.ET_ELLIPSOID,
        quota = 800,
        texture = "BurstGold.png",
        particle_life = Range(.7, .7),
        particle_size = Range(9, 9),
        life = .12,
        width = Range(.3, 4),
        height = Range(.3, 4),
        depth = Range(.3, 4),
        emission_speed = Range(.25, .25),
        emission_rate = Range(250, 250)
    }
    
    self.sparks = self.node:particle_system() {
        type = ParticleSystem.ET_ELLIPSOID,
        quota = 9,
        texture = "IncandescentGold.png",
        particle_life = Range(.6, .6),
        particle_size = Range(.8, .8),
        life = .1,
        width = Range(.8, .8),
        height = Range(.8, .8),
        depth = Range(.8, .8),
        emission_speed = Range(30, 40),
        emission_rate = Range(1000, 1000)
    }
    
    self.smoke = self.node:particle_system() {
        type = ParticleSystem.ET_ELLIPSOID,
        quota = 2,
        texture = "SmokeWhite.png",
        particle_life = Range(1, 1),
        particle_size = Range(9, 10),
        life = .1,
        width = Range(1, 3),
        height = Range(1, 3),
        depth = Range(1, 3),
        emission_speed = Range(.6, .6),
        emission_rate = Range(300, 300)
    }
    
    self.actor.state = "Alive"
end


function Explosion.Alive:on_state_enter()
    self.sparks.life = .1
    self.explosion.life = .12
    self.smoke.life = .1
end

function Explosion.Alive:on_tick()
    if (self.explosion.life <= 0) then
        self.actor.state = "Dead"
    end
end

