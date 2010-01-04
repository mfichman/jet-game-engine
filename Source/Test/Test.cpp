#include <Jet/Root.hpp>
#include <Jet/Registry.hpp>
#include <iostream>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;

#include <direct.h>

int main(int argc, char** argv) {
	
	_chdir("..");

	try {

		Root::Ptr root = Root::make();

		Loader::Ptr loader = root->loader();
		loader->moduleNew("Binary\\ODE.dll");
		loader->moduleNew("Binary\\FMOD.dll");
		//loader->moduleNew("Binary\\libODE.so");
		//loader->moduleNew("Binary\\libFMOD.so");

		Sphere sphere(Vector(0.0f, 0.0f, 0.0f), 10.0f);
		Mesh::Ptr mesh = loader->meshNew("test.x");
		mesh->boundingSphere(sphere);

		Body::Ptr body = root->bodyNew();
		body->mass(10.0f);

		Model::Ptr model = root->modelNew();
		model->mesh(mesh);
		model->position(Vector(1.0f, 0.0, 0.0f));		

	} catch (std::exception& e) {
		cout << e.what() << endl;
	}



	//Object::Ptr o = Object::make();
	
	//Class::Ptr c = r->type(o);
	//c->mutator("position")(o, SimpleSource());
	//c->accessor("position")(o, SimpleSink());
}
