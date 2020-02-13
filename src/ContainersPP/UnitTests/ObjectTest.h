//------------------------------------------------------------------------------
//  objectTest.cc
//------------------------------------------------------------------------------
//#include "ContainersPP/Containers/Object.h"
#include "ContainersPP/Containers/Catalogue.h"
using namespace ContainersPP;

#define CHECK o_assert_dbg

 
bool TestObject() {

    Primitive p(Types::baseTypes::uint64);
    uint64_t val = 5;
    CHECK(p.GetValue(val));
    CHECK(val == 0);
    CHECK(p.SetValue((uint64_t)6));
    CHECK(p.GetValue(val));
    CHECK(val == 6);

    //test null object
    Object o;

    CHECK(o.Begin() == 0);
    CHECK(o.Ptr() == 0);
    CHECK(o.Begin(5) == 0);
    CHECK(o.Count() == 0);
    CHECK(o.Type() == Types::baseTypes::Void);
    CHECK(o.CDRRef(0).Type() == Types::baseTypes::Void);

    //test primitive object
    Object op(p);

    val = 5;
    CHECK(op.GetValue(val));
    CHECK(val == 6);
    CHECK(op.SetValue((uint64_t)12));
    CHECK(op.GetValue(val));
    CHECK(val == 12);
    CHECK(p.SetValue((uint64_t)6));
    CHECK(p.GetValue(val));
    CHECK(val == 6);

    //test primitive object array
    uint64_t a[5] = { 0,0,0,0,0 };
    Object op1(Types::baseTypes::uint64,a,5);

    CHECK(!op1.SetValue((uint64_t)12));
    CHECK(op1.GetIndexValue(0,val));
    CHECK(op1.SetIndexValue(1, (uint64_t)5));

    CHECK(a[1] == 5);

    Buffer b;

    Object op2(Types::baseTypes::uint64, b);

    CHECK(!op2.Count());
    CHECK(op2.PushBack((uint64_t)12));
    CHECK(op2.PushBack((uint64_t)13));
    CHECK(op2.PushBack((uint64_t)14));
    CHECK(op2.Count() ==3);

    CHECK(((uint64_t*)op2.Begin())[0] == 12);
    CHECK(((uint64_t*)op2.Begin())[1] == 13);
    CHECK(((uint64_t*)op2.Begin())[2] == 14);

    CHECK(op2.GetIndexValue(0, val));
    CHECK(val == 12);
    CHECK(op2.GetIndexValue(1, val));
    CHECK(val == 13);
    CHECK(op2.GetIndexValue(2, val));
    CHECK(val == 14);
    CHECK(!op2.GetIndexValue(3, val));
    CHECK(val == 14);

    Object ptr(Types::baseTypes::uint64, op2.Begin(1));
    CHECK(ptr.GetValue(val));
    CHECK(val == 13);

    //test GetIndex() primative
    Object i = op2.GetIndex(2);
    CHECK(i.GetValue(val));
    CHECK(val == 14);

    return true;
}


bool TestCatalogue() {

    Catalogue c;

    CHECK(c.DefineAttribute("Hello", Types::baseTypes::uint64));
    CHECK(c.DefineAttribute("World", Types::baseTypes::uint64));

    CHECK(c.SchemaList()[0].offset == 0);
    CHECK(c.SchemaList()[1].offset == 8);

    CHECK(c.GetByIndex(0).SetValue((uint64_t)5));
    CHECK(c.GetByIndex(1).SetValue((uint64_t)12));

    uint64_t* raw = (uint64_t*)c.Ptr();
    CHECK(raw[0] == 5);
    CHECK(raw[1] == 12);

    uint64_t val = 0;
    CHECK(c.Get("Hello").GetValue(val));
    CHECK(val == 5);
    CHECK(c.Get("World").GetValue(val));
    CHECK(val == 12);

    //Todo:: Test instances list



    return true;
}

//bool TestFileCatalogue() {
//    FileBlock f("C:\\COSMIC_TEST\\1\\", 0);
//
//    FileCatalogue c("C:\\COSMIC_TEST\\1\\", 0);
//    c.Schema()[0].AddBack(sizeof(CatTypeDescr));
//
//
//    CHECK(c.DefineAttribute("Hello", Types::baseTypes::uint64));
//    c.Delete();
//    return true;
//}
//
bool TestFileCatalogue() {

    FileCatalogue c("C:\\COSMIC_TEST\\1\\", 0);

    CHECK(c.DefineAttribute("Hello", Types::baseTypes::uint64));
    CHECK(c.DefineAttribute("World", Types::baseTypes::uint64));

    CHECK(c.SchemaList()[0].offset == 0);
    CHECK(c.SchemaList()[1].offset == 8);

    CHECK(c.GetByIndex(0).SetValue((uint64_t)5));
    CHECK(c.GetByIndex(1).SetValue((uint64_t)12));

    uint64_t* raw = (uint64_t*)c.Ptr();
    CHECK(raw[0] == 5);
    CHECK(raw[1] == 12);

    uint64_t val = 0;
    CHECK(c.Get("Hello").GetValue(val));
    CHECK(val == 5);
    CHECK(c.Get("World").GetValue(val));
    CHECK(val == 12);

    //Todo:: Test instances list
    CHECK(c.Save());
    c.~FileCatalogue();


    //////////////////////////
    FileCatalogue c2("C:\\COSMIC_TEST\\1\\", 0);

    CHECK(c2.SchemaList()[0].offset == 0);
    CHECK(c2.SchemaList()[1].offset == 8);

    CHECK(c2.GetByIndex(0).SetValue((uint64_t)5));
    CHECK(c2.GetByIndex(1).SetValue((uint64_t)12));

    uint64_t* raw2 = (uint64_t*)c2.Ptr();
    CHECK(raw2[0] == 5);
    CHECK(raw2[1] == 12);

    uint64_t val2 = 0;
    CHECK(c2.Get("Hello").GetValue(val2));
    CHECK(val2 == 5);
    CHECK(c2.Get("World").GetValue(val2));
    CHECK(val2 == 12);

    //Todo:: Test instances list
    CHECK(c2.Delete());

    return true;
}
