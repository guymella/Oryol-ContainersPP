//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------

//#include "Structures/Containers/TableStruct.h"
#include "ContainersPP/Containers/Table.h"
#include "ContainersPP/Containers/InlineTable.h"
//#include "Structures/Containers/Block.h"
//#include "ContainersPP/Containers/Partition.h"
using namespace ContainersPP;
bool TestTable()
{
	
	//Test EditableTable
	Table tt;

	CHECK(!tt.Count());
	tt.New(8);
	CHECK(tt.Count() == 1);
	*(uint64_t*)tt[0].Data() = 0x05;
	CHECK(tt[0].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	*tt[0].Data(7) = 0x06;
	CHECK(*(uint64_t*)tt[0].Data() == 0x0600000000000005);
	*(uint64_t*)tt[0].Data() = 0x05;

	tt.New(8);
	CHECK(tt.Count() == 2);
	*(uint64_t*)tt[1].Data() = 0x09;
	CHECK(tt[1].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x09);

//uint64_t dbg = *(uint64_t*)tt[2].Data();


	tt.Insert(1,8);
	CHECK(tt.Count() == 3);
	*(uint64_t*)tt[1].Data() = 0x01;
	CHECK(tt[2].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x01);
	
	CHECK(tt[2].Data()[0] == 0x09);

	tt.Insert(1, 8);
	CHECK(tt.Count() == 4);
	*(uint64_t*)tt[1].Data() = 0x3;
	CHECK(tt[3].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x03);
	CHECK(tt[2].Data()[0] == 0x01);
	CHECK(tt[3].Data()[0] == 0x09);
	
	
	tt.Remove(2);
	CHECK(tt.Count() == 3);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x03);
	CHECK(tt[2].Data()[0] == 0x09);

	//TODO:: Test Partitions individually

	return true;
}

bool TestInlineTable()
{

	//Test EditableTable
	InlineTable tt;

	CHECK(!tt.Count());
	tt.New(8);
	CHECK(tt.Count() == 1);
	*(uint64_t*)tt[0].Data() = 0x05;
	CHECK(tt[0].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	*tt[0].Data(7) = 0x06;
	CHECK(*(uint64_t*)tt[0].Data() == 0x0600000000000005);
	*(uint64_t*)tt[0].Data() = 0x05;

	tt.New(8);
	CHECK(tt.Count() == 2);
	*(uint64_t*)tt[1].Data() = 0x09;
	CHECK(tt[1].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x09);

	//uint64_t dbg = *(uint64_t*)tt[2].Data();


	tt.Insert(1, 8);
	CHECK(tt.Count() == 3);
	*(uint64_t*)tt[1].Data() = 0x01;
	//uint64_t dbg = tt[2].Size();
	CHECK(tt[2].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x01);

	CHECK(tt[2].Data()[0] == 0x09);

	tt.Insert(1, 8);
	CHECK(tt.Count() == 4);
	*(uint64_t*)tt[1].Data() = 0x3;
	CHECK(tt[3].Size() == 8);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x03);
	CHECK(tt[2].Data()[0] == 0x01);
	CHECK(tt[3].Data()[0] == 0x09);


	tt.Remove(2);
	CHECK(tt.Count() == 3);
	CHECK(tt[0].Data()[0] == 0x05);
	CHECK(tt[1].Data()[0] == 0x03);
	CHECK(tt[2].Data()[0] == 0x09);

	//uint64_t dbg = tt.Count();

	//TEST subtable
	InlineSubTable st(&tt, tt.New());

	CHECK(!st.Count());
	st.New(8);
	CHECK(st.Count() == 1);
	*(uint64_t*)st[0].Data() = 0x05;
	CHECK(st[0].Size() == 8);
	CHECK(st[0].Data()[0] == 0x05);
	*st[0].Data(7) = 0x06;
	CHECK(*(uint64_t*)st[0].Data() == 0x0600000000000005);
	*(uint64_t*)st[0].Data() = 0x05;

	st.New(8);
	CHECK(st.Count() == 2);
	*(uint64_t*)st[1].Data() = 0x09;
	CHECK(st[1].Size() == 8);
	CHECK(st[0].Data()[0] == 0x05);
	CHECK(st[1].Data()[0] == 0x09);

	


	st.Insert(1, 8);
	CHECK(st.Count() == 3);
	*(uint64_t*)st[1].Data() = 0x01;
	//uint64_t dbg = st[2].Size();
	CHECK(st[2].Size() == 8);
	CHECK(st[0].Data()[0] == 0x05);
	CHECK(st[1].Data()[0] == 0x01);

	CHECK(st[2].Data()[0] == 0x09);

	st.Insert(1, 8);
	CHECK(st.Count() == 4);
	*(uint64_t*)st[1].Data() = 0x3;
	CHECK(st[3].Size() == 8);
	CHECK(st[0].Data()[0] == 0x05);
	CHECK(st[1].Data()[0] == 0x03);
	CHECK(st[2].Data()[0] == 0x01);
	CHECK(st[3].Data()[0] == 0x09);


	st.Remove(2);
	CHECK(st.Count() == 3);
	CHECK(st[0].Data()[0] == 0x05);
	CHECK(st[1].Data()[0] == 0x03);
	CHECK(st[2].Data()[0] == 0x09);

	////////////////////
	CHECK(tt.Count()-1==st.Count());



	//TEST subsubtable
	InlineSubTable sst(&st, st.New());

	CHECK(!sst.Count());
	sst.New(8);
	CHECK(sst.Count() == 1);
	*(uint64_t*)sst[0].Data() = 0x05;
	CHECK(sst[0].Size() == 8);
	CHECK(sst[0].Data()[0] == 0x05);
	*sst[0].Data(7) = 0x06;
	CHECK(*(uint64_t*)sst[0].Data() == 0x0600000000000005);
	*(uint64_t*)sst[0].Data() = 0x05;

	sst.New(8);
	CHECK(sst.Count() == 2);
	*(uint64_t*)sst[1].Data() = 0x09;
	CHECK(sst[1].Size() == 8);
	CHECK(sst[0].Data()[0] == 0x05);
	CHECK(sst[1].Data()[0] == 0x09);

	//uint64_t dbg = *(uint64_t*)sst[2].Data();


	sst.Insert(1, 8);
	CHECK(sst.Count() == 3);
	*(uint64_t*)sst[1].Data() = 0x01;
	//uint64_t dbg = sst[2].Size();
	CHECK(sst[2].Size() == 8);
	CHECK(sst[0].Data()[0] == 0x05);
	CHECK(sst[1].Data()[0] == 0x01);

	CHECK(sst[2].Data()[0] == 0x09);

	sst.Insert(1, 8);
	CHECK(sst.Count() == 4);
	*(uint64_t*)sst[1].Data() = 0x3;
	CHECK(sst[3].Size() == 8);
	CHECK(sst[0].Data()[0] == 0x05);
	CHECK(sst[1].Data()[0] == 0x03);
	CHECK(sst[2].Data()[0] == 0x01);
	CHECK(sst[3].Data()[0] == 0x09);


	sst.Remove(2);
	CHECK(sst.Count() == 3);
	CHECK(sst[0].Data()[0] == 0x05);
	CHECK(sst[1].Data()[0] == 0x03);
	CHECK(sst[2].Data()[0] == 0x09);
	//TODO:: Test Partitions individually

	CHECK(tt.Count() == st.Count());
	CHECK(st.Count() -1 == sst.Count());

	return true;
}