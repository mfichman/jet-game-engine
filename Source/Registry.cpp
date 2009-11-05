#include <Registry.hpp>
#include <Object.hpp>

using namespace Jet;

Registry::Registry()
{
    Registry::accessor("position", &Object::position);
    Registry::accessor("rotation", &Object::rotation);
    //Registry::accessor("anchor", &Object::anchor);
    //Registry::accessor("frame", &Object::frame);
    Registry::mutator("position", &Object::position);
    Registry::mutator("rotation", &Object::rotation);
    //Registry::mutator("frame", &Object::frame);
}
