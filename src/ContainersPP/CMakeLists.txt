fips_begin_module(ContainersPP)
    fips_vs_warning_level(3)
	
    fips_dir(Containers)
    fips_files(
        #Array.h        
		#ArrayMap.h
        #Slice.h
        BlockStatic.h
		Block.h
        Buffer.h
		BufferStatic.h
        #HashSet.h
        #KeyValuePair.h
        #Map.h
        #Queue.h
        #Set.h
        #StaticArray.h
        #elementBuffer.h
        #InlineArray.h
		#Trie.h
		#Allocator.h
		#BufferBlock.h
    )

    fips_dir(Interfaces)
    fips_files(
        iArray.h iArray.cpp
        iBlock.h
		iBuffer.h
    )
	
    fips_dir(Types)
    fips_files(
        Types.h
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
        )
    fips_deps(ContainersPP)
fips_end_app()