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
require 'SlideAnimation'

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
        font = options.title_font or "Russel.ttf#90",
        text = options.title_text
    }
    
    -- Create the list of buttons
    self.list = List {
        parent = self.overlay,
        name = options.name.."_list",
        font_face = options.font_face,
        font_size = options.font_size,
        y = 240,
        button_width = options.button_width
    }
    
    -- Create a slide-in animation
    SlideAnimation {
        overlay = self.overlay,
        start_position = "left",
        end_position = 20,
        on_complete = options.on_complete
    }
end

function Menu:clear()
    self.list:clear()
end

function Menu:label(...)
    return self.list:label(...)
end

function Menu:button(...)
    return self.list:button(...)
end

function Menu:text_field(...)
    return self.list:text_field(...)
end

function Menu:next(on_complete)
    SlideAnimation {
        overlay = self.overlay,
        end_position = "right",
        on_complete = function()
            self.overlay.visible = false
            self:clear()
            on_complete()
        end
    }
end

