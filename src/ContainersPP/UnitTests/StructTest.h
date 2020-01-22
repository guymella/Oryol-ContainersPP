//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
//#include "ContainersPP/Types/schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "ContainersPP/Types/Entity.h"
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
	//CHECK(!td.CustomDefault());
	CHECK(!td.Constrained());
	CHECK(!td.Derived());
	CHECK(!td.Cached());
	CHECK(!td.Subscribable());

	td.Nullable(true);
	//td.CustomDefault(true);

	CHECK(td.Nullable());
	CHECK(!td.Multiple());
	CHECK(!td.Sparse());
	//CHECK(td.CustomDefault());
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
	//CHECK(s.AddAttribute(Types::MakeKey("dingleflags"), { Types::baseTypes::boolean,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("name"), { Types::baseTypes::String,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingles"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,1,0,0,0,0,0)}));
	CHECK(s.AddAttribute(Types::MakeKey("dinglet"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,0,0,0,1,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingtime"), { Types::baseTypes::uint32, Types::TypeDescr::setFLags(1,0,0,0,0,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingwimple"), { Types::baseTypes::uint64, Types::TypeDescr::setFLags(0,0,1,0,0,0,0) }));

	size_t size = s.SizeOfFixed();
	CHECK(size == 18);
	//Test Indexes	
	CHECK(s.FindIndex("dingletruth") == 2);
	CHECK(s.FindIndex("dingleCount") == 3);
	CHECK(s.FindIndex("dingleratio") == 5);
	CHECK(s.FindIndex("dingleletter") == 4);
	CHECK(s.FindIndex("name") == 6);
	CHECK(s.FindIndex("dingles") == 7);
	CHECK(s.FindIndex("dinglet") == 8);
	CHECK(s.FindIndex("dingtime") == 0);
	CHECK(s.FindIndex("dingwimple") == 1);


	//TODO:: Test Offsets
	//uint64_t dbg = s.GetOffset(s.FindIndex("dingles"));
	CHECK(s.GetOffset(s.FindIndex("dingtime")) == 1); //nullable, flag offset is column index
	CHECK(s.GetOffset(s.FindIndex("dingwimple")) == 8); //sparse, offset = size
	CHECK(s.GetOffset(s.FindIndex("dingletruth")) == 2); //bool, offset is in bits
	CHECK(s.GetOffset(s.FindIndex("dingleCount")) == 5);
	CHECK(s.GetOffset(s.FindIndex("dingleletter")) == 9);
	CHECK(s.GetOffset(s.FindIndex("dingleratio")) == 10);
	CHECK(s.GetOffset(s.FindIndex("name")) == 1); //columnar offset is columnstore index
	CHECK(s.GetOffset(s.FindIndex("dingles")) == 2);
	CHECK(s.GetOffset(s.FindIndex("dinglet")) == 3);

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
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check GetBool
	e.GetBool("dingletruth")[0] = true;
	
	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check SetValue
	e.SetValue<uint32_t>("dingtime", 101);

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	e.SetValue<uint32_t>("dingwimple", 202);

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime")[0] == 101);
	CHECK(e.GetValue<uint64_t>("dingwimple")[0] == 202);

	//Check SetNull
	e.SetNull("dingtime");

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple")[0] == 202);

	e.SetNull("dingwimple");

	CHECK(e.GetBool("dingletruth")[0]);
	CHECK(e.GetValue<uint32_t>("dingleCount")[0] == 12);
	CHECK(e.GetValue<double>("dingleratio")[0] = 55.55);
	CHECK(e.GetValue<uint8_t>("dingleletter")[0] = 11);
	CHECK(e.GetValue<uint32_t>("dingtime") == nullptr);
	CHECK(e.GetValue<uint64_t>("dingwimple") == nullptr);

	//Check GetVar
	//TODO::

	//Check GetArray
	//TODO::



	return true;
}