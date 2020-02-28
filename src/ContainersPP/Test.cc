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
#include "UnitTests/ProfileTest.h"


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
	CHECK((TestObject<uint32_t, Types::baseTypes::uint32>()));
	CHECK((TestObject<uint16_t, Types::baseTypes::uint16>()));
	CHECK((TestObject<uint8_t, Types::baseTypes::uint8>()));
	CHECK((TestObject<int64_t, Types::baseTypes::int64>()));
	CHECK((TestObject<int32_t, Types::baseTypes::int32>()));
	CHECK((TestObject<int16_t, Types::baseTypes::int16>()));
	CHECK((TestObject<int8_t, Types::baseTypes::int8>()));
	CHECK((TestObject<float_t, Types::baseTypes::float32>()));
	CHECK((TestObject<double_t, Types::baseTypes::float64>()));
	
	CHECK(TestCatalogue());
	CHECK(TestFileCatalogue());

	CHECK(TestProfile());

	return 0;
}