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

class 'Multiplayer' (Module)

function Multiplayer:__init()
    Module.__init(self)

    -- Multiplayer overlay
    self.screen = engine.screen:overlay("multiplayer_screen") {
        visible = true,
        x = 20
    }
    
    -- Title for the MP screen
    self.title = self.screen:overlay("mp_title") {
        height = 0,
        y = 120,
        text_color = Color(1, .4, .1, 1.0),
        font = "Russel.ttf#90",
        text = "multiplayer."
    }
    
    -- List for MP screen buttons
    self.list = List(self.screen, "mp_list")
    self.list.button_width = 320
    self.list:button("host game", callback(self, "on_host_click"))
    self.list:button("back", callback(self, "on_back_click"))
    self.list.overlay.y = 230
    
    self.list = List(self.screen, "games_list")
    self.list.overlay.y = 235
    self.list.overlay.x = 360
    self.list.font_size = 24
    self.list:button("matt's game", callback(self, "b"))
    self.list:button("bob's game", callback(self, "a"))
    self.list:button("mikes's game", callback(self, "b"))
    self.list:button("jim's game", callback(self, "a"))
    self.list:button("jon's game", callback(self, "b"))
    self.list:button("bill's game", callback(self, "a"))

end

function Multiplayer:a()
end

function Multiplayer:b()
end

function Multiplayer:on_host_click(widget, button)
    print("host")
end

function Multiplayer:on_more_click(widget, button)
end

function Multiplayer:on_back_click(widget, button)
    Start()
end

function Multiplayer:on_destroy()
    self.screen.visible = false
end
