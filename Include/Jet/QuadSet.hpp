/*
 * Copyright (c) 2010 Matt Fichman
 *
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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Range.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quad.hpp>
#include <Jet/Vertex.hpp>
#include <vector>

namespace Jet {

//! Class for rendering a set of textured quads; useful for billboards and
//! billboad clouds.
//! @class QuadSet
//! Generates particle effects.
class JETAPI QuadSet : public Object {
public:
    
    //! Destructor.
    virtual ~QuadSet() {}
    
    //! Returns the parent node.
    Node* parent() const {
        return parent_;
    }
    
    //! Gets the quad at the given index.
    //! @param index the index of the quad
    inline const Quad& quad(size_t index) const {
        return quad_[index];
    }
    
    //! Returns the list of vertices.
    inline const Vertex* vertex_data() const {
        return vertex_.size() ? &vertex_.front() : 0;
    }
    
    //! Returns the number of vertices.
    inline size_t vertex_count() const {
        return vertex_.size();
    }
    
    //! Sets the quad at the given index
    //! @param index the index of the quad
    //! @param quad the quad.
    void quad(size_t index, const Quad& quad) const;
    
private:
    QuadSet(Engine* engine, Node* parent);
    
    Engine* engine_;
    Node* parent_;
#pragma warning(disable:4251)
    std::vector<Quad> quad_;
    std::vector<Vertex> vertex_;
#pragma warning(default:4251)

    friend class Node;
};

}