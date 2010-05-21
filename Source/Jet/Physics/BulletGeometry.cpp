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

#include <Jet/Physics/BulletGeometry.hpp>
#include <Jet/Resources/Mesh.hpp>
#include <Jet/Types/Vertex.hpp>

using namespace Jet;
using namespace std;
using namespace boost;

BulletGeometry::~BulletGeometry() {
    
    // Free the buffer from memory
    state(UNLOADED);
}

void BulletGeometry::state(ResourceState state) {
    if (state == state_) {
        return;
    }

    if (LOADED == state) {
        update_collision_shape();
    }
    
    state_ = state;
}

void BulletGeometry::update_collision_shape() {
    // Create a triangle array using the data loaded from the disk
    MeshPtr mesh = BulletGeometry::mesh();
	btIndexedMesh imesh;
	imesh.m_numTriangles = mesh->index_count()/3;
	imesh.m_triangleIndexBase = (uint8_t*)mesh->index_data();
	imesh.m_triangleIndexStride = 3*sizeof(uint32_t);
	imesh.m_numVertices = mesh->vertex_count();
	imesh.m_vertexBase = (uint8_t*)mesh->vertex_data();
	imesh.m_vertexStride = sizeof(Vertex);
	btTriangleIndexVertexArray vertex_array;
	vertex_array.addIndexedMesh(imesh);
	
	//! Create a temporary shape to hold the vertice;
	btConvexTriangleMeshShape temp_shape(&vertex_array);
	btShapeHull shape_hull(&temp_shape);
	btScalar margin = temp_shape.getMargin();
	shape_hull.buildHull(margin);
	shape_ = btConvexHullShape();
	temp_shape.setUserPointer(&shape_hull);

	// Awesome! Create a hull using the hull vertices.
	for (int i = 0; i < shape_hull.numVertices(); i++) {
		shape_.addPoint(shape_hull.getVertexPointer()[i]);
	}
}