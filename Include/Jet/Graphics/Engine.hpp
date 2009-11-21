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
	friend class Root;
	typedef shared_ptr<Engine> Ptr;
	
	typedef map<string, Texture* >   TextureContainer;
	typedef map<string, Cubemap* >   CubemapContainer;
	typedef map<string, Mesh* >      MeshContainer;
	typedef map<string, Shader* >    ShaderContainer;
	typedef vector<Quad* >           QuadContainer;
	typedef vector<Model* >          ModelContainer;
	typedef vector<TextBox* >        TextBoxContainer;
	
	typedef Iterator<TextureContainer>          TextureItr;
	typedef Iterator<CubemapContainer>          CubemapItr;
	typedef Iterator<MeshContainer>             MeshItr;
	typedef Iterator<ShaderContainer>           ShaderItr;
	
	typedef vector<Listener*>::iterator         ListenerItr;
	
	enum RenderQuality { hdr, normal };
	enum RenderOrder { up, down };
	enum MultisampleQuality { ms0, ms1, ms2, ms3, ms4, ms5 };
	enum DisplayMode { windowed, fullscreen };

	shared_ptr<Texture>	    textureCreate(const string& name);
	shared_ptr<Cubemap>     cubemapCreate(const string& name);
	shared_ptr<Mesh>        meshCreate(const string& name);
	shared_ptr<Shader> 	    shaderCreate(const string& name);
	shared_ptr<Quad>        quadCreate();
	shared_ptr<Model>       modelCreate();
	shared_ptr<TextBox>     textBoxCreate();
	
	void                    textureDestroy(Texture* t);
	void                    cubemapDestroy(Cubemap* c);
	void                    meshDestroy(Mesh* m);
	void                    shaderDestroy(Shader* s);
	void                    quadDestroy(Quad* s);
	void                    modelDestroy(Model* m);
	void                    textBoxDestroy(TextBox* t);
	
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
	vector<Listener*>       listener_;
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
    virtual void onTextureCreate(shared_ptr<Texture> t) {}
    virtual void onCubemapCreate(shared_ptr<Cubemap> c) {}
    virtual void onShaderCreate(shared_ptr<Shader> s) {}
    virtual void onMeshCreate(shared_ptr<Mesh> m) {}
    virtual void onQuadCreate(shared_ptr<Quad> s) {}
    virtual void onModelCreate(shared_ptr<Model> s) {}
    virtual void onTextBoxCreate(shared_ptr<TextBox> t) {}
    
    virtual void onTextureDestroy(Texture* t) {}
    virtual void onCubemapDestroy(Cubemap* c) {}
    virtual void onShaderDestroy(Shader* s) {}
    virtual void onMeshDestroy(Mesh* m) {}
    virtual void onQuadDestroy(Quad* s) {}
    virtual void onModelDestroy(Model* s) {}
    virtual void onTextBoxDestroy(TextBox* t) {}
    
    virtual void onOptions(shared_ptr<Options> o) {}
};

}}
