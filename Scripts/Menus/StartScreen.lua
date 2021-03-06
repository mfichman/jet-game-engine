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
require 'Menu'
require 'SPGame'
require 'MPScreen'

class 'StartScreen' (Module)

function StartScreen:__init()
    Module.__init(self)
    
    -- Create the menu
    self.menu = Menu {
        name = "startscreen_menu",
        title_text = "zero combat."
    }
    self.mp_button = self.menu:button("multiplayer")
    self.mp_button.on_click = bind("on_mp_click", self)
    
    self.sp_button = self.menu:button("single player")
    self.sp_button.on_click = bind("on_sp_click", self)
    
    self.quit_button = self.menu:button("quit")
    self.quit_button.on_click = bind("on_quit_click", self)

    -- Set up the box
    local box_node = engine.root:node("box1")
    box_node.position = Vector(0, 0, -14)
    local box = box_node:mesh_object("box") {
        mesh = "Box.obj",
        material = "Metal.mtl"
    }    
    local box_body = box_node.rigid_body
    box_body.mass = 0
      

    engine.network.state = Network.NS_DISABLED
end

function StartScreen:on_key_pressed(key)
    if (key == 'q') then
        engine.running = false
    end
end

function StartScreen:on_mp_click(widget, buttton)
    self.menu:next(MPScreen)
end

function StartScreen:on_sp_click(widget, buttton)
    self.menu:next(SPGame)
end

function StartScreen:on_quit_click(widget, buttton)
    engine.running = false
end
