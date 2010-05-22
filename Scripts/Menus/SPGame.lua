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

require 'Module'
require 'Shark'
require 'Dagger'
require 'Monkey'
require 'Rock'
require 'Box'
require 'Explosion'
require 'Bullet'
require 'Build'

class 'SPGame' (Module)

function SPGame:__init()
    Module.__init(self)
    math.randomseed(os.time())

    self.yaw = 0
    self.pitch = 0
    self.camera_position = Vector()
    self.camera_rotation = Quaternion()
    self.bullet = {}
    self.bullet_index = 0
    
    -- Start overlay
    self.menu = Menu {
        name = "sp_menu",
        title_text = "loading...",
        on_complete = function() self:on_load() end
    }
end

function SPGame:on_load()
    -- Create overlay
    print("Creating overlay")
    self.build = Build(nil, "build")
    
    -- Set up scene objects and apply some forces
    print("Creating objects")
    self.ship = Shark()
    self.ship.node.position = Vector(-5, -5, 5)
    
    local dagger = Shark()
    dagger.node.position = Vector(10, 10, 10)

    -- Create rocks
    print("Creating rocks")
    self.rocks = {}
    for i=1,10 do
        self.rocks[i] = Rock()
        local x = math.random(-100, 100)
        local y = math.random(-100, 100)
        local z = math.random(-100, 100)
        local pos = Vector(x, y, z)
        self.rocks[i].node.position = pos
        
        local x = math.random(-100, 100)
        local y = math.random(-100, 100)
        local z = math.random(-100, 100)
        local pos = Vector(x, y, z)
        self.rocks[i].body.angular_velocity = pos.unit * 0.4

        --self.rocks[i].body.linear_velocity = -pos.unit * 5;
    end
    
    self.menu.overlay.visible = false
    self.thrust = true
end

function SPGame:on_update(delta)
    self:update_camera(delta)
end

function SPGame:on_tick()
    self:update_ship()
end

function SPGame:on_mouse_motion(point)
    self.yaw = point.x
    self.pitch = point.y
end

function SPGame:on_mouse_pressed(button, point)
    self.bullet[self.bullet_index] = self.bullet[self.bullet_index] or Bullet()
    
    local forward = self.ship.node.matrix.forward
    self.bullet[self.bullet_index].node.position = self.ship.node.position + forward * 4
    self.bullet[self.bullet_index].body.linear_velocity = forward * 80 + self.ship.body.linear_velocity    
    self.bullet_index = (self.bullet_index + 1) % 10
end

function SPGame:on_key_pressed(key, point)

    if (key == 'q') then
        engine.running = false
    elseif (key == 'i') then
        --self.thrust = true
        --self.ship.flame.life = -1
    end
end

function SPGame:on_key_released(key, point)
    
    if (key == 'i') then
        --self.thrust = false
        --self.ship.flame.life = 0
    end
    
end

function SPGame:update_camera(delta)
    if (not self.ship) then return end

    if (self.ship.dead) then
        camera_node.position = camera_node.position + camera_velocity * delta
        camera_node:look(self.ship.node.position, camera_node.matrix.up)
    else
        local alpha1 = .09
        local alpha2 = .06
        
        
        camera_velocity = self.ship.body.linear_velocity
        
        -- Do a slerp on the expected rotation
        self.camera_rotation = self.camera_rotation:slerp(self.ship.node.rotation, alpha1)
        
        local position = self.camera_rotation * Vector(0, 4, -18)
        local up = self.camera_rotation * Vector(0, 1, 0)
        local target = self.camera_rotation * Vector(0, 0, 20)
        
        
        -- Now do a lerp to bring the position into line
        self.camera_position = self.camera_position:lerp(position, alpha2)
        
        camera_node.position = self.camera_position + self.ship.node.position
        camera_node:look(self.ship.node.position + target, up)
        --camera_node.position

    end
end

function SPGame:update_ship()
    if (not self.ship or self.ship.dead) then return end


    local max_force = 35
    local max_speed = 19
    
    local mass = self.ship.body.mass

    -- Handle the thrust and automatic course correction
    if (self.thrust) then
        
        -- Auto-correct if the thruster is on
        local forward = self.ship.node.matrix.forward*max_speed
        local direction = forward - self.ship.body.linear_velocity
        
        -- Correct for differences in the direction of the velocity
        -- vector (this is the auto-correct activated after collision or
        -- during a turn)
        local difference = direction.length
        
        -- Select the magnitude of the force applied.  This is always
        -- less than the total force the thrusters can apply
        local magnitude = math.min(max_force, difference*100)
        
        
        if (math.abs(magnitude) < 0.01) then
            -- Do the final correction to get the speed where we want it
            self.ship.body.linear_velocity = forward
        else
            -- Apply the velocity correction by using a force in the
            -- velocity difference direction
            self.ship.body:apply_force(direction.unit * magnitude)
        end
    end
    
    -- Handle each turn individually if the key is pressed, and
    -- add total angular velocity at the end
    
    -- Calculate horizontal circular motion
    local hmagnitude = -max_force * self.yaw
    local hradius = mass * math.pow(max_speed, 2) / hmagnitude
    local hangular = Vector(0, max_speed/hradius, 0)
    hangular = self.ship.node.rotation * hangular
    
    -- Calculate the vertical circular motion
    local vmagnitude = max_force * self.pitch
    local vradius = mass * math.pow(max_speed, 2) / vmagnitude
    local vangular = Vector(max_speed/vradius, 0, 0)
    vangular = self.ship.node.rotation * vangular
    
    self.ship.body.angular_velocity = hangular + vangular
end
