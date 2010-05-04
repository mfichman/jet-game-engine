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

#include <Jet/Core/FractureObject.hpp>
#include <Jet/Core/RigidBody.hpp>
#include <Jet/Plane.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

using namespace Jet;
using namespace std;

Core::FractureObject::~FractureObject() {
}

void Core::FractureObject::fracture(const Plane& plane) {
    Mesh* mesh = mesh_object_->mesh();
    
    if (fracture_count_ <= 0) {
        return;
    } else if (mesh) {
        mesh->state(SYNCED);
        
        // Here, we will begin the fracture.  The object must have a mesh
        // attached, or else we won't be able to split the object.
        fracture_count_--;
        fracture_indices(plane);
    }
}

void Core::FractureObject::fracture_indices(const Plane& plane) {
    Mesh* mesh = mesh_object_->mesh();
    
    // Create two new anonymous meshes for the pieces that splinter off
    MeshPtr mesh1 = new Mesh(engine_, mesh);
    MeshPtr mesh2 = new Mesh(engine_, mesh);
    
    BoundingBox b1;
    BoundingBox b2;

    // Loop through each face, and divide them using the plane.  Some vertices
    // will go to one mesh, some will go to the other
    for (size_t i = 2; i < mesh->index_count(); i += 3) {
        uint32_t i0 = mesh->index(i-2);
        uint32_t i1 = mesh->index(i-1);
        uint32_t i2 = mesh->index(i-0);
        const Vector& v0 = mesh->vertex(i0).position;
        const Vector& v1 = mesh->vertex(i1).position;
        const Vector& v2 = mesh->vertex(i2).position;
        real_t d0 = plane.distance(v0);
        real_t d1 = plane.distance(v1);
        real_t d2 = plane.distance(v2);
        
        
        if (d0 <= 0 && d1 <= 0 && d2 <= 0) {
            // If all the vertices are on the lower half-space created by the
            // plane, then push the indices to the first buffer
            mesh1->index(mesh1->index_count(), i0);
            mesh1->index(mesh1->index_count(), i1);
            mesh1->index(mesh1->index_count(), i2);
            b1.point(v0);
            b1.point(v1);
            b1.point(v2);
        } else  {//if (d0 >= 0 && d1 >= 0 && d2 >= 0) {
            // If all the vertices are on the upper half-space created by the
            // plane, then push the indices to the second buffer
            mesh2->index(mesh2->index_count(), i0);
            mesh2->index(mesh2->index_count(), i1);
            mesh2->index(mesh2->index_count(), i2);
            b2.point(v0);
            b2.point(v1);
            b2.point(v2);
        }
    }

    // Get the volume of bounding box to decide which half of the mesh is
    // bigger.  The bigger half stays with the current fracture object and the
    // smaller half goes with the new fracture object.
    real_t v1 = b1.volume();
    real_t v2 = b2.volume();

    // Sets the mass of the rigid bodies
    real_t mass = parent_->rigid_body()->mass();
    real_t m1 = v1 * mass / (v1 + v2);
    real_t m2 = v2 * mass / (v1 + v2);
    
    // Swap the mesh halves if v1 is smaller than v2 (index1 should always
    // have the largest mesh)
    if (v1 < v2) {
        swap(b1, b2);
        swap(m1, m2);
        swap(v1, v2);
        swap(mesh1, mesh2);
    }
    
    if (mesh2->index_count() > 120) {
        // Create a new fracture object that is an approximate "clone" of this
        // one.  Material, mesh, and other attributes should be copied, but
        // the indices will be different.
        FractureObject* clone = create_clone();
        clone->mesh_object_->mesh(mesh2.get());   
        clone->parent()->rigid_body()->mass(m2);
        clone->parent()->rigid_body()->linear_velocity(parent_->rigid_body()->linear_velocity());
		parent_->fracture(clone->parent());
    }
    
    // Initialize the index buffer for this object
    mesh_object_->mesh(mesh1.get());
    RigidBody* rigid_body = static_cast<RigidBody*>(parent_->rigid_body());
    rigid_body->mass(m1);
    rigid_body->update_collision_shapes();
}

Core::FractureObject* Core::FractureObject::create_clone() {
    static char blah = 'a';
    
    // Create a new node and fracture object
    Node* node = static_cast<Node*>(parent_->parent()->node());
    FractureObject* clone = static_cast<FractureObject*>(node->fracture_object());
  
    // TODO: HACK HACK HACK.  Anonymous nodes needed
    blah++;

    clone->material(material());
    clone->fracture_count(fracture_count());
    clone->cast_shadows(cast_shadows());
    clone->seal_fractures(seal_fractures());
    
    node->position(parent_->position());
    node->rotation(parent_->rotation());
    
    return clone;
}