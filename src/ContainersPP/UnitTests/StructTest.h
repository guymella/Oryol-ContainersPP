//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
#include "ContainersPP/Types/schema.h"
#include "ContainersPP/Types/BitPointer.h"
//#include "ContainersPP/Types/Entity.h"

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
	

	CHECK(s.AddAttribute(Types::MakeKey("dingleratio"), {Types::baseTypes::float64,0 }));
	CHECK(!s.AddAttribute(Types::MakeKey("dingleratio"), { Types::baseTypes::float64,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingleletter"), {Types::baseTypes::chr,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("name"), { Types::baseTypes::String,0 }));
	CHECK(s.AddAttribute(Types::MakeKey("dingles"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,1,0,0,0,0,0)}));
	CHECK(s.AddAttribute(Types::MakeKey("dinglet"), { Types::baseTypes::String, Types::TypeDescr::setFLags(0,0,0,0,1,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingtime"), { Types::baseTypes::float32, Types::TypeDescr::setFLags(1,0,0,0,0,0,0) }));
	CHECK(s.AddAttribute(Types::MakeKey("dingwimple"), { Types::baseTypes::uint128, Types::TypeDescr::setFLags(0,0,1,0,0,0,0) }));

	size_t size = s.SizeOfFixed();

	CHECK(size == 14);

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

	//Entity s1(&s);

	

	return true;
}