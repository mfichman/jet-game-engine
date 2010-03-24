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

#include <Jet/Jet.hpp>
#include <Jet/Vertex.hpp>

namespace Jet {

//! Allows modification of a hardware mesh buffer, used in physics and 
//! rendering.  Changes to the buffer are stored in application memory, and
//! they are not pushed to the hardware until the flush() method is called.
//! @class Mesh
//! @brief Class for modifing a mesh, or building a new mesh.
class MeshObject : public Object {
public:

    //! Destructor
    virtual ~MeshObject() {}

    //! Gets the parent scene node.
    virtual SceneNode* get_parent() const=0;

    //! Gets the name of the current mesh.
    virtual const std::string& get_mesh() const=0;

    //! Gets the material name.
    virtual const std::string& get_material() const=0;

    //! Sets the mesh.
    //! @param mesh the mesh to set for this object
    virtual void get_mesh(const std::string& name)=0;
    
    //! Sets the material for this mesh object.
    //! @param name the name of the material
    virtual void get_material(const std::string& name)=0;
};

}
