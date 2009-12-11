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

#include <Jet/Graphics/Engine.hpp>
#include <Jet/Physics/Engine.hpp>
#include <Jet/Registry.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;

class Root : public Interface {
public:
	typedef intrusive_ptr<Graphics::Engine> GraphicsPtr;
	typedef intrusive_ptr<Physics::Engine> PhysicsPtr;
	typedef shared_ptr<Registry> RegistryPtr;
	typedef intrusive_ptr<Root> Ptr;

	inline GraphicsPtr	graphics() { return graphics_; }
	inline PhysicsPtr	physics() { return physics_; }
	inline RegistryPtr  registry() { return registry_; }

	static Root::Ptr	make() { return Root::Ptr(new Root()); }

private:
	Root() : graphics_(new Graphics::Engine), physics_(new Physics::Engine),
	    registry_(new Registry()) {}
	GraphicsPtr		graphics_;
	PhysicsPtr		physics_;
	RegistryPtr     registry_;
};

}
