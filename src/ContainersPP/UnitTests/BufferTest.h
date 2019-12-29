//------------------------------------------------------------------------------
//  BufferTest.cc
//------------------------------------------------------------------------------
//#include "Pre.h"
////#include "UnitTest++/src/UnitTest++.h"
//#include "Core/Assertion.h"
////#include "ContainersPP/Containers/Block.h"
//#include <cstring>
#include "BlockTest.h"

//using namespace ContainersPP;

//#define CHECK o_assert_dbg

template <typename BUFFER>
bool BufferTest() {
    CHECK(BlockTest<BUFFER>());
    BUFFER buf0;
    CHECK(buf0.Size() == 0);
    CHECK(buf0.Empty());
    CHECK(buf0.Capacity() == 0);
    CHECK(buf0.SpareBack() == 0);

    buf0.ReserveBack(12);
    CHECK(buf0.Size() == 0);
    CHECK(buf0.Empty());
    CHECK(buf0.Capacity() == 12);
    CHECK(buf0.SpareBack() == 12);

    static const uint8_t bla[] = { 1, 2, 3, 4, 5, 6, 7 };
    buf0.AddBack(bla, sizeof(bla));
    CHECK(buf0.Size() == 7);
    CHECK(!buf0.Empty());
    CHECK(buf0.Capacity() == 12);
    CHECK(buf0.SpareBack() == 5);
    for (int i = 0; i < int(sizeof(bla)); i++) {
        CHECK(i+1 == buf0.Data()[i]);
    }

    buf0.AddBack(bla, sizeof(bla));
    CHECK(buf0.Size() == 14);
    CHECK(!buf0.Empty());
    CHECK(buf0.Capacity() == 14);
    CHECK(buf0.SpareBack() == 0);
    for (int i = 0; i < int(sizeof(bla)); i++) {
        CHECK(i+1 == buf0.Data()[i]);
        CHECK(i+1 == buf0.Data()[i+sizeof(bla)]);
    }

    BUFFER buf1(std::move(buf0));
    CHECK(buf1.Size() == 14);
    CHECK(!buf1.Empty());
    CHECK(buf1.Capacity() == 14);
    CHECK(buf1.SpareBack() == 0);
    for (int i = 0; i < int(sizeof(bla)); i++) {
        CHECK(i+1 == buf1.Data()[i]);
        CHECK(i+1 == buf1.Data()[i+sizeof(bla)]);
    }
    CHECK(buf0.Empty());
    CHECK(buf0.Capacity() == 0);

    BUFFER buf2;
    buf2 = std::move(buf1);
    CHECK(buf2.Size() == 14);
    CHECK(!buf2.Empty());
    CHECK(buf2.Capacity() == 14);
    CHECK(buf2.SpareBack() == 0);
    for (int i = 0; i < int(sizeof(bla)); i++) {
        CHECK(i+1 == buf2.Data()[i]);
        CHECK(i+1 == buf2.Data()[i+sizeof(bla)]);
    }
    CHECK(buf1.Empty());
    CHECK(buf1.Capacity() == 0);
    uint8_t* ptr = buf2.AddBack(sizeof(bla));
    for (uint8_t c : bla) {
        *ptr++ = c;
    }
    CHECK(ptr == buf2.Data() + 21);
    CHECK(buf2.Size() == 21);
    CHECK(!buf2.Empty());
    CHECK(buf2.Capacity() == 21);
    CHECK(buf2.SpareBack() == 0);
    for (int i = 0; i < int(sizeof(bla)); i++) {
        CHECK(i+1 == buf2.Data()[i]);
        CHECK(i+1 == buf2.Data()[i+sizeof(bla)]);
        CHECK(i+1 == buf2.Data()[i+2*sizeof(bla)]);
    }
    buf2.Clear();
    CHECK(buf2.Size() == 0);
    CHECK(buf2.Empty());
    CHECK(buf2.Capacity() == 21);
    CHECK(buf2.SpareBack() == 21);

    BUFFER buf3;
    const char* str = "Hello wonderful world!";
    buf3.AddBack((const uint8_t*)str, int(std::strlen(str))+1);
    CHECK(0 == buf3.Remove(0, 0));
    CHECK(std::strcmp((const char*)buf3.Data(), str) == 0);
    CHECK(buf3.Size() == 23);
    CHECK(0 == buf3.Remove(128, 10));
    CHECK(std::strcmp((const char*)buf3.Data(), str) == 0);
    CHECK(buf3.Size() == 23);
    CHECK(10 == buf3.Remove(5, 10));
    CHECK(std::strcmp((const char*)buf3.Data(), "Hello world!") == 0);
    CHECK(buf3.Size() == 13);
    CHECK(3 == buf3.Remove(10, 5));
    CHECK(buf3.Size() == 10);
    buf3.AddBack((const uint8_t*)"\0", 1);
    CHECK(std::strcmp((const char*)buf3.Data(), "Hello worl") == 0);
    CHECK(6 == buf3.Remove(0, 6));
    CHECK(buf3.Size() == 5);
    CHECK(std::strcmp((const char*)buf3.Data(), "worl") == 0);

    BUFFER buf4;
    buf4.AddBack((const uint8_t*)str, int(std::strlen(str))+1);
    CHECK(6 == buf4.Remove(0, 6));
    CHECK(std::strcmp((const char*)buf4.Data(), "wonderful world!") == 0);
    return true;
};


template <typename BUFFER>
bool BufferDblTest() {    
    CHECK(BufferTest<BUFFER>());
    //Todo:: add reserve front and spare front tests
    return true;
};