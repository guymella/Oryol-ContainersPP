//------------------------------------------------------------------------------
//  elementBufferTest.cc
//  Test low-level elementBuffer class.
//------------------------------------------------------------------------------
#include "Pre.h"
//#include "UnitTest++/src/UnitTest++.h"
//#include "Core/Containers/elementBuffer.h"

//using namespace Oryol;
//using namespace Oryol::_priv;

class _test
{
public:
    // this class requires that the underlying memory is filled with BB
    // for the default, copy and move constructors!
    _test() {
        //o_assert(ORYOL_MEMORY_DEBUG_INT == this->canary);
        this->canary = 0xABBAABBA;
        this->value  = 0;
    };
    _test(int val) {
        this->canary = 0xABBAABBA;
        this->value  = val;
    };
    _test(const _test& rhs) {
       // o_assert(ORYOL_MEMORY_DEBUG_INT == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        this->canary = 0xABBAABBA;
        this->value  = rhs.value;
    }
    _test(_test&& rhs) {
        // NOTE: rhs must still be in the constructed state after this!
        o_assert(0xABBAABBA == rhs.canary);
        this->canary = 0xABBAABBA;
        this->value = rhs.value;
        rhs.value  = 0;
    }
    ~_test() {
        o_assert(0xABBAABBA == this->canary);
        this->canary = ORYOL_MEMORY_DEBUG_INT;
        this->value  = ORYOL_MEMORY_DEBUG_INT;
    };
    void operator=(int val) {
        o_assert(0xABBAABBA == this->canary);
        this->value = val;
    };
    void operator=(const _test& rhs) {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        this->value = rhs.value;
    };
    void operator=(_test&& rhs) {
        // NOTE: rhs must still be in the constructed state after this!
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        this->value = rhs.value;
        this->canary = 0xABBAABBA;
    };
    bool operator==(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value == rhs.value;
    };
    bool operator!=(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value != rhs.value;
    };
    bool operator<(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value < rhs.value;
    };
    bool operator>(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value > rhs.value;
    };
    bool operator<=(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value <= rhs.value;
    };
    bool operator>=(const _test& rhs) const {
        o_assert(0xABBAABBA == this->canary);
        o_assert(0xABBAABBA == rhs.canary);
        return this->value >= rhs.value;
    };
    
    uint32_t canary;
    int value;
};

//------------------------------------------------------------------------------
template <typename ELEMENTBUFFER>
bool TestMemory(const ELEMENTBUFFER& buf) {
    // test that front-spare is completely destructed
    if (buf.Size()) {
        for (const _test* ptr = buf.begin()-buf.SpareFront(); ptr < buf.begin(); ptr++) {
            if (ptr->canary == 0xABBAABBA ){//!= ORYOL_MEMORY_DEBUG_INT) {
                return false;
            }
        }
        // test that elements are all constructed
        for (const _test* ptr = buf.begin(); ptr < buf.end(); ptr++) {
            if (ptr->canary != 0xABBAABBA) {
                return false;
            }
        }
        // test that back-spare is completely destructed
        for (const _test* ptr = buf.end(); ptr < buf.end()+buf.SpareBack(); ptr++) {
            if (ptr->canary == 0xABBAABBA) {
                return false;
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
template <typename EELEMENTBUFFER>
bool elementBufferTest() {

    EELEMENTBUFFER buf;
    buf.Allocate(128, 64);
    CHECK(TestMemory(buf));
    CHECK(buf.Capacity() == 128);
    CHECK(buf.Size() == 0);
    CHECK(buf.SpareFront() == 64);
    CHECK(buf.SpareBack() == 64);
    
    const _test _0(0);
    const _test _1(1);
    const _test _2(2);
    const _test _3(3);
    const _test _4(4);
    const _test _5(5);
    const _test _6(6);
    const _test _7(7);
    const _test _8(8);
    const _test _9(9);
    const _test _10(10);
    const _test _11(11);
    const _test _12(12);
    const _test _13(13);
    const _test _14(14);
    const _test _15(15);
    const _test _16(16);
    const _test _17(17);
    
    // this should invoke the copy-method
    buf.PushBack(_2);
    buf.PushFront(_1);
    CHECK(TestMemory(buf));
    CHECK(buf.Size() == 2);
    CHECK(buf.SpareFront() == 63);
    CHECK(buf.SpareBack() == 63);
    CHECK(buf[0] == _1);
    CHECK(buf[1] == _2);

    // this should invoke the move-method
    buf.PushBack(_3);
    buf.PushFront(_4);
    CHECK(TestMemory(buf));
    CHECK(buf.Size() == 4);
    CHECK(buf.SpareFront() == 62);
    CHECK(buf.SpareBack() == 62);
    CHECK(buf[0] == _4);
    CHECK(buf[1] == _1);
    CHECK(buf[2] == _2);
    CHECK(buf[3] == _3);
    
    buf.Allocate(256, 64);
    CHECK(TestMemory(buf));
    CHECK(buf.Capacity() == 256);
    CHECK(buf.Size() == 4);
    CHECK(buf.SpareFront() == 64);
    CHECK(buf.SpareBack() == 188);
    CHECK(buf[0] == _4);
    CHECK(buf[1] == _1);
    CHECK(buf[2] == _2);
    CHECK(buf[3] == _3);
    
    //buf.emplaceBack(5);
    //buf.emplaceFront(6);
    buf.PushBack(_5);
    buf.PushFront(_6);
    CHECK(TestMemory(buf));
    CHECK(buf.Capacity() == 256);
    CHECK(buf.Size() == 6);
    CHECK(buf.SpareFront() == 63);
    CHECK(buf.SpareBack() == 187);
    CHECK(buf[0] == _6);
    CHECK(buf[1] == _4);
    CHECK(buf[2] == _1);
    CHECK(buf[3] == _2);
    CHECK(buf[4] == _3);
    CHECK(buf[5] == _5);
    
    // copy constructor
    EELEMENTBUFFER buf1(buf);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Capacity() == 6);
    CHECK(buf1.Size() == 6);
    CHECK(buf1.SpareFront() == 0);
    CHECK(buf1.SpareBack() == 0);
    CHECK(buf1[0] == _6);
    CHECK(buf1[1] == _4);
    CHECK(buf1[2] == _1);
    CHECK(buf1[3] == _2);
    CHECK(buf1[4] == _3);
    CHECK(buf1[5] == _5);
    
    // move constructor
    EELEMENTBUFFER buf2(std::move(buf1));
    CHECK(TestMemory(buf1));
    CHECK(TestMemory(buf2));
    CHECK(buf1.Capacity() == 0);
    CHECK(buf1.Size() == 0);
    CHECK(buf2.Capacity() == 6);
    CHECK(buf2.Size() == 6);
    CHECK(buf2.SpareFront() == 0);
    CHECK(buf2.SpareBack() == 0);
    CHECK(buf2[0] == _6);
    CHECK(buf2[1] == _4);
    CHECK(buf2[2] == _1);
    CHECK(buf2[3] == _2);
    CHECK(buf2[4] == _3);
    CHECK(buf2[5] == _5);
    
    // assign empty array
    buf2 = buf1;
    //CHECK(buf2.Capacity() == 0);
    CHECK(buf2.Size() == 0);
    CHECK(TestMemory(buf1));
    CHECK(TestMemory(buf2));
    
    // copy assignment
    buf2 = buf;
    CHECK(TestMemory(buf));
    CHECK(TestMemory(buf2));
    CHECK(buf2.Size() == 6);
    CHECK(buf2.SpareFront() == 0);
    CHECK(buf2.SpareBack() == 0);
    CHECK(buf2[0] == _6);
    CHECK(buf2[1] == _4);
    CHECK(buf2[2] == _1);
    CHECK(buf2[3] == _2);
    CHECK(buf2[4] == _3);
    CHECK(buf2[5] == _5);

    // move assignment
    buf1 = std::move(buf2);
    CHECK(TestMemory(buf2));
    CHECK(TestMemory(buf1));
    CHECK(buf2.Capacity() == 0);
    CHECK(buf2.Size() == 0);
    CHECK(buf1.Capacity() == 6);
    CHECK(buf1.Size() == 6);
    CHECK(buf1.SpareFront() == 0);
    CHECK(buf1.SpareBack() == 0);
    CHECK(buf1[0] == _6);
    CHECK(buf1[1] == _4);
    CHECK(buf1[2] == _1);
    CHECK(buf1[3] == _2);
    CHECK(buf1[4] == _3);
    CHECK(buf1[5] == _5);
    
    // erase element at front
    buf1.Erase(0);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 5);
    CHECK(buf1[0] == _4);
    CHECK(buf1[1] == _1);
    CHECK(buf1[2] == _2);
    CHECK(buf1[3] == _3);
    CHECK(buf1[4] == _5);
    
    // erase element at back
    buf1.Erase(4);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 4);
    CHECK(buf1[0] == _4);
    CHECK(buf1[1] == _1);
    CHECK(buf1[2] == _2);
    CHECK(buf1[3] == _3);
    
    // erase element near front
    buf1.Erase(1);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 3);
    CHECK(buf1[0] == _4);
    CHECK(buf1[1] == _2);
    CHECK(buf1[2] == _3);
    
    // erase element in the middle
    buf1.Erase(1);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 2);
    CHECK(buf1[0] == _4);
    CHECK(buf1[1] == _3);
    
    // and fully erase all
    buf1.Erase(0);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 1);
    CHECK(buf1[0] == _3);
    buf1.Erase(0);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 0);
   

    // test Insertion
    buf1.Allocate(16, 0);
    for (int i = 0; i < 8; i++) {
        buf1.PushBack(i);
    }
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 8);
    
    // Insert at front
    _test _33(33);
    buf1.Insert(0, 33);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 9);
    CHECK(buf1[0] == _33);
    CHECK(buf1[1] == _0);
    CHECK(buf1[2] == _1);
    CHECK(buf1[8] == _7);
    
    // Insert at end
    _test _34(34);
    buf1.Insert(9, 34);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 10);
    CHECK(buf1[0] == _33);
    CHECK(buf1[1] == _0);
    CHECK(buf1[9] == _34);
    CHECK(buf1[8] == _7);
    
    // Insert near front
    _test _35(35);
    buf1.Insert(2, 35);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 11);
    CHECK(buf1[0] == _33);
    CHECK(buf1[1] == _0);
    CHECK(buf1[2] == _35);
    CHECK(buf1[3] == _1);
    CHECK(buf1[4] == _2);
    CHECK(buf1[9] == _7);
    CHECK(buf1[10] == _34);
    
    // Insert near end
    _test _36(36);
    buf1.Insert(8, 36);
    CHECK(TestMemory(buf1));
    CHECK(buf1.Size() == 12);
    CHECK(buf1[0] == _33);
    CHECK(buf1[1] == _0);
    CHECK(buf1[2] == _35);
    CHECK(buf1[3] == _1);
    CHECK(buf1[4] == _2);
    CHECK(buf1[7] == _5);
    CHECK(buf1[8] == _36);
    CHECK(buf1[9] == _6);
    CHECK(buf1[10] == _7);
    CHECK(buf1[11] == _34);
    
    // allocate a new buffer with 1 free slot at the front
    EELEMENTBUFFER buf3;
    buf3.Allocate(16, 1);
    buf3.PushBack(_1);
    buf3.PushBack(_2);
    buf3.PushBack(_3);
    buf3.PushBack(_4);
    // first Insert at front should use the empty slot
    buf3.Insert(0, _5);
    // second Insert at front should move content towards back
    buf3.Insert(0, _6);
    CHECK(TestMemory(buf3));
    CHECK(buf3.Size() == 6);
    CHECK(buf3[0] == _6);
    CHECK(buf3[1] == _5);
    CHECK(buf3[2] == _1);
    CHECK(buf3[3] == _2);
    CHECK(buf3[4] == _3);
    CHECK(buf3[5] == _4);
    
    // and test the same the other way around
    EELEMENTBUFFER buf4;
    buf4.Allocate(16, 10);
    CHECK(TestMemory(buf4));
    buf4.PushBack(_10);
    buf4.PushBack(_11);
    buf4.PushBack(_12);
    buf4.PushBack(_13);
    buf4.PushBack(_14);
    buf4.PushBack(_15);
    
    // this Insert should move the array towards the front
    buf4.Insert(6, 16);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 7);
    CHECK(buf4[0] == _10);
    CHECK(buf4[1] == _11);
    CHECK(buf4[2] == _12);
    CHECK(buf4[3] == _13);
    CHECK(buf4[4] == _14);
    CHECK(buf4[5] == _15);
    CHECK(buf4[6] == _16);
    
    // check normal erase
    // erase from front-area
    buf4.Erase(1);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 6);
    CHECK(buf4[0] == _10);
    CHECK(buf4[1] == _12);
    CHECK(buf4[2] == _13);
    CHECK(buf4[3] == _14);
    CHECK(buf4[4] == _15);
    CHECK(buf4[5] == _16);   
    // erase from back area
    buf4.Erase(3);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 5);
    CHECK(buf4[0] == _10);
    CHECK(buf4[1] == _12);
    CHECK(buf4[2] == _13);
    CHECK(buf4[3] == _15);
    CHECK(buf4[4] == _16);

    // erase from front
    buf4.Erase(0);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 4);
    CHECK(buf4[0] == _12);
    CHECK(buf4[1] == _13);
    CHECK(buf4[2] == _15);
    CHECK(buf4[3] == _16);
    
    // erase from back
    buf4.Erase(3);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 3);
    CHECK(buf4[0] == _12);
    CHECK(buf4[1] == _13);
    CHECK(buf4[2] == _15);
    
    // and erase the rest
    buf4.Erase(1);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 2);
    CHECK(buf4[0] == _12);
    CHECK(buf4[1] == _15);
    buf4.Erase(1);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 1);
    CHECK(buf4[0] == _12);
    buf4.Erase(0);
    CHECK(TestMemory(buf4));
    CHECK(buf4.Size() == 0);
    
    // eraseSwap tests
    EELEMENTBUFFER buf5;
    buf5.Allocate(8, 0);
    for (int i = 0; i < 8; i++) {
        buf5.PushBack(i);
    }
    CHECK(TestMemory(buf5));
    
    // eraseSwap from front
    buf5.EraseSwap(0);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 7);
    CHECK(buf5[0] == _1);
    CHECK(buf5[1] == _2);
    CHECK(buf5[2] == _3);
    CHECK(buf5[3] == _4);
    CHECK(buf5[4] == _5);
    CHECK(buf5[5] == _6);
    CHECK(buf5[6] == _7);
    
    // eraseSwap from end
    buf5.EraseSwap(6);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 6);
    CHECK(buf5[0] == _1);
    CHECK(buf5[1] == _2);
    CHECK(buf5[2] == _3);
    CHECK(buf5[3] == _4);
    CHECK(buf5[4] == _5);
    CHECK(buf5[5] == _6);
    
    // eraseSwap from near front
    buf5.EraseSwap(2);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 5);
    CHECK(buf5[0] == _2);
    CHECK(buf5[1] == _1);
    CHECK(buf5[2] == _4);
    CHECK(buf5[3] == _5);
    CHECK(buf5[4] == _6);
    buf5.PushBack(7);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 6);
    CHECK(buf5[0] == _2);
    CHECK(buf5[1] == _1);
    CHECK(buf5[2] == _4);
    CHECK(buf5[3] == _5);
    CHECK(buf5[4] == _6);
    CHECK(buf5[5] == _7);
    
    // eraseSwap from near back
    buf5.EraseSwap(3);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 5);
    CHECK(buf5[0] == _2);
    CHECK(buf5[1] == _1);
    CHECK(buf5[2] == _4);
    CHECK(buf5[3] == _7);
    CHECK(buf5[4] == _6);
    
    // test eraseSwapBack
    buf5.EraseSwapBack(1);
    CHECK(TestMemory(buf5));
    CHECK(buf5.Size() == 4);
    CHECK(buf5[0] == _2);
    CHECK(buf5[1] == _6);
    CHECK(buf5[2] == _4);
    CHECK(buf5[3] == _7);
    buf5.EraseSwapBack(3);
    CHECK(buf5.Size() == 3);
    CHECK(buf5[0] == _2);
    CHECK(buf5[1] == _6);
    CHECK(buf5[2] == _4);
    
    // test eraseSwapFront
    buf5.EraseSwapFront(2);
    CHECK(buf5.Size() == 2);
    CHECK(buf5[0] == _6);
    CHECK(buf5[1] == _2);
    buf5.EraseSwapFront(0);
    CHECK(buf5.Size() == 1);
    CHECK(buf5[0] == _2);
    
    // test PopFront/PopBack
    buf5.Clear();
    buf5.PushBack(1);
    buf5.PushBack(2);
    buf5.PushBack(3);
    buf5.PushBack(4);
    CHECK(buf5.PopFront() == 1);
    CHECK(buf5.Size() == 3);
    CHECK(buf5[0] == 2);
    CHECK(buf5[1] == 3);
    CHECK(buf5[2] == 4);
    CHECK(buf5.PopBack() == 4);
    CHECK(buf5.Size() == 2);
    CHECK(buf5[0] == 2);
    CHECK(buf5[1] == 3);
    CHECK(buf5.PopFront() == 2);
    CHECK(buf5.Size() == 1);
    CHECK(buf5[0] == 3);
    CHECK(buf5.PopBack() == 3);
    CHECK(buf5.Size() == 0);

    // eraseRange
    EELEMENTBUFFER buf6;
    buf6.Allocate(64, 0);
    for (int i = 0; i < 16; i++) {
        buf6.PushBack(i);
    }
    CHECK(buf6.Size() == 16);
    buf6.EraseRange(0, 3);
    CHECK(buf6.Size() == 13);
    CHECK(buf6[0] == 3);
    CHECK(buf6[1] == 4);
    CHECK(TestMemory(buf6));
    buf6.EraseRange(2, 3);
    CHECK(buf6.Size() == 10);
    CHECK(buf6[0] == 3);
    CHECK(buf6[1] == 4);
    CHECK(buf6[2] == 8);
    CHECK(buf6[3] == 9);
    CHECK(TestMemory(buf6));
    buf6.EraseRange(7, 3);
    CHECK(buf6.Size() == 7);
    CHECK(buf6[0] == 3);
    CHECK(buf6[1] == 4);
    CHECK(buf6[2] == 8);
    CHECK(buf6[3] == 9);
    CHECK(buf6[6] == 12);
    CHECK(TestMemory(buf6));

    return true;
}
