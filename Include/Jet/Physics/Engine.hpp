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

#include <Jet/Iterator.hpp>
#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <vector>

namespace Jet { class Root; }

namespace Jet { namespace Physics {
using namespace std;
using namespace std::tr1;

class Body;
class Sphere;
class Box;
class Plane;
class Ray;

class Engine : public Interface {
public:
    class Listener;
	friend class Root;
	typedef shared_ptr<Engine> Ptr;
	
	typedef vector<shared_ptr<Body> >       BodyContainer;
	typedef vector<shared_ptr<Sphere> >     SphereContainer;
	typedef vector<shared_ptr<Box> >        BoxContainer;
	typedef vector<shared_ptr<Plane> >      PlaneContainer;
	typedef vector<shared_ptr<Ray> >        RayContainer;
	
	typedef Iterator<BodyContainer>         BodyItr;
	typedef Iterator<SphereContainer>       SphereItr;
	typedef Iterator<BoxContainer>          BoxItr;
	typedef Iterator<PlaneContainer>        PlaneItr;
	typedef Iterator<RayContainer>          RayItr;

	shared_ptr<Body>	bodyCreate();
	shared_ptr<Sphere>  sphereCreate();
    shared_ptr<Box>		boxCreate();
    shared_ptr<Plane>	planeCreate();
    shared_ptr<Ray>		rayCreate();
    
	void				bodyDestroy(Body* p);
    void				sphereDestroy(Sphere* p);
    void				boxDestroy(Box* p);
    void				planeDestroy(Plane* p);
    void				rayDestroy(Ray* p);    
    
	inline BodyItr		bodyItr() { return BodyItr(body_); }
	inline SphereItr    sphereItr() { return SphereItr(sphere_); }
	inline BoxItr		boxItr() { return BoxItr(box_); }
	inline PlaneItr	    planeItr() { return PlaneItr(plane_); }
	inline RayItr		rayItr() { return RayItr(ray_); }
    
private:
	Engine() {}

    BodyContainer       body_;
    SphereContainer     sphere_;
    BoxContainer        box_;
    PlaneContainer      plane_;
    RayContainer        ray_;
};

class Engine::Listener {
public:
    virtual ~Listener() {}
    virtual void onBodyCreate(shared_ptr<Body> t) {}
    virtual void onSphereCreate(shared_ptr<Sphere> c) {}
    virtual void onBoxCreate(shared_ptr<Box> s) {}
    virtual void onPlaneCreate(shared_ptr<Plane> m) {}
    virtual void onRayCreate(shared_ptr<Ray> s) {}
    
    virtual void onBodyDestroy(Body* t) {}
    virtual void onSphereDestroy(Sphere* c) {}
    virtual void onBoxDestroy(Box* s) {}
    virtual void onPlaneDestroy(Plane* m) {}
    virtual void onRayDestroy(Ray* s) {}
};

}}
