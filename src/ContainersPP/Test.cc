//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/TypeBuffer.h"
#include "UnitTests/BufferTest.h"
//#include "Core/Containers/Array.h"
//using namespace ContainersPP;
#define CHECK o_assert_dbg
int main(void) {
	
	CHECK(BlockTest<Block>());
	CHECK(BlockTest<Buffer>());
	CHECK(BufferTest<Buffer>());
	CHECK(BlockTest<BufferDbl>());
	CHECK(BufferTest<BufferDbl>());
	CHECK(BufferDblTest<BufferDbl>());
	
	TypeBuffer<int> a;
	CHECK(a.Empty());
	a.AddBack(4);
	CHECK(a.Size() == 4);
	CHECK(a.Data(a.Size()) - a.Data() == 16);


	return 0;
}