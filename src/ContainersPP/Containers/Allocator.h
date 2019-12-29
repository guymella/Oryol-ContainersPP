#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Allocator
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "ContainersPP/Containers/Array.h"

namespace ContainersPP {

class Allocator {
public:
    /// default constructor
    Allocator();
    /// move constructor
    Allocator(Allocator&& rhs);
    /// destructor
    ~Allocator();

    /// always force move-construct
    //Buffer(const Buffer& rhs) = delete;
    /// always force move-assign
    //void operator=(const Buffer& rhs) = delete;

    /// move-assignment
    //void operator=(Buffer&& rhs);

    /// get number of buffers
    int Count() const;
    /// return true if empty
    bool Empty() const;

    

private:
    Array<_priv::elementBuffer> buffers;

    int size;
    int capacity;
    uint8_t* data;
};

//------------------------------------------------------------------------------
inline
Buffer::Buffer() :
size(0),
capacity(0),
data(nullptr) {
    // empty
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
    this->destroy();
}

//------------------------------------------------------------------------------
inline void
Buffer::alloc(int newCapacity) {
    o_assert_dbg(newCapacity > this->capacity);
    o_assert_dbg(newCapacity > this->size);

    uint8_t* newBuf = (uint8_t*) Memory::Alloc(newCapacity);
    if (this->size > 0) {
        o_assert_dbg(this->data);
        Memory::Copy(this->data, newBuf, this->size);
    }
    if (this->data) {
        Memory::Free(this->data);
    }
    this->data = newBuf;
    this->capacity = newCapacity;
}

//------------------------------------------------------------------------------
inline void
Buffer::destroy() {
    if (this->data) {
        Memory::Free(this->data);
    }
    this->data = nullptr;
    this->size = 0;
    this->capacity = 0;
}

//------------------------------------------------------------------------------
inline void
Buffer::copy(const uint8_t* ptr, int numBytes) {
    // NOTE: it is valid to call copy with numBytes==0
    o_assert_dbg(this->data);
    o_assert_dbg((this->size + numBytes) <= this->capacity);
    Memory::Copy(ptr, this->data + this->size, numBytes);
    this->size += numBytes;
}

//------------------------------------------------------------------------------
inline void
Buffer::operator=(Buffer&& rhs) {
    this->destroy();
    this->size = rhs.size;
    this->capacity = rhs.capacity;
    this->data = rhs.data;
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

//------------------------------------------------------------------------------
inline int
Buffer::Size() const {
    return this->size;
}

//------------------------------------------------------------------------------
inline bool
Buffer::Empty() const {
    return 0 == this->size;
}

//------------------------------------------------------------------------------
inline int
Buffer::Capacity() const {
    return this->capacity;
}

//------------------------------------------------------------------------------
inline int
Buffer::Spare() const {
    return this->capacity - this->size;
}

//------------------------------------------------------------------------------
inline void
Buffer::Reserve(int numBytes) {
    // need to grow?
    if ((this->size + numBytes) > this->capacity) {
        const int newCapacity = this->size + numBytes;
        this->alloc(newCapacity);
    }
}

//------------------------------------------------------------------------------
inline void
Buffer::Add(const uint8_t* data, int numBytes) {
    this->Reserve(numBytes);
    this->copy(data, numBytes);
}

//------------------------------------------------------------------------------
inline uint8_t*
Buffer::Add(int numBytes) {
    this->Reserve(numBytes);
    uint8_t* ptr = this->data + this->size;
    this->size += numBytes;
    return ptr;
}

//------------------------------------------------------------------------------
inline void
Buffer::Clear() {
    this->size = 0;
}

//------------------------------------------------------------------------------
inline int
Buffer::Remove(int offset, int numBytes) {
    o_assert_dbg(offset >= 0);
    o_assert_dbg(numBytes >= 0);
    if (offset >= this->size) {
        return 0;
    }
    if ((offset + numBytes) >= this->size) {
        numBytes = this->size - offset;
    }
    o_assert_dbg((offset + numBytes) <= this->size);
    o_assert_dbg(numBytes >= 0);
    if (numBytes > 0) {
        int bytesToMove = this->size - (offset + numBytes);
        if (bytesToMove > 0) {
            Memory::Move(this->data + offset + numBytes, this->data + offset, bytesToMove);
        }
        this->size -= numBytes;
        o_assert_dbg(this->size >= 0);
    }
    return numBytes;
}

//------------------------------------------------------------------------------
inline const uint8_t*
Buffer::Data() const {
    o_assert(this->data);
    return this->data;
}

//------------------------------------------------------------------------------
inline uint8_t*
Buffer::Data() {
    o_assert(this->data);
    return this->data;
}

} // namespace Oryol
