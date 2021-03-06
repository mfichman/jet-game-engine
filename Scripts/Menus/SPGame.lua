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
require 'Dagger'
require 'Rock'
require 'Explosion'
require 'Bullet'
require 'Build'

class 'SPGame' (Module)

function SPGame:__init()
    Module.__init(self)
    math.randomseed(os.time())

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
    self.ship = Dagger("Red")
    self.ship.position = Vector(0, 0, 300) 
    self.ship:look(Vector(0, 0, 0), Vector(0, 1, 0))
    if Network.NS_DISABLED ~= engine.network.state then
        self.ship.network_monitor.player_uuid = engine.network:player(0).uuid  
    else
        self.ship.network_monitor.player_uuid = engine.network.current_player.uuid
    end
    
    -- Create rocks
    print("Creating rocks")
    math.randomseed(10)
    
    for i=1,2 do
		local rock = Rock("Large")
		local x = math.random(-120, 120)
		local y = math.random(-15, 15)
		local z = math.random(-80, 80)
        rock.position = Vector(x, y, z)
        rock.rotation = Quaternion(1, x, y, z).unit
	end
	
	for i=1,24 do
		local rock = Rock("Medium")		
		local x = math.random(-120, 120)
		local y = math.random(-15, 15)
		local z = math.random(-80, 80)
        rock.position = Vector(x, y, z)
        rock.rotation = Quaternion(1, x, y, z).unit
	end
		
	for i=1,32 do
		local rock = Rock("Small")
		local x = math.random(-120, 120)
		local y = math.random(-15, 15)
		local z = math.random(-80, 80)
        rock.position = Vector(x, y, z)
        rock.rotation = Quaternion(1, x, y, z).unit
	end
    
    self.menu.overlay.visible = false
    
    print("sending RPC")
    engine.network:reliable_rpc("rpc_test", "hello, world!", 1, 2, 3, 4, 5, true)
end

function SPGame:rpc_test(...)
	print("RPC!!", ...)
end

function SPGame:on_update(delta)
    if not self.ship then return end

    if Network.NS_DISABLED ~= engine.network.state then
        self.ship.network_monitor.player_uuid = engine.network:player(0).uuid  
    else
        self.ship.network_monitor.player_uuid = engine.network.current_player.uuid
    end

	--[[if (engine.network.state == Network.NS_CLIENT) then
		if (self.ship) then
			camera_node:look(self.ship.position, Vector(0, 1, 0))
		end
	else]]
		self:update_camera(delta)
	--end
end

function SPGame:update_camera(delta)
    if (not self.ship) then return end

   
    if (self.ship.actor.state == "Active") then
        self.ship_position = Vector(self.ship.position)
        self.ship_rotation = Quaternion(self.ship.rotation)
    end
    

    local alpha1 = .09*delta*60
    local alpha2 = .06*delta*60
    
    -- Do a slerp on the expected rotation
    self.camera_rotation = self.camera_rotation:slerp(self.ship_rotation, alpha1)
    
    local position = self.camera_rotation * Vector(0, 4, -18)
    local up = self.camera_rotation * Vector(0, 1, 0)
    local target = self.camera_rotation * Vector(0, 0, 20)
    
    
    -- Now do a lerp to bring the position into line
    self.camera_position = self.camera_position:lerp(position, alpha2)
    camera_node.position = self.camera_position + self.ship_position
    
    if (self.ship.actor.state == "Inactive") then
        camera_node:look(self.ship.position + target, up)
    else
        camera_node:look(self.ship_position + target, up)
    end
end


function SPGame:on_mouse_pressed(button, point)
    --self.audio:state(0, AudioSource.PS_PLAY)
	if (not self.ship) then return end
	
	if (not self.bullet[self.bullet_index]) then
		self.bullet[self.bullet_index] = Bullet()
	end
    
    local forward = self.ship.matrix.forward
    self.bullet[self.bullet_index].position = self.ship.position + forward * 3
    self.bullet[self.bullet_index].rigid_body.linear_velocity = forward * 150 + self.ship.rigid_body.linear_velocity
    self.bullet[self.bullet_index].actor.state = "Active"
    self.bullet_index = (self.bullet_index + 1) % 10
end
