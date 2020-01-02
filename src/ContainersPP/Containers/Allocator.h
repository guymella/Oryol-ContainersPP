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
#include "ContainersPP/Containers/TypeBuffer.h"

namespace ContainersPP {

class Allocator {
public:
    /// default constructor
    Allocator() {};
    /// Copy constructor
    Allocator(const Allocator& rhs) { buffers = rhs.buffers; };
    /// move constructor
    Allocator(Allocator&& rhs) { buffers = std::move(rhs.buffers); };
    /// destructor
    //~Allocator();

    Buffer& operator[](uint64_t index) { return buffers[index]; };
    const Buffer& operator[](uint64_t index) const { return buffers[index]; };

    uint64_t New();
    uint64_t New(uint64_t newSize);

    /// get number of buffers
    uint64_t Count() const { return buffers.Size(); };
    /// return true if empty
    bool Empty() const { return !Count(); };
private:
    TypeVector<Buffer> buffers;
};

uint64_t ContainersPP::Allocator::New()
{
    uint64_t id = Count();
    buffers.PushBack(Buffer());
    return id;
}

inline uint64_t Allocator::New(uint64_t newSize)
{
    uint64_t id = Count();
    buffers.PushBack(Buffer(newSize));
    return id;
}

} // namespace Oryol
