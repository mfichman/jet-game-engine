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
    if (ibuffer_) {
        glDeleteBuffers(1, &ibuffer_);
    }
}

void Core::FractureObject::fracture(const Plane& plane) {
    if (fracture_count_ <= 0) {
        return;
    } else if (mesh_) {
        mesh_->state(SYNCED);
        
        // Here, we will begin the fracture.  The object must have a mesh
        // attached, or else we won't be able to split the object.
        fracture_count_--;
        
        if (ibuffer_) {
            //fracture_from_index_buffer(plane);
            fracture_indices(plane, &index_.front(), index_.size());
        } else {
            //fracture_from_mesh(plane);
            fracture_indices(plane, mesh_->index_data(), mesh_->index_count());
        }
    }
}

void Core::FractureObject::fracture_indices(const Plane& plane, const uint32_t* indices, size_t count) {
    vector<uint32_t> index1;
    vector<uint32_t> index2;
    BoundingBox b1;
    BoundingBox b2;

    // Loop through each face, and divide them using the plane.  Some vertices
    // will go to one mesh, some will go to the other
    for (size_t i = 2; i < count; i += 3) {
        uint32_t i0 = indices[i-2];
        uint32_t i1 = indices[i-1];
        uint32_t i2 = indices[i-0];
        const Vector& v0 = mesh_->vertex(i0).position;
        const Vector& v1 = mesh_->vertex(i1).position;
        const Vector& v2 = mesh_->vertex(i2).position;
        real_t d0 = plane.distance(v0);
        real_t d1 = plane.distance(v1);
        real_t d2 = plane.distance(v2);
        
        
        if (d0 <= 0 && d1 <= 0 && d2 <= 0) {
            // If all the vertices are on the lower half-space created by the
            // plane, then push the indices to the first buffer
            index1.push_back(i0);
            index1.push_back(i1);
            index1.push_back(i2);
            b1.point(v0);
            b1.point(v1);
            b1.point(v2);
        } else  {//if (d0 >= 0 && d1 >= 0 && d2 >= 0) {
            // If all the vertices are on the upper half-space created by the
            // plane, then push the indices to the second buffer
            index2.push_back(i0);
            index2.push_back(i1);
            index2.push_back(i2);
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
        swap(index1, index2);
    }
    
    if (index2.size() > 120) {
        // Create a new fracture object that is an approximate "clone" of this
        // one.  Material, mesh, and other attributes should be copied, but
        // the indices will be different.
        FractureObject* clone = create_clone();
        swap(index2, clone->index_);    

        clone->bounding_box_ = b2;
        clone->init_index_buffer();
        clone->parent()->rigid_body()->mass(m2);
        clone->parent()->rigid_body()->linear_velocity(parent_->rigid_body()->linear_velocity());
		parent_->fracture(clone->parent());
    }
    
    // Initialize the index buffer for this object
    swap(index1, index_);
    bounding_box_ = b1;
    init_index_buffer();
    parent_->rigid_body()->mass(m1);
    
    RigidBody* rigid_body = static_cast<RigidBody*>(parent_->rigid_body());
    rigid_body->update_collision_shapes();

	cout << "FRACTURE" << endl;
}

Core::FractureObject* Core::FractureObject::create_clone() {
    static char blah = 'a';
    
    // Create a new node and fracture object
    Node* node = static_cast<Node*>(parent_->parent()->node("fracture" + blah));
    FractureObject* clone = static_cast<FractureObject*>(node->fracture_object("fracture"));
  
    blah++;
  
    clone->mesh_ = mesh_;
    clone->material_ = material_;
    clone->fracture_count_ = fracture_count_;
    clone->cast_shadows_ = cast_shadows_;
    clone->seal_fractures_ = seal_fractures_;
    
    node->position(parent_->position());
    node->rotation(parent_->rotation());
    
    return clone;
}

void Core::FractureObject::init_index_buffer() {
    if (ibuffer_) {
        glDeleteBuffers(1, &ibuffer_);
    }
    
    glGenBuffers(1, &ibuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_.size()*sizeof(uint32_t), &index_.front(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    update_collision_shape();
}

void Core::FractureObject::update_collision_shape() {
    // Create a triangle array using the data loaded from the disk
	btIndexedMesh mesh;
	mesh.m_numTriangles = index_.size()/3;
	mesh.m_triangleIndexBase = (uint8_t*)&index_.front();
	mesh.m_triangleIndexStride = 3*sizeof(uint32_t);
	mesh.m_numVertices = mesh_->vertex_count();
	mesh.m_vertexBase = (uint8_t*)mesh_->vertex_data();
	mesh.m_vertexStride = sizeof(Vertex);
	btTriangleIndexVertexArray vertex_array;
	vertex_array.addIndexedMesh(mesh);
	
	//! Create a temporary shape to hold the vertice;
	btConvexTriangleMeshShape temp_shape(&vertex_array);
	btShapeHull shape_hull(&temp_shape);
	btScalar margin = temp_shape.getMargin();
	shape_hull.buildHull(margin);
	shape_ = btConvexHullShape();
	temp_shape.setUserPointer(&shape_hull);
    
    Vector origin = bounding_box_.origin();
    btVector3 center(origin.x, origin.y, origin.z);

	// Awesome! Create a hull using the hull vertices.
	for (int i = 0; i < shape_hull.numVertices(); i++) {
		shape_.addPoint(shape_hull.getVertexPointer()[i] - center);
	}
}

void Core::FractureObject::render(Shader* shader) {
    if (ibuffer_) {
        mesh_->render(shader, ibuffer_, index_.size());
    } else {
        mesh_->render(shader);
    }
}