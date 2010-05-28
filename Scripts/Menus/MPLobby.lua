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
require 'Menu'

class 'MPLobby' (Module)

function MPLobby:__init()
    Module.__init(self)
    
    -- Overlay
    self.menu = Menu {
        name = "mplobby_menu",
        title_text = engine.network.current_match.name,
        title_font = "Russel.ttf#48",
        button_width = 230
    }
    
    self.menu.title.y = 10
    self.menu.list.overlay.y = 70
    self.back_button = self.menu:button("back")
    self.back_button.on_click = bind("on_back_click", self)
    
    -- Player list
    self.list = List {
        parent = self.menu.overlay,
        name = "player_list",
        font_size = 32,
        y = 240,
        x = 40
    }
    
    -- Numbers
    self.numbers = List {
        parent = self.menu.overlay,
        name = "number_list",
        font_size = 32,
        font_face = "Impact.ttf",
        y = 240,
        x = 4
    }
    
    -- Set put us into host mode
    engine.network.state = Network.NS_CLIENT
end

function MPLobby:on_player_list_update(number, name)
    self.list:clear()
    self.numbers:clear()
    for i=1,engine.network.player_count do
        self.list:label(engine.network:player(i-1).name)
        self.numbers:label(tostring(i))
    end
end

function MPLobby:on_network_error()
    self.menu:next(MPScreen)
end

function MPLobby:on_go_click(widget, button)
    -- Launch the game
    
end

function MPLobby:on_back_click(widget, button)
    self.menu:next(MPScreen)
end
