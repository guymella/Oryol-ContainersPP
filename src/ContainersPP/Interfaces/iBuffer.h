#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::iBlock
    @ingroup Interfaces
    @brief interface for memory block
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "iBlock.h"


namespace ContainersPP {

    class iBufferFront {
    public:
        /// get number of free bytes at back
        virtual uint64_t SpareFront() const = 0;
        /// make room for N more bytes
        virtual void ReserveFront(uint64_t numBytes) = 0;
    };

    class iBufferBack {
    public:
        /// get number of free bytes at back
        virtual uint64_t SpareBack() const = 0;
        /// make room for N more bytes
        virtual void ReserveBack(uint64_t numBytes) = 0;
    };

    class iBufferV : public iBlockD, public iBufferBack { //single ended buffer interface (vector)
    public:
        /// Force Allocate the buffer
        virtual void Allocate(uint64_t newCapacity) = 0;

        /// get capacity in bytes of buffer
        virtual uint64_t Capacity() const = 0;
        /// get number of free bytes at back
        virtual uint64_t SpareBack() const override;
        /// make room for N more bytes
        virtual void ReserveBack(uint64_t numBytes) override = 0;

        /// get number of bytes in buffer
        virtual uint64_t Size() const override = 0;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override = 0;

        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes) override = 0;
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes) override = 0;
        /// insert uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddInsert(uint64_t offset, uint64_t numBytes) override = 0;
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override = 0;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() override = 0;
    protected:
        
    };   

    class iBufferD : public iBufferV, public iBufferFront { //double endded buffer interface (Buffer)
    public:
        /// Force Allocate the buffer
        virtual void Allocate(uint64_t newCapacity) override = 0;
        /// Force Allocate the buffer
        virtual void Allocate(uint64_t newCapacity, uint64_t frontSpare) = 0;

        /// get capacity in bytes of buffer
        virtual uint64_t Capacity() const = 0;
        
         /// get number of free bytes at back
        virtual uint64_t SpareBack() const override = 0;
        /// make room for N more bytes at back
        virtual void ReserveBack(uint64_t numBytes) override = 0;
        /// get number of free bytes at front
        virtual uint64_t SpareFront() const override = 0;
        /// make room for N more bytes at front
        virtual void ReserveFront(uint64_t numBytes) override = 0;

        /// get number of bytes in buffer
        virtual uint64_t Size() const override = 0;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override = 0;

        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes) override = 0;
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes) override = 0;
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override = 0;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() = 0;
    };
    
    uint64_t ContainersPP::iBufferV::SpareBack() const
    {
        return Capacity() - Size();
    }
    
} // namespace
