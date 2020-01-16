//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/TypeBuffer.h"

#include "ContainersPP/Containers/Allocator.h"
#include "ContainersPP/Containers/Trie.h"
#include "ContainersPP/Containers/Coalator.h"

#include "UnitTests/BufferTest.h"
#include "UnitTests/elementBufferTest.h"
#include "UnitTests/StructTest.h"
#include "UnitTests/TrieTest.h"
#include "UnitTests/TypeTrieTest.h"
#include "UnitTests/TableTest.h"



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

	CHECK(TestStruct());
	CHECK(TestTrie());
	CHECK(TestTypeTrie());
	CHECK(TestTable());

	//TODO:: Test Allcoator
	Allocator a1;

	CHECK(a1.New(8) == 0);
	CHECK(a1.New(16) == 1);

	CHECK(a1.Count() == 2);

	CoAllocator c1(&a1);

	CHECK(c1.New(8) == 0);
	CHECK(c1.New(16) == 1);

	CHECK(c1.Count() == 2);
	CHECK(a1.Count() == 4);

	CHECK(c1.New(8) == 2);
	CHECK(c1.New(8) == 3);
	CHECK(c1.New(8) == 4);
	CHECK(c1.New(8) == 5);
	CHECK(c1.New(8) == 6);
	CHECK(c1.New(8) == 7);

	CHECK(c1.Count() == 8);
	CHECK(a1.Count() == 10);
	
	Coalator cc;

	CHECK(cc.Empty());
	cc.Assign(1, 8);	
	CHECK(cc.Count() == 1);
	*(uint64_t*)cc[1].Data() = 100;

	cc.Assign(3, 8);
	CHECK(cc.Count() == 2);
	((uint64_t*)cc[3].Data())[0] = 300;
	CHECK(((uint64_t*)cc[1].Data())[0] == 100);
	CHECK(((uint64_t*)cc[1].Data())[1] == 300);

	cc.Assign(5, 8, 1);
	CHECK(cc.Count() == 3);
	*(uint64_t*)cc[5].Data() = 500;
	//check storage order
	CHECK(((uint64_t*)cc[1].Data())[0] == 100);
	CHECK(((uint64_t*)cc[1].Data())[1] == 500);
	CHECK(((uint64_t*)cc[1].Data())[2] == 300);
	//check lookup order
	CHECK(((uint64_t*)cc[1].Data())[0] == 100);
	CHECK(((uint64_t*)cc[3].Data())[0] == 300);
	CHECK(((uint64_t*)cc[5].Data())[0] == 500);

	//sometimes i impress myself

	return 0;
}