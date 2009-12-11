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
#include <Jet/Physics/Sphere.hpp>
#include <Jet/Physics/Plane.hpp>
#include <Jet/Physics/Ray.hpp>
#include <Jet/Physics/Box.hpp>
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
	typedef intrusive_ptr<Engine> Ptr;
	
	typedef vector<Object::Ptr>     ObjectContainer;
	typedef vector<Sphere::Ptr>     SphereContainer;
	typedef vector<Box::Ptr>        BoxContainer;
	typedef vector<Plane::Ptr>      PlaneContainer;
	typedef vector<Ray::Ptr>        RayContainer;
	
	typedef Iterator<ObjectContainer>       ObjectItr;
	typedef Iterator<SphereContainer>       SphereItr;
	typedef Iterator<BoxContainer>          BoxItr;
	typedef Iterator<PlaneContainer>        PlaneItr;
	typedef Iterator<RayContainer>          RayItr;

    Object::Ptr             objectCreate();
	Sphere::Ptr             sphereCreate();
    Box::Ptr		        boxCreate();
    Plane::Ptr              planeCreate();
    Ray::Ptr		        rayCreate();
    
    inline ObjectItr    objectItr() { return ObjectItr(object_); }
	inline SphereItr    sphereItr() { return SphereItr(sphere_); }
	inline BoxItr		boxItr() { return BoxItr(box_); }
	inline PlaneItr	    planeItr() { return PlaneItr(plane_); }
	inline RayItr		rayItr() { return RayItr(ray_); }
    
private:
	Engine() {}

    ObjectContainer     object_;
    SphereContainer     sphere_;
    BoxContainer        box_;
    PlaneContainer      plane_;
    RayContainer        ray_;
};

class Engine::Listener : public Interface{
public:
    typedef intrusive_ptr<Engine::Listener> Ptr;

    virtual ~Listener() {}
    virtual void onObjectCreate(Object::Ptr o)=0;
    virtual void onSphereCreate(Sphere::Ptr c)=0;
    virtual void onBoxCreate(Box::Ptr s)=0;
    virtual void onPlaneCreate(Plane::Ptr m)=0;
    virtual void onRayCreate(Ray::Ptr s)=0;
};

}}
