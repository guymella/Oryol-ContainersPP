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

    class iBuffer : public iBlockD, public iBufferBack {
    public:
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
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override = 0;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() override = 0;
    protected:
        
    };   

    class iBufferDbl : public iBuffer, public iBufferFront {
    public:
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
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override = 0;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() = 0;
    };
    
    uint64_t ContainersPP::iBuffer::SpareBack() const
    {
        return Capacity() - Size();
    }
    
} // namespace
