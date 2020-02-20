//------------------------------------------------------------------------------
//  objectTest.cc
//------------------------------------------------------------------------------
//#include "ContainersPP/Containers/Object.h"
#include "ContainersPP/Containers/Catalogue.h"
using namespace ContainersPP;

#define CHECK o_assert_dbg

template <typename TYPE = uint64_t, Types::baseTypes type = Types::baseTypes::uint64>
bool TestObject() {

    Primitive p(type);
    TYPE val = 5;
    CHECK(p.GetValue(val));
    CHECK(val == 0);
    CHECK(p.SetValue((TYPE)6));
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
    CHECK(op.SetValue((TYPE)12));
    CHECK(op.GetValue(val));
    CHECK(val == 12);
    CHECK(p.SetValue((TYPE)6));
    CHECK(p.GetValue(val));
    CHECK(val == 6);

    //test primitive object array
    TYPE a[5] = { 0,0,0,0,0 };
    Object op1(type,a,5);

    CHECK(!op1.SetValue((TYPE)12));
    CHECK(op1.GetIndexValue(0,val));
    CHECK(op1.SetIndexValue(1, (TYPE)5));

    CHECK(a[1] == 5);

    Buffer b;

    Object op2(type, b);

    CHECK(!op2.Count());
    CHECK(op2.PushBack((TYPE)12));
    CHECK(op2.PushBack((TYPE)13));
    CHECK(op2.PushBack((TYPE)14));
    CHECK(op2.Count() ==3);

    CHECK(((TYPE*)op2.Begin())[0] == 12);
    CHECK(((TYPE*)op2.Begin())[1] == 13);
    CHECK(((TYPE*)op2.Begin())[2] == 14);

    CHECK(op2.GetIndexValue(0, val));
    CHECK(val == 12);
    CHECK(op2.GetIndexValue(1, val));
    CHECK(val == 13);
    CHECK(op2.GetIndexValue(2, val));
    CHECK(val == 14);
    CHECK(!op2.GetIndexValue(3, val));
    CHECK(val == 14);

    Object ptr(type, op2.Begin(1));
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

    // Test fixed List

    CHECK(c.DefineAttribute("Hellos", Types::baseTypes::int32,5));
    CHECK(c.DefineAttribute("Worlds", Types::baseTypes::chr,12));
    Object o = c.Get("Hellos");
    CHECK(o.Count() == 5);
    CHECK(o.SetIndexValue(0, -1));
    CHECK(o.SetIndexValue(4, -2));
    int32_t* op = (int32_t *)o.Begin();
    op[1] = 1;
    op[2] = 2;
    op[3] = 3;

    CHECK(op[0] == -1);
    CHECK(op[1] == 1);
    CHECK(op[2] == 2);
    CHECK(op[3] == 3);
    CHECK(op[4] == -2);
    

    int32_t valo = 0;
    CHECK(o.GetIndexValue(4, valo));
    CHECK(valo == -2);
    CHECK(o.GetIndexValue(3, valo));
    CHECK(valo == 3);
    CHECK(o.GetIndexValue(2, valo));
    CHECK(valo == 2);
    op = (int32_t*)o.Ptr();
    CHECK(op[0] == -1);
    CHECK(op[1] == 1);

    Object o2 = c.Get("Worlds");
    CHECK(o2.Count() == 12);
    CHECK(o2.SetIndexValue(0, 'a'));
    CHECK(o2.SetIndexValue(4, 'e'));
    char* o2p = (char*)o2.Begin();
    o2p[1] = 'b';
    o2p[2] = 'c';
    o2p[3] = 'd';
    
    Object o25 = o2.GetIndex(5);
    o25.SetValue('f');

    CHECK(o2p[0] == 'a');
    CHECK(o2p[1] == 'b');
    CHECK(o2p[2] == 'c');
    CHECK(o2p[3] == 'd');
    CHECK(o2p[4] == 'e');
    CHECK(o2p[5] == 'f');

    // Test Flex List

    CHECK(c.DefineAttribute("sups", Types::baseTypes::int32, 0));
    CHECK(c.DefineAttribute("vons", Types::baseTypes::chr, 0));
    Object of = c.Get("sups");
    CHECK(of.Count() == 0);
    CHECK(of.PushBack(-1));
    CHECK(of.PushBack(1));
    CHECK(of.PushBack(2));
    CHECK(of.PushBack(3));
    CHECK(of.PushBack(-2));
    CHECK(of.Count() == 5);

    op = (int32_t*)of.Begin();
   
    CHECK(op[0] == -1);
    CHECK(op[1] == 1);
    CHECK(op[2] == 2);
    CHECK(op[3] == 3);
    CHECK(op[4] == -2);

    of.Insert(3, 5);
    CHECK(of.Count() == 6);
    op = (int32_t*)of.Begin();

    CHECK(op[0] == -1);
    CHECK(op[1] == 1);
    CHECK(op[2] == 2);
    CHECK(op[3] == 5);
    CHECK(op[4] == 3);
    CHECK(op[5] == -2);


    Object of2 = c.Get("vons");
    CHECK(of2.Count() == 0);
    CHECK(of2.PushBack('a'));
    CHECK(of2.PushBack('b'));
    CHECK(of2.PushBack('c'));
    CHECK(of2.PushBack('d'));
    CHECK(of2.PushBack('e'));
    CHECK(of2.PushBack('f'));
    CHECK(of2.Count() == 6);

    o2p = (char*)of2.Begin();
   
    CHECK(o2p[0] == 'a');
    CHECK(o2p[1] == 'b');
    CHECK(o2p[2] == 'c');
    CHECK(o2p[3] == 'd');
    CHECK(o2p[4] == 'e');
    CHECK(o2p[5] == 'f');

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
