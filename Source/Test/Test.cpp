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

		Loader::Ptr loader = root->loader();
		loader->moduleNew("Binary\\ODE.dll");
		loader->moduleNew("Binary\\FMOD.dll");
		//loader->moduleNew("Binary\\libODE.so");
		//loader->moduleNew("Binary\\libFMOD.so");

		Mesh::Ptr mesh = loader->meshNew("test.x");
		mesh->boundingSphere(Sphere(Vector(0.0f, 0.0f, 0.0f), 10.0f));
		
        Model::Ptr model = root->modelNew();
		model->mesh(mesh);

		Body::Ptr body = root->bodyNew();
		body->mass(10.0f);
		body->radius(10.0f);
		body->linearVelocity(Vector(10.0f, 0.0f, 0.0f));
		body->attachmentAdd(Attachment(model, Vector(1.0f, 0.0f, 0.0f), Quaternion()));
		
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
