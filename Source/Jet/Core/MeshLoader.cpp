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

#include <Jet/Core/MeshLoader.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
 
using namespace Jet;
using namespace std;
using namespace boost;
 
Core::MeshLoader::MeshLoader(Mesh* mesh, const std::string& file) :
    mesh_(mesh),
    in_(file.c_str()),
    name_(file) {
    
	// Read in the whole file, one commmand at a time.  Each line starts
	// with a command word or "#" if the line is a comment.
    while (in_.good() && !in_.eof()) {
		string command;
        in_ >> command;

		if (!in_.good()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            string line;
            getline(in_, line);
        } else if (command == "v") {
			Vector position;
			in_ >> position;
			position_.push_back(position);
		} else if (command == "vt") {
			Texcoord texcoord;
			in_ >> texcoord;
			texcoord.v = 1 - texcoord.v;
			//texcoord.u = 1 - texcoord.u;
			texcoord_.push_back(texcoord);
		} else if (command == "vn") {
			Vector normal;
			in_ >> normal;
			normal_.push_back(normal);
		} else if (command == "f") {
			read_face();
		}
    }
}

void Core::MeshLoader::read_face() {
    Vertex face[3];
    
    // Read in the face.  The format looks like this:
    // f position/texcoord/normal
    for (int i = 0; i < 3; i++) {
        string line, i1, i2, i3;
        in_ >> line;
        istringstream ss(line);
        getline(ss, i1, '/');
        getline(ss, i2, '/');
        getline(ss, i3, '/');
        
        if (ss.fail()) {
            throw runtime_error("Invalid OBJ file: " + name_);
        }
        
        if (!i1.empty()) {
            size_t j = lexical_cast<size_t>(i1) - 1;
            if (j >= position_.size()) {
                throw runtime_error("Invalid OBJ file: " + name_);
            } else {
                face[i].position = position_[j];
            }
        }
        if (!i2.empty()) {
            size_t j = lexical_cast<size_t>(i2) - 1;
            if (j >= texcoord_.size()) {
                throw runtime_error("Invalid OBJ file: " + name_);
            } else {
                face[i].texcoord = texcoord_[j];
            }
        }
        if (!i3.empty()) {
            size_t j = lexical_cast<size_t>(i3) - 1;
            if (j > normal_.size()) {
                throw runtime_error("Invalid OBJ file: " + name_);
            } else {
                face[i].normal = normal_[j];
            }
        }
    }
    
    insert_face(face);
    
}

void Core::MeshLoader::insert_face(Vertex face[3]) {
	
	// Add vertices to the buffer
	for (int i = 0; i < 3; i++) {
		// Calculate tangent
		compute_tangent(face, i);
		map<Vertex, uint32_t>::iterator j = cache_.find(face[i]);
        size_t index = 0;
		if (j == cache_.end()) {
			// Vertex was not found, so push a new index and vertex
			// into the list.  Add the vertex to the cache
			index = mesh_->vertex_count();
			cache_.insert(make_pair(face[i], index));
		} else {
			// Vertex was found, so use the existing index.  Update
			// the tangent vector so that we can average the tangent
			// vector later
            index = j->second;
            face[i].tangent += mesh_->vertex(index).tangent;
		}
        mesh_->vertex(index, face[i]);
        mesh_->index(mesh_->index_count(), index);
	}
}

void Core::MeshLoader::compute_tangent(Vertex face[3], size_t j) {
	// Calculate binormals
	Vertex& p0 = face[j];
	Vertex& p1 = face[(j+1)%3];
	Vertex& p2 = face[(j+2)%3];
	
    Vector d1 = p1.position - p0.position;
    Vector d2 = p2.position - p1.position;
    const Texcoord& tex0 = p0.texcoord;
    const Texcoord& tex1 = p1.texcoord;
    const Texcoord& tex2 = p2.texcoord;
    real_t s1 = tex1.u - tex0.u;
    real_t t1 = tex1.v - tex0.v;
    real_t s2 = tex2.u - tex0.u;
    real_t t2 = tex2.v - tex0.v;

    real_t a = 1/(s1*t2 - s2*t1);
    p0.tangent = p0.tangent + ((d1*t2 - d2*t1)*a).unit();
	p1.tangent = p1.tangent + ((d1*t2 - d2*t1)*a).unit();
	p2.tangent = p2.tangent + ((d1*t2 - d2*t1)*a).unit();
}