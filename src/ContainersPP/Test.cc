//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/TypeBuffer.h"


#include "ContainersPP/Containers/Trie.h"
#include "ContainersPP/Containers/Coalator.h"

#include "UnitTests/BufferTest.h"
#include "UnitTests/elementBufferTest.h"
#include "UnitTests/StructTest.h"
#include "UnitTests/TrieTest.h"
#include "UnitTests/TypeTrieTest.h"
#include "UnitTests/TableTest.h"
#include "UnitTests/FileBlockTest.h"
#include "UnitTests/AllocatorTest.h"
#include "UnitTests/FileAllocatorTest.h"
#include "UnitTests/ObjectTest.h"


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
	
	CHECK(elementBufferTest<TypeBuffer<_test>>());
	CHECK(TestInlineTable());
	CHECK(TestStruct());
	CHECK(TestTrie());
	CHECK(TestCatTrie());
	CHECK(TestTypeTrie());
	CHECK(TestIndexTrie());
	CHECK(TestTable());
	
	CHECK(TestFileBlock());
	CHECK(TestAllocator());
	CHECK(TestFileAllocator());
	CHECK(TestObject());
	CHECK(TestCatalogue());
	CHECK(TestFileCatalogue());

	return 0;
}