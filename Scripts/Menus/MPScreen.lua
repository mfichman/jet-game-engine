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
require 'List'
require 'MPHost'
require 'MPLobby'

class 'MPScreen' (Module)

function MPScreen:__init()
    Module.__init(self)
    
    -- Screen overlay
    self.menu = Menu {
        name = "mpscreen_menu",
        title_text = "multiplayer.",
        button_width = 330
    }
    self.player_name = self.menu:text_field("name")
    self.player_name.buffer = "anonymous"
    
    self.host_button = self.menu:button("host game")
    self.host_button.on_click = bind("on_host_click", self)
    
    self.back_button = self.menu:button("back")
    self.back_button.on_click = bind("on_back_click", self)
    
    -- Games list
    self.list = List {
        parent = self.menu.overlay,
        name = "games_list",
        font_size = 24,
        y = 320,
        x = 380
    }
    
    -- Put the network into discover mode
    engine.network.state = Network.NS_DISCOVER
end

function MPScreen:on_game_click(widget, button)

    for i=1,engine.network.match_count do
        local match = engine.network:match(i-1)
        if (widget.overlay.text == match.name) then
            engine.network.current_match = match
            break
        end
    end

    engine.network.current_player = Player(self.player_name.buffer)
    self.menu:next(MPLobby)
end

function MPScreen:on_match_list_update()
    self.list:clear()
    for i=1,engine.network.match_count do
        local button = self.list:button(engine.network:match(i-1).name)
        button.on_click = bind("on_game_click", self)
    end
end

function MPScreen:on_host_click(widget, button)
    engine.network.current_player = Player(self.player_name.buffer)
    self.menu:next(MPHost)
end

function MPScreen:on_back_click(widget, button)
    self.menu:next(StartScreen)
end

function MPScreen:on_destroy()
    self.list:clear()
end
