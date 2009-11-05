/*
 * Copyright (c) 2008 Matt Fichman
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

#include <Graphics/Resources.hpp>
#include <Graphics/Quad.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Overlay.hpp>
#include <Graphics/Renderable.hpp>
#include <Graphics/Geometry.hpp>
#include <Graphics/TextBox.hpp>
#include <Graphics/Camera.hpp>
#include <map>

namespace Jet { class Root; }

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;

class Engine {
public:
	class Listener;
	class Options;
	friend class Root;
	typedef map<string, Texture::Ptr>::iterator TextureItr;
	typedef map<string, Cubemap::Ptr>::iterator CubemapItr;
	typedef map<string, Mesh::Ptr>::iterator MeshItr;
	typedef map<string, Shader::Ptr>::iterator ShaderItr;
	typedef shared_ptr<Options> OptionsPtr;
	typedef shared_ptr<Engine> Ptr;
	
	enum RenderQuality { hdr, normal };
	enum RenderOrder { up, down };
	enum MultisampleQuality { ms0, ms1, ms2, ms3, ms4, ms5 };
	enum DisplayMode { windowed, fullscreen };

	Texture::Ptr 	    texture(const string& name);
	Cubemap::Ptr 	    cubemap(const string& name);
	Mesh::Ptr 	        mesh(const string& name);
	Shader::Ptr 	    shader(const string& name);
	Quad::Ptr 	        quad();
	Model::Ptr	        model();
		
	TextureItr          textureItr();
	CubemapItr          cubemapItr();
	MeshItr             meshItr();
    ShaderItr           shaderItr();
    
    OptionsPtr          options(const string& name);
    void                options(OptionsPtr o); 
    
private:
	Engine() {}
};

class Engine::Options {
public:
    RenderOrder         objectOrder() const;
    void                objectOrder(RenderOrder o);
    RenderQuality       renderQuality() const;
    void                renderQuality(RenderQuality q);
    MultisampleQuality  multisampleQuality() const;
    void                multisampleQuality(MultisampleQuality q);
    DisplayMode         displayMode() const;
    void                displayMode(DisplayMode m);
    ScreenCoord         resolution() const;
    void                resolution(ScreenCoord c);
};

}}
