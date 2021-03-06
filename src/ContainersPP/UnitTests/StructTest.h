//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
//#include "ContainersPP/Types/schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "ContainersPP/Types/Entity.h"
#include "ContainersPP/Containers/EntityTable.h"
#include "ContainersPP/Containers/Entity.h"

#include <assert.h>
//#define CHECK assert

using namespace ContainersPP;

bool TestStruct()
{
	Types::TypeDescr td;
	td.type = Types::baseTypes::int32;

	CHECK(!td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	CHECK(!td.Columnar());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(!td.Cached());
	CHECK(!td.Subscribable());

	td.Nullable(true);
	//td.CustomDefault(true);

	CHECK(td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	CHECK(!td.Columnar());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(!td.Cached());
	CHECK(!td.Subscribable());

	td.Nullable(false);
	//td.CustomDefault(true);
	td.Cached(true);

	CHECK(!td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	//CHECK(td.CustomDefault());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(td.Cached());
	CHECK(!td.Subscribable());



	Types::Schema s;
	uint64_t zs1 = sizeof(Types::KeyString);
	uint64_t zs2 = sizeof(Buffer);

	CHECK(s.AddAttribute(Types::MakeKey("dingleCount"), Types::TypeDescr(Types::baseTypes::uint32,0) ));
	CHECK(s.labels.Size() == 1);
	CHECK(s.FindIndex("dingleCount") == 0);
	

	CHECK(s.AddAttribute(Types::MakeKey("dingleratio"), {Types::baseTypes::uint64,0 }));
	CHECK(!s.AddAttribute(Types::MakeKey("dingleratio"), { Types::baseTypes::float64,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingleletter"), {Types::baseTypes::chr,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingletruth"), { Types::baseTypes::boolean,0 }));
	//CHECK(s.AddAttribute(Types::MakeKey("dingleflags"), { Types::baseTypes::boolean,0 }));//todo:: test multibool and fixed multi bool
	CHECK(s.AddAttribute(Types::MakeKey("name"), { Types::baseTypes::String,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingles"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,1,0,0,0,0,0,0)}));
	CHECK(s.AddAttribute(Types::MakeKey("dinglenums"), { Types::baseTypes::uint32, Types::TypeDescr::setFLags(0,1,0,0,0,0,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dinglet"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,0,0,0,0,1,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingtime"), { Types::baseTypes::uint32, Types::TypeDescr::setFLags(1,0,0,0,0,0,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingwimple"), { Types::baseTypes::uint64, Types::TypeDescr::setFLags(0,0,1,0,0,0,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingzipper"), { Types::baseTypes::uint64, Types::TypeDescr::setFLags(0,0,0,1,0,0,0,0) }));

	size_t size = s.SizeOfFixed();
	CHECK(size == 18);
	   
	CHECK(s.SeperatedColumnCount() == 3);
	//uint64_t dbg = s.BlockCount(5);
	CHECK(s.BlockCount(0) == 1);
	CHECK(s.BlockCount(1) == 1);
	CHECK(s.BlockCount(2) == 1);
	CHECK(s.BlockCount(3) == 3);
	CHECK(s.BlockCount(4) == 1);
	CHECK(s.BlockCount(5) == 1);//TODO:: test multi and fixed multi
	CHECK(s.BlockCount(6) == 1);
	CHECK(s.BlockCount(7) == 1);
	CHECK(s.BlockCount(8) == 1);


	//Test Indexes	
	CHECK(s.FindIndex("dingletruth") == 2);
	CHECK(s.FindIndex("dingleCount") == 3);
	CHECK(s.FindIndex("dingleratio") == 5);
	CHECK(s.FindIndex("dingleletter") == 4);
	CHECK(s.FindIndex("dingzipper") == 6);
	CHECK(s.FindIndex("dinglenums") == 7);
	CHECK(s.FindIndex("name") == 8);
	CHECK(s.FindIndex("dingles") == 9);
	CHECK(s.FindIndex("dinglet") == 10);
	CHECK(s.FindIndex("dingtime") == 0);
	CHECK(s.FindIndex("dingwimple") == 1);


	//TODO:: Test Offsets
	//uint64_t dbg = s.GetOffset(s.FindIndex("dinglet"));
	CHECK(s.GetOffset(s.FindIndex("dingtime")) == 1); //nullable, flag offset is column index
	CHECK(s.GetOffset(s.FindIndex("dingwimple")) == 8); //sparse, offset = size
	CHECK(s.GetOffset(s.FindIndex("dingletruth")) == 2); //bool, offset is in bits
	CHECK(s.GetOffset(s.FindIndex("dingleCount")) == 5);
	CHECK(s.GetOffset(s.FindIndex("dingleletter")) == 9);
	CHECK(s.GetOffset(s.FindIndex("dingleratio")) == 10);
	
	CHECK(s.GetOffset(s.FindIndex("dingzipper")) == 1); //columnar, offset is columnstore index
	CHECK(s.GetOffset(s.FindIndex("dinglenums")) == 2);
	CHECK(s.GetOffset(s.FindIndex("name")) == 3); 
	CHECK(s.GetOffset(s.FindIndex("dingles")) == 0); //Multivar, offset is allocators index
	//CHECK(s.GetOffset(s.FindIndex("dinglet")) == 5);

	//TODO:: Custom Defaults test
	Buffer eb(s.SizeOfFixed());
	s.WriteDefaults(eb);

	
	//bit pointer test
	uint8_t set[3] = { 0,0,0 };
	Types::BitItr<uint8_t,1> b(set);
	b[0] = true;
	b[1] = false;
	b[2] = false;
	b[3] = true;

	CHECK(*b);
	b += 1;
	CHECK(!*b);
	b += 1;
	CHECK(!*b);
	b += 1;
	CHECK(*b);
	
	//TODO:: Test Entity
	Entity e(&s);

	//CHECK GetValue
	e.GetValue<uint32_t>("dingleCount")[0] = 12;
	e.GetValue<double>("dingleratio")[0] = 55.55;
	e.GetValue<uint8_t>("dingleletter")[0] = 11;

	CHECK(!e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check GetBool
	e.GetBool("dingletruth")[0] = true;
	
	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check SetValue
	e.SetValue<uint32_t>("dingtime", 101);

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	e.SetValue<uint32_t>("dingwimple", 202);

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(e.GetValue<uint64_t>("dingwimple")[0] == 202);

	//Check SetNull
	e.SetNull("dingtime");

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple")[0] == 202);

	e.SetNull("dingwimple");

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//CHECK GetValue "Columnar"
	CHECK(e.GetValue<uint64_t>("dingzipper")[0] == 0); //Check Default
	e.GetValue<uint64_t>("dingzipper")[0] = 151515;

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingzipper")[0] == 151515);

	//Check GetVar
	e.GetVar("name").CopyBack((const uint8_t*)"helloWorld", 10);
	CHECK(e.GetVar("name").Data()[0] == 'h');

	//Check GetMulti
	//TODO::
	CHECK(e.GetMulti<uint32_t>("dinglenums").Size() == 0);
	e.GetMulti<uint32_t>("dinglenums").PushBack(606);
	e.GetMulti<uint32_t>("dinglenums").PushBack(607);
	CHECK(e.GetMulti<uint32_t>("dinglenums").Size() == 2);

	//Check GetMultiVar
	CHECK(e.GetMultiVar("dingles").Count() == 0);
	CHECK(e.GetMultiVar("dingles").New(10) == 0); 
	CHECK(e.GetMultiVar("dingles").Count() == 1);
	CHECK(e.GetMultiVar("dingles")[0].Size() == 0);
	e.GetMultiVar("dingles")[0].CopyBack((const uint8_t*)"helloWorld", 10);
	e.GetMultiVar("dingles")[e.GetMultiVar("dingles").New(11)].CopyBack((const uint8_t*)"hello2World", 11);
	CHECK(e.GetMultiVar("dingles").Count() == 2);
	CHECK(e.GetMultiVar("dingles")[0].Size() == 10);
	CHECK(e.GetMultiVar("dingles")[1].Size() == 11);



	EntityTable t(&s);
	CHECK(t.Count() == 0);
	CHECK(t.Insert() == 0);
	CHECK(t.Count() == 1);
	CHECK(t.Insert() == 1);
	CHECK(t.Count() == 2);

	auto r = t[1];

	//CHECK GetValue
	r.GetValue<uint32_t>("dingleCount")[0] = 12;
	r.GetValue<double>("dingleratio")[0] = 55.55;
	r.GetValue<uint8_t>("dingleletter")[0] = 11;

	CHECK(!r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check GetBool
	r.GetBool("dingletruth")[0] = true;

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check SetValue
	r.SetValue<uint32_t>("dingtime", 101);

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(r.GetValue<uint64_t>("dingwimple") == nullptr);

	r.SetValue<uint32_t>("dingwimple", 202);

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(r.GetValue<uint64_t>("dingwimple")[0] == 202);

	//Check SetNull
	r.SetNull("dingtime");

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingwimple")[0] == 202);

	r.SetNull("dingwimple");

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingwimple") == nullptr);

	//CHECK GetValue "Columnar"
	CHECK(r.GetValue<uint64_t>("dingzipper")[0] == 0); //Check Default
	r.GetValue<uint64_t>("dingzipper")[0] = 151515;

	CHECK(r.GetBool("dingletruth")[0]);
	CHECK(r.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(r.GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(r.GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(r.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingwimple") == nullptr);
	CHECK(r.GetValue<uint64_t>("dingzipper")[0] == 151515);

	//Check GetVar
	r.GetVar("name").CopyBack((const uint8_t*)"helloWorld", 10);
	CHECK(r.GetVar("name").Data()[0] == 'h');

	//Check GetMulti
	//TODO::
	CHECK(r.GetMulti<uint32_t>("dinglenums").Size() == 0);
	r.GetMulti<uint32_t>("dinglenums").PushBack(606);
	r.GetMulti<uint32_t>("dinglenums").PushBack(607);
	CHECK(r.GetMulti<uint32_t>("dinglenums").Size() == 2);

	//Check GetMultiVar
	CHECK(r.GetMultiVar("dingles").Count() == 0);
	CHECK(r.GetMultiVar("dingles").New(10) == 0);
	CHECK(r.GetMultiVar("dingles").Count() == 1);
	CHECK(r.GetMultiVar("dingles")[0].Size() == 0);
	r.GetMultiVar("dingles")[0].CopyBack((const uint8_t*)"helloWorld", 10);
	r.GetMultiVar("dingles")[r.GetMultiVar("dingles").New(11)].CopyBack((const uint8_t*)"hello2World", 11);
	CHECK(r.GetMultiVar("dingles").Count() == 2);
	CHECK(r.GetMultiVar("dingles")[0].Size() == 10);
	CHECK(r.GetMultiVar("dingles")[1].Size() == 11);




	CHECK(t[1].GetBool("dingletruth")[0]);
	CHECK(t[1].GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(t[1].GetValue<double>("dingleratio")[0] == 55.55);
	CHECK(t[1].GetValue<uint8_t>("dingleletter")[0] == 11);
	CHECK(t[1].GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(t[1].GetValue<uint64_t>("dingwimple") == nullptr);
	CHECK(t[1].GetValue<uint64_t>("dingzipper")[0] == 151515);
	CHECK(t[1].GetVar("name").Data()[0] == 'h');
	CHECK(t[1].GetMultiVar("dingles").Count() == 2);
	CHECK(t[1].GetMultiVar("dingles")[0].Size() == 10);
	CHECK(t[1].GetMultiVar("dingles")[1].Size() == 11);

	CHECK(!t[0].GetBool("dingletruth")[0]);
	CHECK(t[0].GetValue<uint32_t>("dingleCount")[0] == 0);
	CHECK(t[0].GetValue<double>("dingleratio")[0] == 0);
	CHECK(t[0].GetValue<uint8_t>("dingleletter")[0] == 0);
	CHECK(t[0].GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(t[0].GetValue<uint64_t>("dingwimple") == nullptr);
	CHECK(t[0].GetValue<uint64_t>("dingzipper")[0] == 0);
	CHECK(t[0].GetVar("name").Size() == 0);
	CHECK(t[0].GetMultiVar("dingles").Count() == 0);

	return true;
}