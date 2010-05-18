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
require 'Build'

class 'Single' (Module)

function Single:__init()
    Module.__init(self)
    math.randomseed(os.time())

    self.camera_velocity = Vector()
    
    -- Start overlay
    self.menu = Menu {
        name = "sp_menu",
        title_text = "loading..."
    }
    
    -- Animation
    SlideAnimation {
        overlay = self.menu.overlay,
        start_position = "left",
        end_position = 20,
        on_complete = function()
            self:on_load()
        end
    }
end

function Single:on_load()
    -- Create overlay
    print("Creating overlay")
    self.build = Build(nil, "build")
    
    -- Set up scene objects and apply some forces
    print("Creating objects")
    self.s1 = Dagger()
    self.s1.node.position = Vector(-5, -5, 5)

    -- Create rocks
    print("Creating rocks")
    self.rocks = {}
    for i=1,15 do
        self.rocks[i] = Rock()
        local x = math.random(-50, 50)
        local y = math.random(-50, 50)
        local z = math.random(-50, 50)
        local pos = Vector(x, y, z)
        self.rocks[i].node.position = pos
        self.rocks[i].body.angular_velocity = pos.unit * 0.2
        --self.rocks[i].body.linear_velocity = -pos.unit * 5;
    end
    
    -- Animation   
    self.menu.overlay.visible = false
end

function Single:on_update(delta)    
    delta = delta / engine:option("simulation_speed");
    camera_node.position = self.camera_velocity*(60*delta) + camera_node.position
    camera_node:look(Vector(0, 0, 0), Vector(0, 1, 0))
end

function Single:on_key_pressed(key, x, y)

    if (key == 'q') then
        engine.running = false
    elseif (key == 's') then
        if (engine:option("simulation_speed") < 1) then
            engine:option("simulation_speed", 1)
        else
            engine:option("simulation_speed", 1/30)
        end
    elseif (key == 'n') then
        engine:option("normal_mapping_enabled", not engine:option("normal_mapping_enabled"))
    elseif (key == 'h') then
        engine:option("shadows_enabled", not engine:option("shadows_enabled"))
    elseif (key == 'f') then
        if (engine:option("fullscreen_enabled")) then
            engine:option("display_width", 1024)
            engine:option("display_height", 768)
            engine:option("fullscreen_enabled", false)
        else
            engine:option("display_width", 1680)
            engine:option("display_height", 1050)
            engine:option("fullscreen_enabled", true)
        end
        
        engine:option("video_mode_synced", false)
    elseif (key == 'e') then
        if (not self.explosion) then
            self.explosion = Explosion()
        else
            self.explosion:reset()
        end
        self.explosion.node.position = self.s1.node.position;
        self.s1.node:signal(Signal("explode", Vector(0, 0, 9), "hello"))
        
    elseif (key == 'j') then
        self.camera_velocity = self.camera_velocity + Vector(-1, 0, 0)
    elseif (key == 'l') then
        self.camera_velocity = self.camera_velocity + Vector(1, 0, 0)
    elseif (key == 'i') then
        self.camera_velocity = self.camera_velocity + Vector(0, 1, 0)
    elseif (key == 'k') then
        self.camera_velocity = self.camera_velocity + Vector(0, -1, 0)
    elseif (key == 'u') then
        self.camera_velocity = self.camera_velocity + Vector(0, 0, -1)
    elseif (key == 'o') then
        self.camera_velocity = self.camera_velocity + Vector(0, 0, 1)
    end
end

function Single:on_key_released(key, x, y)
    if (key == 'j') then
        self.camera_velocity = self.camera_velocity + Vector(1, 0, 0)
    elseif (key == 'l') then
        self.camera_velocity = self.camera_velocity + Vector(-1, 0, 0)
    elseif (key == 'i') then
        self.camera_velocity = self.camera_velocity + Vector(0, -1, 0)
    elseif (key == 'k') then
        self.camera_velocity = self.camera_velocity + Vector(0, 1, 0)
    elseif (key == 'u') then
        self.camera_velocity = self.camera_velocity + Vector(0, 0, 1)
    elseif (key == 'o') then
        self.camera_velocity = self.camera_velocity + Vector(0, 0, -1)
    end
    
end