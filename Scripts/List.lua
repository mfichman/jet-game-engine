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
require 'Button'
require 'TextField'
require 'Engine'

class 'List' (Widget)

function List:__init(options)
    Widget.__init(self, options.parent or engine.screen, options.name)

    -- Default options
    self.name = options.name
    self.parent = engine.screen or options.parent
    self.font_size = options.font_size or 48
    self.font_face = options.font_face or "Russel.ttf"
    self.button_width = options.button_width or 380
    self.button_spacing = options.button_spacing or 10
    self.overlay.x = options.x or 0
    self.overlay.y = options.y or 0
    self.buttons = {}
    self.overlay.height = 0
end


function List:text_field(text, callback)
    -- Creates a new button
    local n = table.getn(self.buttons)+1
    local t = TextField(self.overlay, self.name..n)
    t.overlay {
        font = self.font_face.."#"..self.font_size,
        height = self.font_size,
        width = self.button_width,
        text = text,
        y = self.overlay.height
    }
    t.on_enter = callback
    
    -- Increase the height of the list to accomodate the button
    self.overlay.height = self.overlay.height + self.button_spacing + self.font_size
    
    self.buttons[n] = t
end

function List:button(text, callback)
    -- Creates a new button
    local n = table.getn(self.buttons)+1
    local b = Button(self.overlay, self.name..n)
    b.overlay {
        font = self.font_face.."#"..self.font_size,
        height = self.font_size,
        width = self.button_width,
        text = text,
        y = self.overlay.height
    }
    b.on_click = callback
    
    -- Increase the height of the list to accomodate the button
    self.overlay.height = self.overlay.height + self.button_spacing + self.font_size
    
    self.buttons[n] = b
end
