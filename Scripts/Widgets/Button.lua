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
require 'Engine'

class 'Button' (Widget)

function Button:__init(overlay, name)
    Widget.__init(self, overlay, name)

    -- Default options
    self.overlay.font = "Russel.ttf#14"
    self.overlay.vertical_alignment = Overlay.AL_TOP
    self.overlay.x = 4
    self.overlay.y = 4
    self.overlay.width = 800
    self.overlay.height = 16
    
    if (self.overlay.mouse_over) then
        self.overlay.text_color = Color(1, 1, 1, 1)
        self.color_fade = 1
    else
        self.overlay.text_color = Color(1, 1, 1, 0.7)
        self.color_fade = -1
    end
    
end

function Button:on_update(delta)
    local alpha = self.overlay.text_color.alpha + 2*self.color_fade*delta
    self.overlay.text_color = Color(1, 1, 1, math.clamp(alpha, .7, 1))
end

function Button:on_mouse_pressed(button)
    self:on_click(button)
end

function Button:on_mouse_enter()
    self.color_fade = 1;
end

function Button:on_mouse_exit()
    self.color_fade = -1;
end
