/* WARNING this file is auto-generated.  Do not modify */
#include <Jet/Registry.hpp>

using namespace Jet;

Registry::Registry() {

#if 0
    Class::Ptr c;
    // class Jet::Physics::Box
    c = type<Jet::Physics::Box>();
    c->accessor("width", &Jet::Physics::Box::width);
    c->accessor("height", &Jet::Physics::Box::height);
    //c->accessor("parent", &Jet::Physics::Box::parent);
    c->mutator("width", &Jet::Physics::Box::width);
    c->mutator("height", &Jet::Physics::Box::height);
    //c->mutator("parent", &Jet::Physics::Box::parent);


    // class Jet::Graphics::Quad
    c = type<Jet::Graphics::Quad>();
    //c->accessor("texCoordScale", &Jet::Graphics::Quad::texCoordScale);
    c->accessor("scale", &Jet::Graphics::Quad::scale);
    //c->accessor("shader", &Jet::Graphics::Quad::shader);
    //c->accessor("parent", &Jet::Graphics::Quad::parent);
    //c->mutator("vertex", &Jet::Graphics::Quad::vertex);
    //c->mutator("vertex", &Jet::Graphics::Quad::vertex);
    c->mutator("texCoordScale", &Jet::Graphics::Quad::texCoordScale);
    c->mutator("scale", &Jet::Graphics::Quad::scale);
    //c->mutator("texture", &Jet::Graphics::Quad::texture);
    //c->mutator("shader", &Jet::Graphics::Quad::shader);
    //c->mutator("parent", &Jet::Graphics::Quad::parent);
    
    
/*
    // class Jet::Graphics::Material
    c = type<Jet::Graphics::Material>();
    c->accessor("diffuseColor", &Jet::Graphics::Material::diffuseColor);
    c->accessor("ambientColor", &Jet::Graphics::Material::ambientColor);
    c->accessor("specularColor", &Jet::Graphics::Material::specularColor);
    c->accessor("specularPower", &Jet::Graphics::Material::specularPower);
    c->accessor("reflectivity", &Jet::Graphics::Material::reflectivity);
    //c->accessor("name", &Jet::Graphics::Material::name);
    c->accessor("id", &Jet::Graphics::Material::id);
    c->mutator("diffuseColor", &Jet::Graphics::Material::diffuseColor);
    c->mutator("ambientColor", &Jet::Graphics::Material::ambientColor);
    c->mutator("specularColor", &Jet::Graphics::Material::specularColor);
    c->mutator("specularPower", &Jet::Graphics::Material::specularPower);
    c->mutator("reflectivity", &Jet::Graphics::Material::reflectivity);
*/
    

    // class Jet::Graphics::Cubemap
    c = type<Jet::Graphics::Cubemap>();
    //c->accessor("name", &Jet::Graphics::Cubemap::name);
    c->accessor("id", &Jet::Graphics::Cubemap::id);

    // class Jet::Physics::Sphere
    c = type<Jet::Physics::Sphere>();
    c->accessor("radius", &Jet::Physics::Sphere::radius);
    //c->accessor("parent", &Jet::Physics::Sphere::parent);
    c->mutator("radius", &Jet::Physics::Sphere::radius);
    //c->mutator("parent", &Jet::Physics::Sphere::parent);
    
    

    // class Jet::Physics::Body
    c = type<Jet::Physics::Body>();
   // c->accessor("object", &Jet::Physics::Body::object);
    c->accessor("velocity", &Jet::Physics::Body::velocity);
    c->accessor("angularVelocity", &Jet::Physics::Body::angularVelocity);
    //c->mutator("object", &Jet::Physics::Body::object);
    c->mutator("velocity", &Jet::Physics::Body::velocity);
    c->mutator("angularVelocity", &Jet::Physics::Body::angularVelocity);
    
    

    // class Jet::Graphics::Model
    c = type<Jet::Graphics::Model>();
    //c->accessor("mesh", &Jet::Graphics::Model::mesh);
    //c->accessor("material", &Jet::Graphics::Model::material);
    c->accessor("scale", &Jet::Graphics::Model::scale);
    //c->accessor("shader", &Jet::Graphics::Model::shader);
    //c->accessor("parent", &Jet::Graphics::Model::parent);
    //c->mutator("mesh", &Jet::Graphics::Model::mesh);
    //c->mutator("material", &Jet::Graphics::Model::material);
    //c->mutator("cubemap", &Jet::Graphics::Model::cubemap);
    c->mutator("scale", &Jet::Graphics::Model::scale);
    //c->mutator("texture", &Jet::Graphics::Model::texture);
    //c->mutator("shader", &Jet::Graphics::Model::shader);
    //c->mutator("parent", &Jet::Graphics::Model::parent);



    // class Jet::Graphics::Mesh
    c = type<Jet::Graphics::Mesh>();
    //c->accessor("name", &Jet::Graphics::Mesh::name);
    c->accessor("id", &Jet::Graphics::Mesh::id);

    // class Jet::Graphics::Texture
    c = type<Jet::Graphics::Texture>();
    //c->accessor("name", &Jet::Graphics::Texture::name);
    c->accessor("id", &Jet::Graphics::Texture::id);

    // class Jet::Physics::Plane
    c = type<Jet::Physics::Plane>();
    c->accessor("normal", &Jet::Physics::Plane::normal);
    c->accessor("depth", &Jet::Physics::Plane::depth);
    c->mutator("normal", &Jet::Physics::Plane::normal);
    c->mutator("depth", &Jet::Physics::Plane::depth);
    
    

    // class Jet::Graphics::TextBox
    c = type<Jet::Graphics::TextBox>();
    //c->accessor("fontFamily", &Jet::Graphics::TextBox::fontFamily);
    //c->accessor("fontSize", &Jet::Graphics::TextBox::fontSize);
    c->accessor("fontColor", &Jet::Graphics::TextBox::fontColor);
    //c->accessor("text", &Jet::Graphics::TextBox::text);
    //c->accessor("size", &Jet::Graphics::TextBox::size);
    c->accessor("position", &Jet::Graphics::TextBox::position);
    //c->mutator("fontFamily", &Jet::Graphics::TextBox::fontFamily);
    //c->mutator("fontSize", &Jet::Graphics::TextBox::fontSize);
    c->mutator("fontColor", &Jet::Graphics::TextBox::fontColor);
    //c->mutator("text", &Jet::Graphics::TextBox::text);
    //c->mutator("charBuffer", &Jet::Graphics::TextBox::charBuffer);
    //c->mutator("size", &Jet::Graphics::TextBox::size);
    c->mutator("position", &Jet::Graphics::TextBox::position);

    // class Jet::Object
    c = type<Jet::Object>();
    c->accessor("position", &Jet::Object::position);
    c->accessor("rotation", &Jet::Object::rotation);
   // c->accessor("anchor", &Jet::Object::anchor);
    c->accessor("frame", &Jet::Object::frame);
    c->mutator("position", &Jet::Object::position);
    c->mutator("rotation", &Jet::Object::rotation);
    //c->mutator("anchor", &Jet::Object::anchor);
    c->mutator("frame", &Jet::Object::frame);

    // class Jet::Graphics::Shader
    c = type<Jet::Graphics::Shader>();
    //c->accessor("name", &Jet::Graphics::Shader::name);
    c->accessor("id", &Jet::Graphics::Shader::id);

    // class Jet::Graphics::Camera
    c = type<Jet::Graphics::Camera>();
    //c->accessor("target", &Jet::Graphics::Camera::target);
    c->accessor("up", &Jet::Graphics::Camera::up);
    //c->accessor("parent", &Jet::Graphics::Camera::parent);
    //c->accessor("tightness", &Jet::Graphics::Camera::tightness);
    //c->mutator("target", &Jet::Graphics::Camera::target);
    c->mutator("up", &Jet::Graphics::Camera::up);
    //c->mutator("parent", &Jet::Graphics::Camera::parent);
    c->mutator("tightness", &Jet::Graphics::Camera::tightness);

    // class Jet::Physics::Ray
    c = type<Jet::Physics::Ray>();
    c->accessor("length", &Jet::Physics::Ray::length);
    c->accessor("direction", &Jet::Physics::Ray::direction);
    //c->accessor("parent", &Jet::Physics::Ray::parent);
    c->mutator("length", &Jet::Physics::Ray::length);
    c->mutator("direction", &Jet::Physics::Ray::direction);
    //c->mutator("parent", &Jet::Physics::Ray::parent);

    // class Jet::Anchor
    c = type<Jet::Anchor>();
    c->accessor("position", &Jet::Anchor::position);
    c->accessor("rotation", &Jet::Anchor::rotation);
    //c->accessor("parent", &Jet::Anchor::parent);
    c->mutator("position", &Jet::Anchor::position);
    c->mutator("rotation", &Jet::Anchor::rotation);
    //c->mutator("parent", &Jet::Anchor::parent);*/
    
#endif

}
