//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------

//#include "Structures/Containers/TableStruct.h"
#include "ContainersPP/Containers/Table.h"
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