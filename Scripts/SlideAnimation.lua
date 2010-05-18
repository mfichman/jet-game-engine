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

require 'Task'

class 'SlideAnimation' (Task)

function SlideAnimation:__init(options)
    Task.__init(self)

    self.overlay = options.overlay
    self.start_position = options.start_position
    self.slide_speed = options.slide_speed or 8000
    self.end_position = options.end_position or "right"
    self.on_complete = options.on_complete
    
    
    if (self.end_position == "left") then
        self.end_position = -options.overlay.width
    elseif (self.end_position == "right") then
        self.end_position = engine.screen.width
    end
    
    if (self.start_position == "left") then
        self.start_position = -self.overlay.width
    elseif (self.start_position == "right") then
        self.start_position = engine.screen.width
    elseif (not self.start_position) then
        self.start_position = self.overlay.x
    end
    
    local delta = self.end_position - self.start_position
    self.slide_velocity = delta/math.abs(delta)*self.slide_speed
    self.overlay.x = self.start_position
    self.tag = math.random()
end

function SlideAnimation:on_run()
    while (self.overlay) do
    print("go")
        local step = self.slide_velocity*engine.frame_delta
    
        if (math.abs(self.overlay.x - self.end_position) <= math.abs(step)) then
            self.overlay.x = self.end_position
            self.overlay = nil
            if (self.on_complete) then
                self.on_complete()
            end
        else
            self.overlay.x = self.overlay.x + step
        end
        
        coroutine.yield()
    end
    
    print("done")
end
