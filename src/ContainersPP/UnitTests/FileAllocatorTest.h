//------------------------------------------------------------------------------
//  BufferTest.cc
//------------------------------------------------------------------------------
//#include "Pre.h"
//#include "UnitTest++/src/UnitTest++.h"
//#include "Core/Assertion.h"
//#include "ContainersPP/Containers/Block.h"
//#include <cstring>

#include "ContainersPP/Containers/FileAllocator.h"

using namespace ContainersPP;

#define CHECK o_assert_dbg

///template <typename BLOCK> 
bool TestFileAllocator() {

	//TODO:: Test Allcoator
	FileAllocator a1("C:\\COSMIC_TEST\\0\\");

	CHECK(a1.New(8) == 1);
	CHECK(a1.New(16) == 2);

	//CHECK(a1.Count() == 2);

	a1[1].CopyBack((const uint8_t*)"Hello World!", 13);
	a1.Save(1);

	a1[2].CopyBack((const uint8_t*)"Hello Saved!", 13);
	a1.Save(2);

	a1[4].CopyBack((const uint8_t*)"Crazy Saved!", 13);
	a1.Save(4);

	a1.Delete(2);

	CHECK(a1.New() == 2);
    
    return true;
}
