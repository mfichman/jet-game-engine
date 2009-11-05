#include <Graphics/Engine.hpp>
#include <Physics/Engine.hpp>
#include <Root.hpp>
#include <Registry.hpp>

using namespace Jet;
using namespace Jet::Graphics;
using namespace Jet::Physics;
using namespace std;
using namespace std::tr1;

void f(int a) {}


template <typename T>
class Bob {
public:
	template <typename A>
	static void test(int a) {}
};

int main(int argc, char** argv) {

	Root::Ptr root = Root::make();
	Physics::Engine::Ptr p = root->physics();

	function<void (int, float)> b;

	Object::Ptr o = Object::make();

	for (Iterator<Box*> i = p->boxItr(); i; ++i) {
	}
	//std::mem_fun1_t<void, Graphics::Engine*, > m = mem_fun(&Graphics::Engine::bodyDestroy);

	ClassImpl<Object>::Ptr c = ClassImpl<Object>::instance();
	Registry::instance()->mutator("position", &Object::position);
	Registry::instance()->accessor("position", &Object::position);
	//bind(&ClassImpl<Object>::set, &Object::position, _1);
	//bind(&ClassImpl<Object>::set, 1);
	//bind(&f, 1);
	//bind(&Bob<int>::test<float>, 1);

	Interface::Ptr i = o;

	TypedTransformer<Vector> t = TypedTransformer<Vector>(Vector());
	i->set("position", t);


}
