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

#include <Jet/Core/OBJLoader.hpp>
#include <Jet/Engine.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Texcoord.hpp>
#include <Jet/Vertex.hpp>
#include <Jet/Mesh.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace Jet::Core;
using namespace Jet;
using namespace std;
using namespace boost;


OBJLoader::OBJLoader(Engine* engine) :
    engine_(engine) {
    
    command_["mtllib"] = &OBJLoader::mtllib;
    command_["usemtl"] = &OBJLoader::usemtl;
    command_["v"] = &OBJLoader::vertex;
    command_["vt"] = &OBJLoader::texcoord;
    command_["vn"] = &OBJLoader::normal;
    command_["f"] = &OBJLoader::face;
}

void OBJLoader::mtllib(istream& in) {
    string mttlib;
    in >> mttlib;
    // load material TODO
	engine_->resource(mttlib);
}

void OBJLoader::usemtl(istream& in) {
    in >> material_;
}

void OBJLoader::vertex(istream& in) {
    Vector position;
    in >> position;
    position_.push_back(position);
}

void OBJLoader::texcoord(istream& in) {
    // Read in a texture coordinate
    Texcoord texcoord;
    in >> texcoord;
    texcoord_.push_back(texcoord);
}

void OBJLoader::normal(istream& in) {
    Vector normal;
    in >> normal;
    normal_.push_back(normal);
}

void OBJLoader::face(istream& in) {    
    // Read in a face for the mesh
    Vertex face[3];
    for (int i = 0; i < 3; i++) {
		string line;
		in >> line;

		istringstream ss(line);
        string i1, i2, i3;
        getline(ss, i1, '/');
        getline(ss, i2, '/');
        getline(ss, i3, ' ');
        
        if (!i1.empty()) {
            face[i].position = position_[lexical_cast<size_t>(i1)-1];
		} else {
			throw runtime_error("Malformed file");
		}
        if (!i2.empty()) {
            face[i].texcoord = texcoord_[lexical_cast<size_t>(i2)-1];
        }
        if (!i3.empty()) {
            face[i].normal = normal_[lexical_cast<size_t>(i3)-1];
        }
    }

    // Add vertices to the buffer
    for (int j = 0; j < 3; j++) {  
        // Calculate binormals          
        binormal(face, j);
        
        map<Vertex, uint32_t>::iterator i = vertex_.find(face[j]);
        if (i == vertex_.end()) {
            // Vertex was not found, so push a new index and
            // vertex into the list
            index_.push_back(cur_index_);
            vertex_.insert(make_pair(face[j], cur_index_++));
            
        } else {
            // Vertex was found, so use the existing index
            index_.push_back(i->second);

        }
    }
    
    // Read in the rest of the line
    string line;
    getline(in, line);
}

void OBJLoader::binormal(Vertex face[3], size_t j) {
    // Calculate binormals
    Vector d1 = face[(j+1)%3].position - face[j].position;
    Vector d2 = face[(j+2)%3].position - face[j].position;
    const Texcoord& tex0 = face[j].texcoord;
    const Texcoord& tex1 = face[(j+1)%3].texcoord;
    const Texcoord& tex2 = face[(j+2)%3].texcoord;
    real_t s1 = tex1.u - tex0.u;
    real_t t1 = tex1.v - tex0.v;
    real_t s2 = tex2.u - tex0.u;
    real_t t2 = tex2.v - tex0.v;

    real_t a = 1/(s1*t2 - s2*t1);
    // vertex.tangent = ((d1*t2 - d2*t1)*a).unit();
    face[j].binormal = ((d2*s1 - d1*s2)*a).unit();
}

// .OBJ (Wavefront) and .MTL loader.  Loads linked materials specified in the
// .OBJ file.  
void OBJLoader::resource(const std::string& file) {
    static const std::string& ext = ".obj";
    if ((file.length() - file.rfind(ext)) != ext.length()) {
        throw runtime_error("Invalid file extension");
    }

    ifstream in(file.c_str());

    // Collection of vertices
    string command;
    cur_index_ = 0;
    
    while (in.good() && !in.eof()) {
        in >> command;

		if (!in.good()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            string line;
            getline(in, line);
        } else {
            map<string, void (OBJLoader::*)(istream&)>::iterator i = command_.find(command);
            if (i != command_.end()) {
                ((this)->*(i->second))(in);
			} else {
				string line;
				getline(in, line);
			}
        }

    }
    
    std::string name = file.substr(file.rfind("/") + 1, string::npos);

    // Build the mesh data
    MeshPtr mesh(engine_->mesh(name, false));
    for (map<Vertex, uint32_t>::iterator i = vertex_.begin(); i != vertex_.end(); i++) {
        mesh->vertex(i->second, i->first);
    }
    for (size_t i = 0; i < index_.size(); i++) {
        mesh->index(i, index_[i]);
    }
    mesh->loaded(true);

    //! Build a component that is a template meshobject
    ComponentPtr component(engine_->component(name));
    component->value("mesh", name);
	component->value("renderable", true);
    component->component("material", engine_->component(material_));

    position_.clear();
    normal_.clear();
    texcoord_.clear();
    vertex_.clear();
    index_.clear();   
}

