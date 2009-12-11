

#include <Jet/Interface.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void 
Interface::refCountInc() const  {

    Interface* self = const_cast<Interface*>(this);
    self->refCount_++;
}

//------------------------------------------------------------------------------
void 
Interface::refCountDec() const  {

    Interface* self = const_cast<Interface*>(this);
    self->refCount_--; 
    if (self->refCount_ <= 0) {
        delete self;
    }
}
