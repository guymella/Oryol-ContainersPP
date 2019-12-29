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


namespace ContainersPP {

    class iBlock {
    public:
        /// CopyAssign
        virtual void operator=(const iBlock& rhs);

        /// get number of bytes in buffer
        virtual uint64_t Size() const = 0;
        /// return true if empty
        virtual bool Empty() const;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) = 0;

    protected:
        /// copy content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0);
    };

    class iBlockD : public iBlock {
    public:

        /// get number of bytes in buffer
        virtual uint64_t Size() const override = 0;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override = 0;

        /// add bytes to Block
        virtual void AddBack(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes) = 0;
        /// add bytes to front of Block
        virtual void AddFront(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes) = 0;
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) = 0;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() = 0;

    protected:
        /// copy content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0) override;
    };

   
    inline void iBlock::operator=(const iBlock& rhs)
    {
        copy(rhs.Data(), rhs.Size());
    }
    
    //------------------------------------------------------------------------------
    inline void iBlock::copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset) {
        // NOTE: it is valid to call copy with numBytes==0
        o_assert_dbg(Data());
        if (numBytes > Size() - offset)
            numBytes = Size() - offset;
        Oryol::Memory::Copy(ptr, Data(offset), (int)numBytes);
    }

    //------------------------------------------------------------------------------
    inline bool iBlock::Empty() const {
        return !Size();
    }    

    inline void iBlockD::copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset)
    {
        Clear();
        AddBack(offset + numBytes);
        Oryol::Memory::Copy(ptr, Data(offset), (int)numBytes);
    }

    void ContainersPP::iBlockD::AddBack(const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddBack(numBytes), (int)numBytes);
    }
    inline void iBlockD::AddFront(const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddFront(numBytes), (int)numBytes);
    }
} // namespace
