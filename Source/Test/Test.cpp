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
    loader->moduleNew("./libOde.so");
    //loader->moduleDel("./libOde.so");

    Vector vec;
    vec[0] = 1.0f;

    Model::Ptr model = root->modelNew();
    model->object()->position(vec);



	//Object::Ptr o = Object::make();
	
	//Class::Ptr c = r->type(o);
	//c->mutator("position")(o, SimpleSource());
	//c->accessor("position")(o, SimpleSink());
}
