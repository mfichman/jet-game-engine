#include <Jet/Root.hpp>
#include <Jet/Registry.hpp>
#include <iostream>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;

#include <direct.h>

ostream& operator<<(ostream& os, const Vector& vec) {
    return os << "(" << vec.x() << "," << vec.y() << "," << vec.z() << ")";
}

int main(int argc, char** argv) {
	
	_chdir("..");

	try {
		Root::Ptr root = Root::make();
		
		Options::Ptr options = root->options();
		VideoMode mode = options->videoMode();
		mode.width_ = 600;
		mode.height_ = 600;
		mode.windowMode_ = VideoMode::modeWindowed;
		//options->videoMode(mode);

		Loader::Ptr loader = root->loader();

#ifdef WINDOWS
		loader->moduleNew("Binary\\Physics_ODE.dll");
		loader->moduleNew("Binary\\Audio_FMOD.dll");
		loader->moduleNew("Binary\\Renderer_D3D9.dll");
#endif
#ifdef LINUX
		loader->moduleNew("Binary\\libPhysics_ODE.so");
		loader->moduleNew("Binary\\libAudio_FMOD.so");
		loader->moduleNew("Binary\\libRenderer_OpenGL.so");
#endif
#ifdef DARWIN
        loader->moduleNew("Binary\\libPhysics_ODE.dylib");
        loader->moduleNew("Binary\\libAudio_FMOD.so");
		loader->moduleNew("Binary\\libRenderer_OpenGL.so");
#endif

        loader->textureNew("test1.dds");
        loader->textureNew("test2.dds");
        loader->textureNew("test3.dds");
        loader->textureNew("test4.dds");
        loader->textureNew("test5.dds");
        loader->textureNew("test6.dds");
        loader->textureNew("test7.dds");
        loader->textureNew("test8.dds");
        loader->textureNew("test9.dds");
        loader->videoLoadStatus(Loader::statusLoading);

		Mesh::Ptr mesh = loader->meshNew("test.x");
		mesh->boundingSphere(Sphere(Vector(0.0f, 0.0f, 0.0f), 10.0f));
		
        Model::Ptr model = root->modelNew();
		model->mesh(mesh);

		Body::Ptr body = root->bodyNew();
		body->mass(10.0f);
		body->radius(10.0f);
		body->linearVelocity(Vector(10.0f, 0.0f, 0.0f));
		body->attachmentAdd(Attachment(model, Vector(1.0f, 0.0f, 0.0f), Quaternion()));
		
		//loader->moduleDel("Binary\\Renderer_D3D9.dll");
		options->videoMode(mode);
		
		do {
		    root->stepInc(0.0f, Step::typeRender);
		    cout << body->position() << endl;
		    cout << model->position() << endl;
		} while (cin.get() != 'q');

	

	} catch (std::exception& e) {
		cout << e.what() << endl;
	}



	//Object::Ptr o = Object::make();
	
	//Class::Ptr c = r->type(o);
	//c->mutator("position")(o, SimpleSource());
	//c->accessor("position")(o, SimpleSink());
}
