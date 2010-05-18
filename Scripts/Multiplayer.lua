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
require 'SlideAnimation'

class 'Multiplayer' (Module)

function Multiplayer:__init()
    Module.__init(self)

    -- Multiplayer overlay
    self.menu = Menu {
        name = "mp_menu",
        title_text = "multiplayer.",
        button_width = 230
    }
    self.menu:button("host game", bind("on_host_click", self))
    self.menu:button("back", bind("on_back_click", self))
    
    -- Games list
    self.list = List {
        parent = self.menu.overlay,
        name = "games_list",
        font_size = 24,
        y = 280,
        x = 380
    }
    self.list:button("matt's game", bind("on_null", self))
    self.list:button("bob's game", bind("on_null", self))
    self.list:button("mikes's game", bind("on_null", self))
    self.list:button("jim's game", bind("on_null", self))
    self.list:button("jon's game", bind("on_null", self))
    self.list:button("bill's game", bind("on_null", self))
    
    -- Animation
    SlideAnimation {
        overlay = self.menu.overlay,
        start_position = "left",
        end_position = 20
    }
end

function Multiplayer:on_null()
end

function Multiplayer:on_host_click(widget, button)
end

function Multiplayer:on_back_click(widget, button)
    SlideAnimation {
        overlay = self.menu.overlay,
        end_position = "right",
        on_complete = Start
    }
end

function Multiplayer:on_destroy()
    self.menu.overlay.visible = false
end
