//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/TypeBuffer.h"
#include "UnitTests/BufferTest.h"
#include "UnitTests/elementBufferTest.h"
//#include "Core/Containers/Array.h"
//using namespace ContainersPP;
#define CHECK o_assert_dbg
int main(void) {
	//TODO:: test insert, overwrite, addfront
	CHECK(BlockTest<Block>());
	CHECK(BlockTest<Buffer>());
	CHECK(BufferTest<Buffer>());
	CHECK(BlockTest<BufferDbl>());
	CHECK(BufferTest<BufferDbl>());
	CHECK(BufferDblTest<BufferDbl>());
	
	//TODO:: Test TypeBuffers
	TypeBuffer<int> a;
	CHECK(a.Empty());
	a.AddBack(4);
	CHECK(a.Size() == 4);
	CHECK(a.Data(a.Size()) - a.Data() == 16);

	CHECK(elementBufferTest<TypeBuffer<_test>>());

	return 0;
}