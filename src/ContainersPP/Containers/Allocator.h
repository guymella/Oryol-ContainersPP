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
#include "ContainersPP/Interfaces/iAllocator.h"

namespace ContainersPP {

class Allocator : public iAllocator {
public:
    /// default constructor
    Allocator() {};
    /// Copy constructor
    Allocator(const Allocator& rhs) { buffers = rhs.buffers; };
    /// move constructor
    Allocator(Allocator&& rhs) { buffers = std::move(rhs.buffers); };
    /// destructor
    //~Allocator();

    virtual Buffer& operator[](uint64_t index) override { return buffers[index]; };
    virtual const Buffer& operator[](uint64_t index) const override { return buffers[index]; };

    virtual uint64_t New() override;
    virtual uint64_t New(uint64_t newSize) override;

    /// get number of buffers
    virtual uint64_t Count() const override { return buffers.Size(); };
   /* /// return true if empty
    bool Empty() const { return !Count(); };*/
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

struct BufferPtr {
    BufferPtr(Allocator* _allocator, uint64_t bufferId) : allocator(_allocator), BufferId(bufferId) {};
    Allocator* allocator;
    uint64_t BufferId;
    Buffer& get() { return (*allocator)[BufferId]; };
    const Buffer& get() const { return (*allocator)[BufferId]; };
};




class CoAllocator : public iAllocator {
public:
    /// default constructor
    CoAllocator(Allocator* ParentAllocator) : allocator(ParentAllocator) {};
    /// Copy constructor
    CoAllocator(const CoAllocator& rhs);
    /// move constructor
    CoAllocator(CoAllocator&& rhs) { 
        _index = std::move(rhs._index);
        allocator = rhs.allocator;
        rhs.allocator = nullptr;
    };
    /// destructor
    //~Allocator();

    virtual Buffer& operator[](uint64_t index) override { return (*allocator)[_index[index]]; };
    virtual const Buffer& operator[](uint64_t index) const override { return (*allocator)[_index[index]]; };

    virtual uint64_t New() override ;
    virtual uint64_t New(uint64_t newSize) override;

    /// get number of buffers
    virtual uint64_t Count() const override { return _index.Size(); };
    
private:
    Allocator* allocator;
    TypeVector<uint64_t> _index;
};

uint64_t ContainersPP::CoAllocator::New()
{
    uint64_t newID = Count();
    _index.PushBack(allocator->New());
    return newID;
}

inline uint64_t CoAllocator::New(uint64_t newSize)
{
    uint64_t newID = Count();
    _index.PushBack(allocator->New(newSize));
    return newID;
}



} // namespace Oryol
