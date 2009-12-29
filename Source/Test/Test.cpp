#include <Jet/Root.hpp>
#include <Jet/Registry.hpp>
#include <iostream>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;


int main(int argc, char** argv) {

	Root::Ptr root = Root::make();

    Loader::Ptr loader = root->loader();
    loader->moduleNew("./libODE.so");
    loader->moduleNew("./libFMOD.so");

    Sphere sphere = {10.0f, 0.0f, 0.0f, 0.0f};
    Mesh::Ptr mesh = loader->meshNew("test.x");
    mesh->boundingSphere(sphere);

    Actor::Ptr actor = root->actorNew();

    Model::Ptr model = root->modelNew();
    model->mesh(mesh);
    model->anchor()->parent(actor);
    model->collidable()->mass(10.0f);



	//Object::Ptr o = Object::make();
	
	//Class::Ptr c = r->type(o);
	//c->mutator("position")(o, SimpleSource());
	//c->accessor("position")(o, SimpleSink());
}
