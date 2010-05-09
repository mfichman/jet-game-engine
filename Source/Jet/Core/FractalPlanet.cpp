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

#include <Jet/Core/FractalPlanet.hpp>

using namespace Jet;
using namespace std;

#define PI 3.14159f


void Core::FractalPlanet::generate_mesh() {
    mesh_->vertex_count(0);
	mesh_->index_count(0);
    mesh_->state(LOADED);
    
    float radius = 10.0f;
    float dphi = PI/ring_count_;
    float dtheta = 2.0f*PI/division_count_;
    
    for (float phi = 0.0f; phi < PI; phi += dphi) {
        for (float theta = 0.0f; theta < 2*PI; theta += dtheta) {
            Vertex quad[4];
            Vector p0, p1, p2, p3;
            p0.x = radius * sinf(phi) * cosf(theta);
            p0.z = radius * sinf(phi) * sinf(theta);
            p0.y = radius * cosf(phi);
            
            p1.x = radius * sinf(phi + dphi) * cosf(theta);
            p1.z = radius * sinf(phi + dphi) * sinf(theta);
            p1.y = radius * cosf(phi + dphi);
            
            p2.x = radius * sinf(phi + dphi) * cosf(theta + dtheta);
            p2.z = radius * sinf(phi + dphi) * sinf(theta + dtheta);
            p2.y = radius * cosf(phi + dphi);
            
            p3.x = radius * sinf(phi) * cosf(theta + dtheta);
            p3.z = radius * sinf(phi) * sinf(theta + dtheta);
            p3.y = radius * cosf(phi);
            

	/*for (float x = 1.0f; x < 5.0f; x++) {
		for (float y = 1.0f; y < 5.0f; y++) {
			Vector p0(x, y, 0);
			Vector p1(x+1, y, 0);
			Vector p2(x+1, y+1, 0);
			Vector p3(x, y+1, 0);
			Vertex quad[4];*/

            quad[0].position = p0;
            quad[1].position = p1;
            quad[2].position = p2;
            quad[3].position = p3;
            quad[0].texcoord = Texcoord(theta/(2*PI), phi/PI);
            quad[1].texcoord = Texcoord(theta/(2*PI), (phi+dphi)/PI);
            quad[2].texcoord = Texcoord((theta+dtheta)/(2*PI), (phi+dphi)/PI);
            quad[3].texcoord = Texcoord((theta+dtheta)/(2*PI), phi/PI);
            quad[0].normal = p0.unit();
            quad[1].normal = p1.unit();
            quad[2].normal = p2.unit();
            quad[3].normal = p3.unit();
            generate_quad(quad, 3);
        }
    }
    
    /*    float phi = rand_range(Range(0.0f, PI));
    float theta = rand_range(Range(0.0f, 2.0f*PI));
    float a = rand_range(width_);
    float b = rand_range(height_);
    float c = rand_range(depth_);
    p.init_position.x = a * sinf(phi) * cosf(theta);
    p.init_position.y = b * sinf(phi) * sinf(theta);
    p.init_position.z = c * cosf(phi);
    
    float speed = rand_range(emission_speed_);
    p.init_velocity.x = speed * sinf(phi) * cosf(theta);
    p.init_velocity.y = speed * sinf(phi) * sinf(theta);
    p.init_velocity.z = speed * cosf(phi);*/
    
    // TODO: Generate quads dynamically in a mesh shape
	recalculate_normals();

    mesh_->state(SYNCED);
}

void Core::FractalPlanet::generate_quad(Vertex quad[4], size_t level) {
    // The quad array is arranged like this:
    // top left, top right, bottom right, bottom left.
    
    const size_t size = (1 << level)+1; // # # vertices along one edge of quad
    const size_t vertex_offset = mesh_->vertex_count();
    const size_t vertex_count = (size)*(size);  // (2^level + 1)^2
    mesh_->vertex_count(mesh_->vertex_count() + vertex_count);
    
    // Generate the grid by interpolating between the corners of the quad.
    for (size_t row = 0; row < size; row++) {
        for (size_t col = 0; col < size; col++) {
            float fh = (float)col/(float)(size-1);
            float fv = (float)row/(float)(size-1);
            
            // Horizontal interpolation (line 1)
            Vertex vh1 = quad[0]*(1.0f-fh) + quad[1]*fh;
            Vertex vh2 = quad[3]*(1.0f-fh) + quad[2]*fh;
            
            // Vertical interpolation (line 2).  TODO: Normals (autogen?)
            Vertex v = vh1*(1.0f-fv) + vh2*fv;

            mesh_->vertex(vertex_offset + row*size + col, v);
        }
    }

	// Generate the height values of the fractal.  This adds
	// a distortion along the normal using the diamond-square
	// algorithm
	generate_fractal(vertex_offset, level);
    
    // Iterate through the grid to build the index array.
    for (size_t row = 0; row < (size-1); row++) {
        for (size_t col = 0; col < (size-1); col++) {
            mesh_->index(mesh_->index_count(), vertex_offset + row*size + col);
			mesh_->index(mesh_->index_count(), vertex_offset + row*size + (col+1));
			mesh_->index(mesh_->index_count(), vertex_offset + (row+1)*size + (col+1));

			mesh_->index(mesh_->index_count(), vertex_offset + row*size + col);
			mesh_->index(mesh_->index_count(), vertex_offset + (row+1)*size + (col+1));
			mesh_->index(mesh_->index_count(), vertex_offset + (row+1)*size + col);
		}
    }

}

void Core::FractalPlanet::generate_fractal(size_t vertex_offset, size_t level) {
	const int size = (1 << level)+1; // # vertices along one edge of quad
	const int vertex_count = (size)*(size);  // (2^level + 1)^2
	int stride = (1 << (level-1));
	float range = 0.1f;

	vector<float> noise(vertex_count);

	noise[0] = 0.1f;
	noise[size-1] = 0.1f;
	noise[size*(size-1)] = 0.1f;
	noise[size*(size-1) + (size-1)] = 0.1f;

	while (stride >= 1) {

		// Diamond part of the algorithm.  This averages square vertices together
		// in the center.
		for (int row = stride; row < size-1; row += stride*2) {
			for (int col = stride; col < size-1; col += stride*2) {
                if (col <= 0 || col >= size-2 || row <= 0 || row >= size-2) {
                    continue;
                }
                
				// Average the four corners surrounding the current diamond corner.
				float& value = noise[row*size + col];
				assert(value == 0.0f);
				value += noise[(row-stride)*size + (col-stride)];
				value += noise[(row+stride)*size + (col-stride)];
				value += noise[(row+stride)*size + (col+stride)];
				value += noise[(row-stride)*size + (col+stride)];
				value /= 4;

				// Add a random roughness, and distort along the normal
				value += range * ((float)rand()/(float)RAND_MAX*2.0f - 1.0f); 
				
				// Now distort the vertex
				Vertex& vertex = mesh_->vertex(vertex_offset + row*size + col);
				vertex.position += vertex.normal * value;
			}
		}
	
		// Square part of the algorithm.  This averages the edges.
		for (int row = 0; row < size; row += stride) {
			// If this is an even iteration of the loop, then start the column
			// traversal at the stride.  Otherwise, start at 0
			size_t offset = (row % (stride*2)) ? 0 : stride;
			for	(int col = offset; col < size; col += stride*2) {
                if (col <= 0 || col >= size-2 || row <= 0 || row >= size-2) {
                    continue;
                }
                
				int bottom = row+stride;
				int top = row-stride;

				int right = col+stride;
				int left = col-stride;

				// Get the vertex we're working on, and average it with the
				// surrounding vertices.  Do not wrap around the edges of 
				// the quad tile.
				float& value = noise[row*size + col];
				assert(value == 0.0f);
				float count = 0;
				if (top >= 0) {
					value += noise[top*size + col];
					count++;
				}
				if (bottom < size) {
					value += noise[bottom*size + col];
					count++;
				}
				if (left >= 0) {
					value += noise[row*size + left];
					count++;
				}
				if (right < size) {
					value += noise[row*size + right];
					count++;
				}
				value /= count;
				
				// Add a random roughness, and distort along the normal
				value += range * ((float)rand()/(float)RAND_MAX*2.0f - 1.0f); 

				// Now distort the vertex
				Vertex& vertex = mesh_->vertex(vertex_offset + row*size + col);
				vertex.position += vertex.normal * value;
			}
		}

		stride /= 2;
		range *= pow(2, -roughness_);
	}
}

void Core::FractalPlanet::recalculate_normals() {

	// Recalculate normals
	for (size_t i = 0; i < mesh_->vertex_count(); i++) {
		mesh_->vertex(i).normal = Vector();
	}

	for (size_t i = 2; i < mesh_->index_count(); i += 3) {
		Vertex& p0 = mesh_->vertex(mesh_->index(i-2));
		Vertex& p1 = mesh_->vertex(mesh_->index(i-1));
		Vertex& p2 = mesh_->vertex(mesh_->index(i-0));
		Vector v1 = p1.position - p0.position;
		Vector v2 = p2.position - p0.position;
		Vector normal = (v2.cross(v1)).unit();
		p0.normal += normal;
		p1.normal += normal;
		p2.normal += normal;
	}

	for (size_t i = 0; i < mesh_->vertex_count(); i++) {
		mesh_->vertex(i).normal = mesh_->vertex(i).normal.unit();
	}
}
