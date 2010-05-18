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

require 'Widget'
require 'List'

class 'Menu' (Widget)

function Menu:__init(options)
    Widget.__init(self, options.parent or engine.screen, options.name)
    
    -- Make sure the screen is visible
    self.overlay.visible = true
    
    -- Create a title for the screen
    self.title = self.overlay:overlay(options.name.."_title") {
        height = 0,
        y = 120,
        text_color = Color(1, .4, .1, 1.0),
        font = "Russel.ttf#90",
        text = options.title_text
    }
    
    -- Create the list of buttons
    self.list = List {
        parent = self.overlay,
        name = options.name.."_list",
        y = 230,
        button_width = options.button_width
    }
end

function Menu:button(...)
    self.list:button(...)
end

