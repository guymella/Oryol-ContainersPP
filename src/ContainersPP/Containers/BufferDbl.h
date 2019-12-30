#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::BufferDbl
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "ContainersPP/Interfaces/iBuffer.h"

namespace ContainersPP {

class BufferDbl : public iBufferD{
public:
    /// default constructor
    BufferDbl(); 
    /// Copy-construct
    BufferDbl(const BufferDbl& rhs);
    /// move constructor
    BufferDbl(BufferDbl&& rhs);
    /// destructor
    ~BufferDbl();

   

    /// move-assignment
    void operator=(BufferDbl&& rhs);
    /// get capacity in bytes of buffer
    virtual uint64_t Capacity() const override;
    /// get number of free bytes at back
    virtual uint64_t SpareBack() const override;
    /// make room for N more bytes
    virtual void ReserveBack(uint64_t numBytes) override;
    /// get number of free bytes at front
    virtual uint64_t SpareFront() const override;
    /// make room for N more bytes at front
    virtual void ReserveFront(uint64_t numBytes) override;

    /// get number of bytes in buffer
    virtual uint64_t Size() const override;
    /// get read-only pointer to content (throws assert if would return nullptr)
    virtual const uint8_t* Data(uint64_t offset = 0) const override;
    /// get read/write pointer to content (throws assert if would return nullptr)
    virtual uint8_t* Data(uint64_t offset = 0) override;

    using iBlockD::CopyBack;
    /// add uninitialized bytes to Block, return pointer to start
    virtual uint8_t* AddBack(uint64_t numBytes) override;
    using iBlockD::CopyFront;
    /// add uninitialized bytes to front of Block, return pointer to start
    virtual uint8_t* AddFront(uint64_t numBytes) override;
    /// insert uninitialized bytes to Block, return pointer to start
    virtual uint8_t* AddInsert(uint64_t offset, uint64_t numBytes) override;
    /// remove a chunk of data from the Block, return number of bytes removed
    virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override;
    /// clear the Block (deletes content, keeps capacity)
    virtual void Clear() override;

private:
    /// (re-)allocate buffer
    void alloc(uint64_t newCapacity,uint64_t dataoffset = 0, uint64_t bufferOffset = 0);
    /// destroy buffer
    void destroy();
    

    uint64_t start = 0;
    uint64_t size =0;
    uint64_t capacity =0;
    uint8_t* data =0;
};

//------------------------------------------------------------------------------
inline BufferDbl::BufferDbl() : start(0), size(0), capacity(0), data(nullptr) {
    // empty
}

inline BufferDbl::BufferDbl(const BufferDbl& rhs)
{
    copy(rhs.Data(), rhs.Size());
}

//------------------------------------------------------------------------------
inline BufferDbl::BufferDbl(BufferDbl&& rhs) : start(rhs.start), size(rhs.size), capacity(rhs.capacity), data(rhs.data) {
    rhs.start = 0;
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

//------------------------------------------------------------------------------
inline BufferDbl::~BufferDbl() {
    destroy();
}

//------------------------------------------------------------------------------
inline void BufferDbl::alloc(uint64_t newCapacity, uint64_t offset, uint64_t bufferOffset) {
    o_assert_dbg(newCapacity > capacity);
    o_assert_dbg(newCapacity > size + offset + bufferOffset);

    uint8_t* newBuf = (uint8_t*) Oryol::Memory::Alloc((int)newCapacity);
    if (size > 0) {
        o_assert_dbg(data);
        Oryol::Memory::Copy(Data(), newBuf + offset + bufferOffset, (int)size);
    }
    if (data) {
        Oryol::Memory::Free(data);
    }
    data = newBuf;
    capacity = newCapacity;
    size += offset;
    start = bufferOffset;
}

//------------------------------------------------------------------------------
inline void BufferDbl::destroy() {
    if (data) {
        Oryol::Memory::Free(data);
    }
    start = 0;
    data = nullptr;
    size = 0;
    capacity = 0;
}

//------------------------------------------------------------------------------
inline void BufferDbl::operator=(BufferDbl&& rhs) {
    destroy();
    start = rhs.start;
    size = rhs.size;
    capacity = rhs.capacity;
    data = rhs.data;
    rhs.start = 0;
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

//------------------------------------------------------------------------------
inline uint64_t BufferDbl::Size() const {
    return size;
}
//------------------------------------------------------------------------------
inline uint64_t BufferDbl::Capacity() const {
    return capacity;
}

//------------------------------------------------------------------------------
inline uint64_t BufferDbl::SpareBack() const {
    return capacity - size - start;
}

//------------------------------------------------------------------------------
inline void BufferDbl::ReserveBack(uint64_t numBytes) {
    // need to grow?
    if ((size + numBytes) > capacity) {
        const uint64_t newCapacity = size + numBytes;
        alloc((int)newCapacity);
    }
}

inline uint64_t BufferDbl::SpareFront() const
{
    return start;
}

inline void BufferDbl::ReserveFront(uint64_t numBytes)
{
    // need to grow?
    if (numBytes > SpareFront()) {
        const uint64_t newCapacity = Capacity() + numBytes;
        alloc((int)newCapacity,0,SpareFront() + numBytes);
    }
}

//------------------------------------------------------------------------------
inline uint8_t* BufferDbl::AddBack(uint64_t numBytes) {
    ReserveBack(numBytes);
    uint8_t* ptr = data + size;
    size += numBytes;
    return ptr;
}

//------------------------------------------------------------------------------
inline void
BufferDbl::Clear() {
    size = 0;
}

inline uint8_t* BufferDbl::AddFront(uint64_t numBytes)
{
    if(numBytes>SpareBack())
        alloc(Size()+numBytes,numBytes);
    else {
        Oryol::Memory::Move(data, data + numBytes, (int)numBytes);
        size += numBytes;
    }

    return data;
}

inline uint8_t* BufferDbl::AddInsert(uint64_t offset, uint64_t numBytes)
{
    if (offset < Size() - offset && SpareFront() > numBytes) {
        ReserveFront(numBytes);
        start -= numBytes;
        Oryol::Memory::Move(Data(numBytes), Data(), (int)offset);               
    } else {
        ReserveBack(numBytes);
        Oryol::Memory::Move(Data(offset), Data(offset + numBytes), (int)(Size()-offset));
    }

    size += numBytes;
    return Data(offset);
}

//------------------------------------------------------------------------------
inline uint64_t BufferDbl::Remove(uint64_t offset, uint64_t numBytes) {
    o_assert_dbg(offset >= 0);
    o_assert_dbg(numBytes >= 0);
    if (offset >= size) {
        return 0;
    }
    if ((offset + numBytes) >= size) {
        numBytes = size - offset;
    }
    o_assert_dbg((offset + numBytes) <= size);
    o_assert_dbg(numBytes >= 0);
    if (numBytes > 0) {
        uint64_t bytesToMove = size - (offset + numBytes);
        if (bytesToMove > 0) {
            Oryol::Memory::Move(Data(offset + numBytes), Data(offset) , (int)bytesToMove);
        }
        size -= numBytes;
        o_assert_dbg(size >= 0);
    }
    return numBytes;
}

//------------------------------------------------------------------------------
inline const uint8_t* BufferDbl::Data(uint64_t offset) const {
    o_assert(data);
    return data+start+offset;
}

//------------------------------------------------------------------------------
inline uint8_t*
BufferDbl::Data(uint64_t offset) {
    o_assert(data);
    return data + start+offset;
}

} // namespace Oryol
