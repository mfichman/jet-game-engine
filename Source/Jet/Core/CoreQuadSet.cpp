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
    
#include <Jet/Core/CoreQuadSet.hpp>

using namespace Jet;

void CoreQuadSet::quad(size_t index, const Quad& quad) {

	// Make room for the quads that we are adding
	if (vertex_.size() < (index + 1) * 4) {
		vertex_.resize((index + 1) * 4);
	}

	const Vector& up = quad.up;
	Vector right = up.cross(quad.normal);
	Vertex v;

	v.position = right * -quad.width/2 + up * quad.height/2;
	v.normal = quad.normal;
	v.texcoord = Texcoord(0.0f, 1.0f);
	vertex_[index*4] = v;

	v.position = right * quad.width/2 + up * quad.height/2;
	v.normal = quad.normal;
	v.texcoord = Texcoord(1.0f, 1.0f);
	vertex_[index*4+1] = v;

	v.position = right * quad.width/2 + up * -quad.height/2;
	v.normal = quad.normal;
	v.texcoord = Texcoord(1.0f, 0.0f);
	vertex_[index*4+2] = v;

	v.position = right * -quad.width/2 + up * -quad.height/2;
	v.normal = quad.normal;
	v.texcoord = Texcoord(0.0f, 0.0f);
	vertex_[index*4+3] = v;
}