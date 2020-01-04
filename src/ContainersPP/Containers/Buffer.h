#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Buffer
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "ContainersPP/Interfaces/iBuffer.h"

namespace ContainersPP {

class Buffer : public iBufferV{
public:
    /// default constructor
    Buffer(); 
    /// default constructor
    Buffer(uint64_t newCapacity);
    /// Copy-construct
    Buffer(const Buffer& rhs);
    /// move constructor
    Buffer(Buffer&& rhs);
    /// destructor
    ~Buffer();

    /// Force Allocate the buffer
    virtual void Allocate(uint64_t newCapacity);
    /// move-assignment
    virtual void operator=(const Buffer& rhs);
    /// move-assignment
    void operator=(Buffer&& rhs);
    /// get capacity in bytes of buffer
    virtual uint64_t Capacity() const override;
    /// get number of free bytes at back
    virtual uint64_t SpareBack() const override;
    /// make room for N more bytes
    virtual void ReserveBack(uint64_t numBytes) override;

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
    void alloc(uint64_t newCapacity,uint64_t offset = 0);
    /// destroy buffer
    void destroy();    

    uint64_t size = 0;
    uint64_t capacity = 0;
    uint8_t* data = 0;
};

//------------------------------------------------------------------------------
inline
Buffer::Buffer() :
size(0),
capacity(0),
data(nullptr) {
    // empty
}

inline Buffer::Buffer(uint64_t newCapacity)
{
    alloc(newCapacity);
}

inline Buffer::Buffer(const Buffer& rhs)
{
    copy(rhs.Data(), rhs.Size());
}

//------------------------------------------------------------------------------
inline
Buffer::Buffer(Buffer&& rhs) :
size(rhs.size),
capacity(rhs.capacity),
data(rhs.data) {
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

//------------------------------------------------------------------------------
inline
Buffer::~Buffer() {
    destroy();
}

inline void Buffer::Allocate(uint64_t newCapacity)
{
    alloc(newCapacity);
}

inline void Buffer::operator=(const Buffer& rhs)
{
    copy(rhs.Data(), rhs.Size());
}

//------------------------------------------------------------------------------
inline void Buffer::alloc(uint64_t newCapacity, uint64_t offset) {
    o_assert_dbg(newCapacity > capacity);
    o_assert_dbg(newCapacity > size + offset);

    uint8_t* newBuf = (uint8_t*) Oryol::Memory::Alloc((int)newCapacity);
    Oryol::Memory::Fill(newBuf, (int)newCapacity, 0);
    if (size > 0) {
        o_assert_dbg(data);
        Oryol::Memory::Copy(data, newBuf + offset, (int)size);
    }
    if (data) {
        Oryol::Memory::Free(data);
    }
    data = newBuf;
    capacity = newCapacity;
    size += offset;
}

//------------------------------------------------------------------------------
inline void Buffer::destroy() {
    if (data) {
        Oryol::Memory::Free(data);
    }
    data = nullptr;
    size = 0;
    capacity = 0;
}

//------------------------------------------------------------------------------
inline void Buffer::operator=(Buffer&& rhs) {
    destroy();
    size = rhs.size;
    capacity = rhs.capacity;
    data = rhs.data;
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

//------------------------------------------------------------------------------
inline uint64_t Buffer::Size() const {
    return size;
}
//------------------------------------------------------------------------------
inline uint64_t Buffer::Capacity() const {
    return capacity;
}

//------------------------------------------------------------------------------
inline uint64_t Buffer::SpareBack() const {
    return capacity - size;
}

//------------------------------------------------------------------------------
inline void Buffer::ReserveBack(uint64_t numBytes) {
    // need to grow?
    if ((size + numBytes) > capacity) {
        const uint64_t newCapacity = size + numBytes;
        alloc((int)newCapacity);
    }
}

//------------------------------------------------------------------------------
inline uint8_t* Buffer::AddBack(uint64_t numBytes) {
    ReserveBack(numBytes);
    uint8_t* ptr = data + size;
    size += numBytes;
    return ptr;
}

//------------------------------------------------------------------------------
inline void
Buffer::Clear() {
    size = 0;
}

inline uint8_t* Buffer::AddFront(uint64_t numBytes)
{
    if(numBytes>SpareBack())
        alloc(Size()+numBytes,numBytes);
    else {
        Oryol::Memory::Move(data, data + numBytes, (int)numBytes);
        size += numBytes;
    }

    return data;
}

inline uint8_t* Buffer::AddInsert(uint64_t offset, uint64_t numBytes)
{
    ReserveBack(numBytes);
    Oryol::Memory::Move(Data(offset), Data(offset + numBytes), (int)(Size() - offset));
    Oryol::Memory::Fill(Data(offset), (int)numBytes, 0);
    size += numBytes;
    return Data(offset);
}

//------------------------------------------------------------------------------
inline uint64_t Buffer::Remove(uint64_t offset, uint64_t numBytes) {
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
            Oryol::Memory::Move(data + offset + numBytes, data + offset, (int)bytesToMove);
        }
        size -= numBytes;
        o_assert_dbg(size >= 0);
    }
    return numBytes;
}

//------------------------------------------------------------------------------
inline const uint8_t* Buffer::Data(uint64_t offset) const {
    o_assert(data);
    return data+offset;
}

//------------------------------------------------------------------------------
inline uint8_t*
Buffer::Data(uint64_t offset) {
    o_assert(data);
    return data + offset;
}

} // namespace Oryol
