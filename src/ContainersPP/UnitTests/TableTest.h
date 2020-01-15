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
	uint64_t x = *(uint64_t*)tt[0].Data();
	*tt[0].Data(7) = 0x06;
	uint64_t xx = *(uint64_t*)tt[0].Data();
	CHECK(*(uint64_t*)tt[0].Data() == 0x0600000000000005);

	//TODO:: Test Blocks individually
	

	return true;
}