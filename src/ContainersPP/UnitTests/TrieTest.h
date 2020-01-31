//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
//#include "ContainersPP/Types/schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "ContainersPP/Types/Entity.h"

#include "ContainersPP/Containers/Trie.h"

//#include <assert.h>
//#define CHECK assert

using namespace ContainersPP;

bool TestTrie()
{
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



	Types::KeyString i = Types::MakeKey(0x0102030405060708);
	CHECK(i.Size() == 8);
	CHECK(i.Data()[0] == 1);
	CHECK(i.Data()[7] == 8);
	i = Types::MakeKey(0x01020304050607);
	CHECK(i.Data()[0] == 1);
	CHECK(i.Data()[6] == 7);
	CHECK(i.Size() == 7);
	i = Types::MakeKey(0x04050607);
	CHECK(i.Data()[0] == 4);
	CHECK(i.Data()[3] == 7);
	CHECK(i.Size() == 4);
	i = Types::MakeKey(0x0506);
	CHECK(i.Data()[0] == 5);
	CHECK(i.Data()[1] == 6);
	CHECK(i.Size() == 2);
	i = Types::MakeKey(258);
	CHECK(i.Data()[0] == 1);
	CHECK(i.Data()[1] == 2);
	CHECK(i.Size() == 2);
	i = Types::MakeKey(255);
	CHECK(i.Size() == 1);
	CHECK(i.Data()[0] == 255);
	i = Types::MakeKey(18);
	CHECK(i.Size() == 1);
	CHECK(i.Data()[0] == 18);

	Trie ti;

	//add first key
	CHECK(!ti.Contains(0x100000));
	*(uint64_t*)(ti.GetOrAdd(0x100000).AddBack(sizeof(uint64_t))) = 20;
	CHECK(ti.Contains(0x100000));
	CHECK(*(uint64_t*)(ti.Find(0x100000)->Data()) == 20);

	// add different key
	CHECK(!ti.Contains(0x200000));
	*(uint64_t*)(ti.GetOrAdd(0x200000).AddBack(sizeof(uint64_t))) = 21;
	CHECK(ti.Contains(0x200000));
	CHECK(*(uint64_t*)(ti.Find(0x200000)->Data()) == 21);

	// add extending key
	CHECK(!ti.Contains(0x1000000505));
	*(uint64_t*)(ti.GetOrAdd(0x1000000505).AddBack(sizeof(uint64_t))) = 22;
	CHECK(ti.Contains(0x1000000505));
	CHECK(*(uint64_t*)(ti.Find(0x1000000505)->Data()) == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(*(uint64_t*)(ti.Find(0x100000)->Data()) == 20);

	// add Splitting key
	CHECK(!ti.Contains(0x20));
	*(uint64_t*)(ti.GetOrAdd(0x20).AddBack(sizeof(uint64_t))) = 23;
	CHECK(ti.Contains(0x20));
	CHECK(*(uint64_t*)(ti.Find(0x20)->Data()) == 23);
	CHECK(ti.Contains(0x200000));
	CHECK(*(uint64_t*)(ti.Find(0x200000)->Data()) == 21);

	// split extended key
	CHECK(!ti.Contains(0x1000));
	*(uint64_t*)(ti.GetOrAdd(0x1000).AddBack(sizeof(uint64_t))) = 24;
	CHECK(ti.Contains(0x1000));
	CHECK(*(uint64_t*)(ti.Find(0x1000)->Data()) == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(*(uint64_t*)(ti.Find(0x1000000505)->Data()) == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(*(uint64_t*)(ti.Find(0x100000)->Data()) == 20);

	// split split key
	CHECK(!ti.Contains(0x100000050222));
	*(uint64_t*)(ti.GetOrAdd(0x100000050222).AddBack(sizeof(uint64_t))) = 25;
	CHECK(ti.Contains(0x100000050222));
	CHECK(*(uint64_t*)(ti.Find(0x100000050222)->Data()) == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(*(uint64_t*)(ti.Find(0x1000)->Data()) == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(*(uint64_t*)(ti.Find(0x1000000505)->Data()) == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(*(uint64_t*)(ti.Find(0x100000)->Data()) == 20);

	return true;
}


