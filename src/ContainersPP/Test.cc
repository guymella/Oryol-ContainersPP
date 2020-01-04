//------------------------------------------------------------------------------
//  Test.cc
//------------------------------------------------------------------------------

#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/TypeBuffer.h"

#include "ContainersPP/Containers/Allocator.h"
#include "ContainersPP/Containers/Trie.h"

#include "UnitTests/BufferTest.h"
#include "UnitTests/elementBufferTest.h"
#include "UnitTests/StructTest.h"

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


	CHECK(TestStruct());

	//TODO:: Test Allcoator
	Allocator a1;


	//TODO:: Test Trie
	Trie t;
	
	//add first key
	CHECK(!t.Contains("hello"));
	*(uint64_t*)(t.GetOrAdd("hello").AddBack(sizeof(uint64_t))) = 20;
	CHECK(t.Contains("hello"));
	CHECK(*(uint64_t*)(t.Find("hello")->Data()) == 20);

	// add different key
	CHECK(!t.Contains("supdude"));
	*(uint64_t*)(t.GetOrAdd("supdude").AddBack(sizeof(uint64_t))) = 21;
	CHECK(t.Contains("supdude"));
	CHECK(*(uint64_t*)(t.Find("supdude")->Data()) == 21);

	// add extending key
	CHECK(!t.Contains("helloworld"));
	*(uint64_t*)(t.GetOrAdd("helloworld").AddBack(sizeof(uint64_t))) = 22;
	CHECK(t.Contains("helloworld"));
	CHECK(*(uint64_t*)(t.Find("helloworld")->Data()) == 22);
	CHECK(t.Contains("hello"));
	CHECK(*(uint64_t*)(t.Find("hello")->Data()) == 20);
	
	// add Splitting key
	CHECK(!t.Contains("sup"));
	*(uint64_t*)(t.GetOrAdd("sup").AddBack(sizeof(uint64_t))) = 23;
	CHECK(t.Contains("sup"));
	CHECK(*(uint64_t*)(t.Find("sup")->Data()) == 23);
	CHECK(t.Contains("supdude"));
	CHECK(*(uint64_t*)(t.Find("supdude")->Data()) == 21);

	// split extended key
	CHECK(!t.Contains("hell"));
	*(uint64_t*)(t.GetOrAdd("hell").AddBack(sizeof(uint64_t))) = 24;
	CHECK(t.Contains("hell"));
	CHECK(*(uint64_t*)(t.Find("hell")->Data()) == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(*(uint64_t*)(t.Find("helloworld")->Data()) == 22);
	CHECK(t.Contains("hello"));
	CHECK(*(uint64_t*)(t.Find("hello")->Data()) == 20);

	// split split key
	CHECK(!t.Contains("hellowombats"));
	*(uint64_t*)(t.GetOrAdd("hellowombats").AddBack(sizeof(uint64_t))) = 25;
	CHECK(t.Contains("hellowombats"));
	CHECK(*(uint64_t*)(t.Find("hellowombats")->Data()) == 25);
	CHECK(t.Contains("hell"));
	CHECK(*(uint64_t*)(t.Find("hell")->Data()) == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(*(uint64_t*)(t.Find("helloworld")->Data()) == 22);
	CHECK(t.Contains("hello"));
	CHECK(*(uint64_t*)(t.Find("hello")->Data()) == 20);

	return 0;
}