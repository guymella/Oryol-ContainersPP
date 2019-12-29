//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "UnitTests/BlockTest.h"
#include "UnitTests/BufferTest.h"
//#include "Core/Containers/Array.h"
//using namespace ContainersPP;
#define CHECK o_assert_dbg
int main(void) {
	
	CHECK(BlockTest<Block>());
	CHECK(BlockTest<Buffer>());
	CHECK(BufferTest<Buffer>());
	//Oryol::Array<uint8_t> a;

	return 0;
}