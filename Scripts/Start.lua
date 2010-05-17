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
require 'List'
require 'Rock'
require 'Multiplayer'

class 'Start' (Module)

function Start:__init()
    Module.__init(self)
    
    -- Start overlay
    self.screen = engine.screen:overlay("start_screen") {
        visible = true,
        x = 20
    }
    
    -- Title for the main screen
    self.title = self.screen:overlay("start_title") {
        height = 0,
        y = 120,
        text_color = Color(1, .4, .1, 1.0),
        font = "Russel.ttf#96",
        text = "zero combat."
    }
    
    -- List for start screen buttons
    self.list = List(self.screen, "start_list")
    self.list:button("multiplayer", callback(self, "on_mp_click"))
    self.list:button("single player", callback(self, "on_sp_click"))
    self.list:button("quit", callback(self, "on_quit_click"))
    self.list.overlay.y = 250
    
    -- Set up the box
    self.box_node = engine.root:node("box")
    self.box_node.position = Vector(0, 0, -14)
    self.box = self.box_node:mesh_object("box") {
        mesh = "Box.obj",
        material = "Metal.mtl"
    }
    self.box_body = self.box_node:rigid_body()
    self.box_body.mass = 1000.0
    self.box_body.angular_velocity = Vector(.05, .2, 0)
end

function Start:on_key_pressed(key)
    if (key == 'q') then
        engine.running = false
    end
end

function Start:on_mp_click(widget, buttton)
    Multiplayer()
end

function Start:on_sp_click(widget, buttton)
end

function Start:on_quit_click(widget, buttton)
    engine.running = false
end

function Start:on_destroy()
    self.screen.visible = false
end
