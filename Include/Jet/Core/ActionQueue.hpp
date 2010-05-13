/*
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <Jet/Core/Types.hpp>
#include <Jet/ActionQueue.hpp>

namespace Jet { namespace Core {

//! Manages semantic input signals for the game.
//! @class ActionQeue
//! @brief Manages semantic input signals for the game.
class ActionQueue : public Jet::ActionQueue {
public:
    //! Creates a new queue.
    ActionQueue() {
        
    }

    //! Returns the ith action currently at the top of the queue.
    inline std::string action() {
        if (action_.empty()) {
            throw std::runtime_error("Action queue is empty");
        } else {
            std::string action = action_.back();
            action_.pop_back();
            return action;
        }
    }
    
     //! Returns number of actions in the queue
    inline size_t action_count() const {
        return action_.size();
    }
    
    //! Offers an action for the next input tick.  This is usually a few
    //! ticks ahead of the current game tick.
    //! @param action the action to record for this tick
    inline void action(const std::string& action) {
        action_.push_back(action);
    }
    
private:
    std::vector<std::string> action_;
};

}}