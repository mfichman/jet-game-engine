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

#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <Jet/Iterator.hpp>
#include <Jet/Graphics/Texture.hpp>
#include <Jet/Graphics/Cubemap.hpp>
#include <Jet/Graphics/Mesh.hpp>
#include <Jet/Graphics/Shader.hpp>
#include <Jet/Graphics/Quad.hpp>
#include <Jet/Graphics/Model.hpp>
#include <Jet/Graphics/TextBox.hpp>
#include <map>
#include <vector>

namespace Jet { class Root; }

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;

class Texture;
class Cubemap;
class Mesh;
class Shader;
class Quad;
class Model;
class TextBox;

class Engine : public Interface {
public:
	class Listener;
	class Options;
	friend class Jet::Root;
	typedef intrusive_ptr<Engine> Ptr;
	
	typedef map<string, Texture::Ptr>   TextureContainer;
	typedef map<string, Cubemap::Ptr>   CubemapContainer;
	typedef map<string, Mesh::Ptr>      MeshContainer;
	typedef map<string, Shader::Ptr>    ShaderContainer;
	typedef vector<Quad::Ptr>           QuadContainer;
	typedef vector<Model::Ptr>          ModelContainer;
	typedef vector<TextBox::Ptr>        TextBoxContainer;
	
	typedef Iterator<TextureContainer>          TextureItr;
	typedef Iterator<CubemapContainer>          CubemapItr;
	typedef Iterator<MeshContainer>             MeshItr;
	typedef Iterator<ShaderContainer>           ShaderItr;
	
	typedef vector<Listener*>::iterator         ListenerItr;
	
	enum RenderQuality { hdr, normal };
	enum RenderOrder { up, down };
	enum MultisampleQuality { ms0, ms1, ms2, ms3, ms4, ms5 };
	enum DisplayMode { windowed, fullscreen };

	Texture::Ptr	        textureCreate(const string& name);
	Cubemap::Ptr            cubemapCreate(const string& name);
	Mesh::Ptr               meshCreate(const string& name);
	Shader::Ptr 	        shaderCreate(const string& name);
	Quad::Ptr               quadCreate();
	Model::Ptr              modelCreate();
	TextBox::Ptr            textBoxCreate();
	
	void                    listener(Listener* listener);
		
	TextureItr              textureItr() { return TextureItr(texture_); }
	CubemapItr              cubemapItr() { return CubemapItr(cubemap_); }
	MeshItr                 meshItr() { return MeshItr(mesh_); }
    ShaderItr               shaderItr() { return ShaderItr(shader_); }
    
    shared_ptr<Options>     options(const string& name);
    void                    options(shared_ptr<Options> o); 
    
private:
	Engine() {}
	
	TextureContainer        texture_;
	CubemapContainer        cubemap_;
	MeshContainer           mesh_;
	ShaderContainer         shader_;
	QuadContainer           quad_;
	ModelContainer          model_;
	TextBoxContainer        textBox_;
	vector<Listener*> listener_;
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

class Engine::Listener {
public:
    virtual ~Listener() {}
    virtual void onTextureCreate(intrusive_ptr<Texture> t) {}
    virtual void onCubemapCreate(intrusive_ptr<Cubemap> c) {}
    virtual void onShaderCreate(intrusive_ptr<Shader> s) {}
    virtual void onMeshCreate(intrusive_ptr<Mesh> m) {}
    virtual void onQuadCreate(intrusive_ptr<Quad> s) {}
    virtual void onModelCreate(intrusive_ptr<Model> s) {}
    virtual void onTextBoxCreate(intrusive_ptr<TextBox> t) {}
    
    virtual void onOptions(shared_ptr<Options> o) {}
};

}}
