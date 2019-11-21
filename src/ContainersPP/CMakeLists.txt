fips_begin_module(ContainersPP)
    fips_vs_warning_level(3)
	
    fips_dir(Containers)
    fips_files(
        Array.h        
		ArrayMap.h
        Slice.h
        Buffer.h
        HashSet.h
        KeyValuePair.h
        Map.h
        Queue.h
        Set.h
        StaticArray.h
        elementBuffer.h
        InlineArray.h
    )

    fips_dir(interfaces)
    fips_files(
        iArray.h 
		iArray.cpp
    )

    

fips_end_module()

fips_begin_app(test cmdline)
    fips_vs_warning_level(3)	
    fips_files(Test.cc)
fips_end_app()