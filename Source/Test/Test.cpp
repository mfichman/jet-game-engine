#include <Jet/Graphics/Engine.hpp>
#include <Jet/Graphics/Model.hpp>
#include <Jet/Physics/Engine.hpp>
#include <Jet/Physics/Box.hpp>
#include <Jet/Physics/Object.hpp>
#include <Jet/Root.hpp>
#include <Jet/Registry.hpp>
#include <iostream>

using namespace Jet;
using namespace Jet::Graphics;
using namespace Jet::Physics;
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;


class SimpleSource : public Source {
public:
    void operator()(float& t) { cin >> t; }
    void operator()(int& t) { cin >> t; }
    void operator()(unsigned& t) { cin >> t; }
};

class SimpleSink : public Sink {
public:    
    void operator()(float& t) { cout << t << endl; }
    void operator()(int& t) { cout << t << endl; }
    void operator()(unsigned& t) { cout << t << endl; }
};


class Test { 
public:


    void test(Ordinal<float> f) {}

};


int main(int argc, char** argv) {


	Root::Ptr root = Root::make();
	Physics::Engine::Ptr p = root->physics();
	Graphics::Engine::Ptr g = root->graphics();
    Registry::Ptr r = root->registry();
	
	Object::Ptr o = Object::make();
	
	Class::Ptr c = r->type(o);
	//c->mutator("position")(o, SimpleSource());
	//c->accessor("position")(o, SimpleSink());
}
