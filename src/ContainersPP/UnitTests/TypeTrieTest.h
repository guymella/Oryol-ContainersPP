//------------------------------------------------------------------------------
//  TableTest.h
//  Test Table class.
//------------------------------------------------------------------------------
//#include "ContainersPP/Types/schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "ContainersPP/Types/Entity.h"

#include "ContainersPP/Containers/TypeTrie.h"
#include "ContainersPP/Containers/TypeTrie.h"

//#include <assert.h>
//#define CHECK assert

using namespace ContainersPP;

bool TestTypeTrie()
{
	//TODO:: Test Trie
	TypeTrie<uint64_t> t;

	//add first key
	CHECK(!t.Contains("hello"));
	t.GetOrAdd("hello") = 20;
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")[0] == 20);

	// add different key
	CHECK(!t.Contains("supdude"));
	t.GetOrAdd("supdude") = 21;
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")[0] == 21);

	// add extending key
	CHECK(!t.Contains("helloworld"));
	t.GetOrAdd("helloworld") = 22;
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")[0] == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")[0] == 20);

	// add Splitting key
	CHECK(!t.Contains("sup"));
	t.GetOrAdd("sup") = 23;
	CHECK(t.Contains("sup"));
	CHECK(t.Find("sup")[0] == 23);
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")[0] == 21);

	// split extended key
	CHECK(!t.Contains("hell"));
	t.GetOrAdd("hell") = 24;
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")[0] == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")[0] == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")[0] == 20);

	// split split key
	CHECK(!t.Contains("hellowombats"));
	t.GetOrAdd("hellowombats") = 25;
	CHECK(t.Contains("hellowombats"));
	CHECK(t.Find("hellowombats")[0] == 25);
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")[0] == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")[0] == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")[0] == 20);



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

	TypeTrie<uint64_t> ti;

	//add first key
	CHECK(!ti.Contains(0x100000));
	ti.GetOrAdd(0x100000) = 20;
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)[0] == 20);

	// add different key
	CHECK(!ti.Contains(0x200000));
	ti.GetOrAdd(0x200000) = 21;
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)[0] == 21);

	// add extending key
	CHECK(!ti.Contains(0x1000000505));
	ti.GetOrAdd(0x1000000505) = 22;
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)[0] == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)[0] == 20);

	// add Splitting key
	CHECK(!ti.Contains(0x20));
	ti.GetOrAdd(0x20) = 23;
	CHECK(ti.Contains(0x20));
	CHECK(ti.Find(0x20)[0] == 23);
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)[0] == 21);

	// split extended key
	CHECK(!ti.Contains(0x1000));
	ti.GetOrAdd(0x1000) = 24;
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)[0] == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)[0] == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)[0] == 20);

	// split split key
	CHECK(!ti.Contains(0x100000050222));
	ti.GetOrAdd(0x100000050222) = 25;
	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)[0] == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)[0] == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)[0] == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)[0] == 20);


	//TEST Erase;
	ti.Erase(0x1000000505);

	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)[0] == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)[0] == 24);
	CHECK(!ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)==nullptr);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)[0] == 20);

	//TODO:: test erase value from non leaf node.
	
	return true;
}


bool TestCatTrie()
{
	//TODO:: Test Trie
	CatTrie t;

	////add first key
	CHECK(!t.Contains("hello"));	
	CatKey* p = t.Add("hello", CatValEnums::prm, { Types::uint64,20 });
	CHECK(p->Value()->uint_64 == 20);
	CHECK(t.Contains("hello"));	
	CHECK(t.Find("hello")->Value()->uint_64 == 20);

	// add different key
	CHECK(!t.Contains("supdude"));
	t.Add("supdude", CatValEnums::prm, { Types::uint64,21 });
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")->Value()->uint_64 == 21);

	//// add extending key
	CHECK(!t.Contains("helloworld"));
	t.Add("helloworld", CatValEnums::prm, { Types::uint64,22 }) ;
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value()->uint_64 == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value()->uint_64 == 20);

	//// add Splitting key
	CHECK(!t.Contains("sup"));
	t.Add("sup", CatValEnums::prm, { Types::uint64,23 });
	CHECK(t.Contains("sup"));
	CHECK(t.Find("sup")->Value()->uint_64 == 23);
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")->Value()->uint_64 == 21);

	// split extended key
	CHECK(!t.Contains("hell"));
	t.Add("hell", CatValEnums::prm, { Types::uint64,24 });
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")->Value()->uint_64 == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value()->uint_64 == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value()->uint_64 == 20);

	// split split key
	CHECK(!t.Contains("hellowombats"));
	t.Add("hellowombats", CatValEnums::prm, { Types::uint64,25 });
	CHECK(t.Contains("hellowombats"));
	CHECK(t.Find("hellowombats")->Value()->uint_64 == 25);
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")->Value()->uint_64 == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value()->uint_64 == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value()->uint_64 == 20);





	CatTrie ti;

	//add first key
	CHECK(!ti.Contains(0x100000));
	ti.Add(0x100000, CatValEnums::prm, { Types::uint64,20 });
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()->uint_64 == 20);

	// add different key
	CHECK(!ti.Contains(0x200000));
	ti.Add(0x200000, CatValEnums::prm, { Types::uint64,21 });
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)->Value()->uint_64 == 21);

	// add extending key
	CHECK(!ti.Contains(0x1000000505));
	ti.Add(0x1000000505, CatValEnums::prm, { Types::uint64,22 });
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value()->uint_64 == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()->uint_64 == 20);

	// add Splitting key
	CHECK(!ti.Contains(0x20));
	ti.Add(0x20, CatValEnums::prm, { Types::uint64,23 });
	CHECK(ti.Contains(0x20));
	CHECK(ti.Find(0x20)->Value()->uint_64 == 23);
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)->Value()->uint_64 == 21);

	// split extended key
	CHECK(!ti.Contains(0x1000));
	ti.Add(0x1000, CatValEnums::prm, { Types::uint64,24 });
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value()->uint_64 == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value()->uint_64 == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()->uint_64 == 20);

	// split split key
	CHECK(!ti.Contains(0x100000050222));
	ti.Add(0x100000050222, CatValEnums::prm, { Types::uint64,25 });
	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)->Value()->uint_64 == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value()->uint_64 == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value()->uint_64 == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()->uint_64 == 20);


	//TEST Erase;
	ti.Erase(0x1000000505);

	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)->Value()->uint_64 == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value()->uint_64 == 24);
	CHECK(!ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505) == nullptr);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()->uint_64 == 20);

	//TODO:: test erase value from non leaf node.

	return true;
}


bool TestIndexTrie()
{
	//TODO:: Test Trie
	IndexTrie t;

	////add first key
	CHECK(!t.Contains("hello"));
	t.Get("hello").SetValue(20);
	CHECK(t.Get("hello").Key().Value() == 20);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value() == 20);

	//// add different key
	CHECK(!t.Contains("supdude"));
	t.Get("supdude").SetValue(21);
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")->Value() == 21);

	////// add extending key
	CHECK(!t.Contains("helloworld"));
	//t.
	t.Get("helloworld").SetValue(22);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value() == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value() == 20);

	////// add Splitting key
	CHECK(!t.Contains("sup"));
	t.Get("sup").SetValue(23);
	CHECK(t.Contains("sup"));
	CHECK(t.Find("sup")->Value() == 23);
	CHECK(t.Contains("supdude"));
	CHECK(t.Find("supdude")->Value() == 21);

	// split extended key
	CHECK(!t.Contains("hell"));
	t.Get("hell").SetValue(24);
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")->Value() == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value() == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value() == 20);

	// split split key
	CHECK(!t.Contains("hellowombats"));
	t.Get("hellowombats").SetValue(25);
	CHECK(t.Contains("hellowombats"));
	CHECK(t.Find("hellowombats")->Value() == 25);
	CHECK(t.Contains("hell"));
	CHECK(t.Find("hell")->Value() == 24);
	CHECK(t.Contains("helloworld"));
	CHECK(t.Find("helloworld")->Value() == 22);
	CHECK(t.Contains("hello"));
	CHECK(t.Find("hello")->Value() == 20);





	IndexTrie ti;

	//add first key
	CHECK(!ti.Contains(0x100000));
	ti.Get(0x100000).SetValue(20);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value()== 20);

	// add different key
	CHECK(!ti.Contains(0x200000));
	ti.Get(0x200000).SetValue(21);
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)->Value() == 21);

	// add extending key
	CHECK(!ti.Contains(0x1000000505));
	ti.Get(0x1000000505).SetValue(22);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value() == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value() == 20);

	// add Splitting key
	CHECK(!ti.Contains(0x20));
	ti.Get(0x20).SetValue(23);
	CHECK(ti.Contains(0x20));
	CHECK(ti.Find(0x20)->Value()== 23);
	CHECK(ti.Contains(0x200000));
	CHECK(ti.Find(0x200000)->Value()== 21);

	// split extended key
	CHECK(!ti.Contains(0x1000));
	ti.Get(0x1000).SetValue(24);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value()== 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value() == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value() == 20);

	// split split key
	CHECK(!ti.Contains(0x100000050222));
	ti.Get(0x100000050222).SetValue(25);
	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)->Value() == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value() == 24);
	CHECK(ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505)->Value() == 22);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value() == 20);


	//TEST Erase;
	ti.Erase(0x1000000505);

	CHECK(ti.Contains(0x100000050222));
	CHECK(ti.Find(0x100000050222)->Value() == 25);
	CHECK(ti.Contains(0x1000));
	CHECK(ti.Find(0x1000)->Value() == 24);
	CHECK(!ti.Contains(0x1000000505));
	CHECK(ti.Find(0x1000000505) == nullptr);
	CHECK(ti.Contains(0x100000));
	CHECK(ti.Find(0x100000)->Value() == 20);

	////TODO:: test erase value from non leaf node.

	return true;
}