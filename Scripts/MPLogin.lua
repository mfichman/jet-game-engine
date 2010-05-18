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
require 'MPScreen'

class 'MPLogin' (Module)

function MPLogin:__init()
    Module.__init(self)
    
    -- Overlay
    self.menu = Menu {
        name = "mplogin_menu",
        title_text = "login.",
        button_width = 230
    }
    self.menu:text_field("", bind("on_text_enter", self))
    self.menu:button("ok", bind("on_ok_click", self))
    self.menu:button("back", bind("on_back_click", self))
    
end

function MPLogin:on_text_enter(widget, button)

end

function MPLogin:on_ok_click(widget, button)
    self.menu:next(MPScreen)
end

function MPLogin:on_back_click(widget, button)
    self.menu:next(StartScreen)
end
