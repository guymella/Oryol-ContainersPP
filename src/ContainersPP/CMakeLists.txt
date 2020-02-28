fips_begin_module(ContainersPP)
    fips_vs_warning_level(3)
	
    fips_dir(Containers)
    fips_files(
        BlockStatic.h
		Block.h
        Buffer.h
        BufferDbl.h
		TypeBuffer.h
        #Array.h        
		#ArrayMap.h
        #Slice.h        
        #HashSet.h
        #KeyValuePair.h
        #Map.h
        #Queue.h
        #Set.h
        #StaticArray.h
        elementBuffer.h
        #InlineArray.h
        Partition.h Partition.cpp
        Table.h
		Trie.h
        TypeTrie.h
		Allocator.h
        Coalator.h
        Entity.h
        EntityTable.h
        FileBlock.h
        FileAllocator.h
        Object.h Object.cpp
        Catalogue.h Catalogue.cpp
        InlineTable.h InlineTable.cpp
        CatalogueAllocator.h
        PackageAllocator.h
    )

    fips_dir(Interfaces)
    fips_files(
        iCountable.h
        iIndexable.h
        iIterable.h
        iQueue.h
        iArray.h iArray.cpp
        iBlock.h
		iBuffer.h
		iTypeBuffer.h
        iEntity.h
        iEntityTable.h
        iAllocator.h
        iTable.h iTable.cpp
        iTrie.h
        iCatalogueTrie.h
        iIndexTrie.h iIndexTrie.cpp
        iObject.h iObject.cpp
    )
	
    fips_dir(Types)
    fips_files(
        Types.h
        KeyString.h KeyString.cpp
        TypeDescr.h
        Schema.h
        BitPointer.h
    )

    fips_deps(Core)

fips_end_module()

fips_begin_app(test cmdline)
    fips_vs_warning_level(3)	
    fips_files(Test.cc)
    fips_dir(UnitTests)
        fips_files(
            BlockTest.h
            BufferTest.h
            elementBufferTest.h
            StructTest.h
            TrieTest.h
            TypeTrieTest.h
            TableTest.h
            FileBlockTest.h
            AllocatorTest.h
            FileAllocatorTest.h
            ObjectTest.h
            ProfileTest.h
        )
    fips_deps(ContainersPP)
fips_end_app()