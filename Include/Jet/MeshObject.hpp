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

#include <Jet/Jet.hpp>
#include <Jet/SceneComponent.hpp>
#include <string>

namespace Jet {

//! Displays a solid object using a mesh and a material (also called actors and
//! entities by other game engines).  
//! @class MeshObject
//! @brief Displays a solid object.
class MeshObject : public SceneComponent {
public:    
    //! Destroys the mesh object
    virtual ~MeshObject();

    //! Returns the material used to render this object.
    virtual const std::string& material_name() const=0;

    //! Returns the name of the mesh associated with this object.
    virtual const std::string& mesh_name() const=0;
    
    //! Returns the mesh object attached to this scene component
    virtual Material* material() const=0;
    
    //! Returns the mesh attached to this scene component
    virtual Mesh* mesh() const=0;
    
    //! Returns true if the mesh is used in collision detection.  Note that
    //! the mesh must be attached to a node with a RigidBody also attached.
    virtual CollisionMode collision_mode() const=0;

    //! Sets the material used to render this object.  See the documentation
    //! about materials for more information.
    //! @param s the name of the material.
    virtual void material_name(const std::string& s)=0;

    //! Sets the name of the mesh to be used.
    //! @param s the name of the mesh
    virtual void mesh_name(const std::string& s)=0;
    
    //! Sets the material for this mesh.
    //! @param material the material
    virtual void material(Material* material)=0;
    
    //! Sets the mesh attached to this object.
    //! @param mesh the new mesh
    virtual void mesh(Mesh* mesh)=0;
    
    //! Sets whether or not this object will be used in collision detection.
    //! @param cm the current collision mode.  The values of the CollisionMode
    //! enum are as follows: CM_BOX, use bounding-box collision; CM_SPHERE, use
    //! bounding-sphere collision; and CM_MESH, use triangle mesh collision.
    //! If triangle mesh collision is not available, CM_MESH will default to
    //! CM_SPHERE.
    virtual void collision_mode(CollisionMode cm) const=0;
};

}
