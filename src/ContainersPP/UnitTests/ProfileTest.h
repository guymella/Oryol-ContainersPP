//------------------------------------------------------------------------------
//  objectTest.cc
//------------------------------------------------------------------------------
//#include "ContainersPP/Containers/Object.h"
#include "ContainersPP/Containers/PackageAllocator.h"
#include "ContainersPP/Containers/CatalogueAllocator.h"
using namespace ContainersPP;

#define CHECK o_assert_dbg




bool TestProfile() {

    CatalogueAllocator a;
    
    CHECK(a.New()==0);
    CHECK(a.Get(0));
    CatalogueBase& c = *(a.Get(0));
    CHECK(c.DefineAttribute("x", Types::baseTypes::float32));
    CHECK(c.DefineAttribute("y", Types::baseTypes::float32));
    CHECK(c.DefineAttribute("z", Types::baseTypes::float32));

    CHECK(a[0].Get("y").SetValue(1.5));
    CHECK(1.5 == *(float_t*)c.Get("y").Ptr());

    return true;
}


