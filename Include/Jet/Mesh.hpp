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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Vertex.hpp>

namespace Jet {

//! Allows modification of a hardware mesh buffer, used in physics and 
//! rendering.  Changes to the buffer are stored in application memory, and
//! they are not pushed to the hardware until the flush() method is called.
//! @class Mesh
//! @brief Class for modifing a mesh, or building a new mesh.
class Mesh : public Object {
public:

    //! Destructor
    virtual ~Mesh() {}

    //! Returns the source file for this mesh
    virtual const std::string& source() const=0;

    //! Returns the vertex at this index.
    virtual const Vertex& vertex(size_t index) const=0;

    //! Returns the normal at the index.  If the index is larger than the size
    //! of the buffer, the zero vector will be returned.
    //! @param index the normal at the given index.
    virtual const Vector& normal(size_t index) const=0;

    //! Returns the binormal at the given index.  If the index is larger than
    //! the size of the buffer, the zero vector will be returned.
    //! @param index the binormal at the given index.
    virtual const Vector& binormal(size_t index) const=0;

    //! Returns the position at the given index.  If the index is larger than
    //! the size of the buffer, the zero vector will be returned.
    //! @param index the position at the given index.
    virtual const Vector& position(size_t index) const=0;

    //! Return the texture coordinate at the given index.  If the index is
    //! larger than the size of the buffer, the zero vector will be returned.
    //! @param index the texcoord at the given index
    virtual const Texcoord& texcoord(size_t index) const=0;

    //! Return the index from the index buffer at the given index.  If the
    //! index is larger than the size of the buffer, zero will be returned.
    virtual uint32_t index(size_t index) const=0;

    //! Sets or resets the data in this mesh to the contents of the given 
    //! source file
    virtual void source(const std::string& source)=0;

    //! Sets the normal at the given index.  If the index is larger than the
    //! size of the buffer, zero vectors are filled up to the given index.
    //! The buffer will be resized.
    virtual void normal(size_t index, const Vector& normal)=0;

    //! Sets the binormal at the given index.  If the index is larger than the
    //! size of the buffer, zero vectors are filled in up to the given index.
    //! The buffer will be resized.
    virtual void binormal(size_t index, const Vector& binormal)=0;

    //! Sets the position at the given index.  If the index is larger than the
    //! size of the buffer, zero vectors are filled in up to the given index.
    virtual void position(size_t index, const Vector& position)=0; 

    //! Sets the texture coordinate at the given index.  If the index is larger
    //! than the size of the buffer, zero is filled in up to the given index.
    virtual void texcoord(size_t index, const Texcoord& texcoord)=0;

    //! Sets the index in the index buffer.  If the index is larger than the
    //! size of the buffer, zero vectors are filled up to the given index.
    virtual void index(size_t index, uint32_t ind)=0; 

    //! Sets the vertex at the given index.  If the index is larger than the
    //! size of the buffer, zero is filled in up to the given index.
    virtual void vertex(size_t index, const Vertex& vertex)=0;

    //! Flushes changes to the graphics card/physics engine.  Note that when
    //! manipulating a Mesh, the changes are done in application memory, and 
    //! so flush must be called before the update takes place.
    virtual void flush()=0; 

protected:
    //! Clones this mesh object
    virtual Mesh* clone() const=0;
};

}
