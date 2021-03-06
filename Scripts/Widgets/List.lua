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
require 'Label'
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
    self.elements = {}
    self.overlay.height = 0
end

function List:clear()
    for i,v in ipairs(self.elements) do
        v.overlay.visible = false
        v.overlay.text = ""
        v.on_click = function() end
    end
    self.elements = {}
    self.overlay.height = 0
end

function List:remove(name)
    for i,v in ipairs(self.elements) do
        if (v.overlay.text == name) then
            local temp = v;
            local n = table.getn(self.elements)
            self.elements[i].overlay.text = self.elements[n].overlay.text
            self.elements[n].overlay.text = ""
            self.elements[n].visible = false
            self.elements[n] = nil
            self.overlay.height = self.overlay.height - self.button_spacing - self.font_size
            break
        end
    end
end

function List:label(text)
    -- Creates a new button
    local n = table.getn(self.elements)+1
    local l = Label(self.overlay, self.name..n)
    l.overlay {
        visible = true,
        text = text,
        font = self.font_face.."#"..self.font_size,
        height = self.font_size,
        width = self.button_width,
        y = self.overlay.height
    }
    
    -- Increase the height of the list to accomodate the button
    self.overlay.height = self.overlay.height + self.button_spacing + self.font_size
    self.elements[n] = l
    
    return l
end

function List:text_field(label)
    -- Creates a new button
    local n = table.getn(self.elements)+1
    local t = TextField(self.overlay, self.name..n)
    t.overlay {
        visible = true,
        font = self.font_face.."#"..self.font_size,
        height = self.font_size,
        width = self.button_width,
        y = self.overlay.height
    }
    t.label = label
    t.buffer = text
    
    -- Increase the height of the list to accomodate the button
    self.overlay.height = self.overlay.height + self.button_spacing + self.font_size
    self.elements[n] = t
    
    return t
end

function List:button(text)
    -- Creates a new button
    local n = table.getn(self.elements)+1
    local b = Button(self.overlay, self.name..n)
    b.overlay {
        visible = true,
        font = self.font_face.."#"..self.font_size,
        height = self.font_size,
        width = self.button_width,
        text = text,
        y = self.overlay.height
    }
    
    -- Increase the height of the list to accomodate the button
    self.overlay.height = self.overlay.height + self.button_spacing + self.font_size
    self.elements[n] = b
    
    return b
end
