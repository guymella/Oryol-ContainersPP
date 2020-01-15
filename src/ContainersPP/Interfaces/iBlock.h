#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::iBlock
    @ingroup Interfaces
    @brief interface for memory block
*/
#ifndef _IBLOCK_DEF
#define _IBLOCK_DEF

#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "iCountable.h"
#include <algorithm>


namespace ContainersPP {
    
    class iBlockS : public iCountable { //static block interface
    public:
        /// CopyAssign
        virtual void operator=(const iBlockS& rhs);

        /// get number of bytes in buffer
        virtual uint64_t Size() const override = 0;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) = 0;

    protected:
        /// copy content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0);
    };

    class iBlockD : public iBlockS { //Dynamic block interface
    public:

        /// get number of bytes in buffer
        virtual uint64_t Size() const override = 0;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override = 0;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override = 0;
         
        /// add bytes to Block
        virtual void CopyBack(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes) = 0;
        /// add bytes to front of Block
        virtual void CopyFront(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes) = 0;
        /// copy bytes into Block move contents to make room
        virtual void CopyInsert(uint64_t offset, const uint8_t* data, uint64_t numBytes);
        /// insert uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddInsert(uint64_t offset, uint64_t numBytes) = 0;
        /// overwite bytes inside of Block
        virtual void CopyOver(uint64_t offset, uint64_t numReplace, const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddOver(uint64_t offset, uint64_t numReplace, uint64_t numBytes);
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) = 0;
        /// Reverse (Transposes) the data at the offset
        virtual uint8_t* Reverse(uint64_t offset, uint64_t numBytes);
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() = 0;

    protected:
        /// overwite all content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0) override;
    };

   
    inline void iBlockS::operator=(const iBlockS& rhs)
    {
        copy(rhs.Data(), rhs.Size());
    }
    
    //------------------------------------------------------------------------------
    inline void iBlockS::copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset) {
        // NOTE: it is valid to call copy with numBytes==0
        o_assert_dbg(Data());
        if (numBytes > Size() - offset)
            numBytes = Size() - offset;
        Oryol::Memory::Copy(ptr, Data(offset), (int)numBytes);
    }

    inline void iBlockD::copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset)
    {
        Clear();
        AddBack(offset + numBytes);
        Oryol::Memory::Copy(ptr, Data(offset), (int)numBytes);
    }

    inline void ContainersPP::iBlockD::CopyBack(const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddBack(numBytes), (int)numBytes);
    }
    inline void iBlockD::CopyFront(const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddFront(numBytes), (int)numBytes);
    }
    inline void iBlockD::CopyInsert(uint64_t offset, const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddInsert(offset,numBytes), (int)numBytes);
    }
    inline void iBlockD::CopyOver(uint64_t offset, uint64_t numReplace, const uint8_t* data, uint64_t numBytes)
    {
        Oryol::Memory::Copy(data, AddOver(offset, numReplace,numBytes), (int)numBytes);
    }
    inline uint8_t* iBlockD::AddOver(uint64_t offset, uint64_t numReplace, uint64_t numBytes)
    {
        if (numReplace > numBytes)//remove some
            Remove(offset + numBytes, numReplace - numBytes);
        else //add some
            AddInsert(offset + numReplace, numBytes - numReplace);
        return Data(offset);
    }
    inline uint8_t* iBlockD::Reverse(uint64_t offset, uint64_t numBytes)
    {
        #ifdef DEFENSE
        if (offset > Size())
            return nullptr;
        if (offset + numBytes > Size())
            numBytes = Size() - offset;
        #endif // DEFENSE
       
        std::reverse(Data(offset), Data(offset + numBytes));
        return Data(offset);
    }
} // namespace

#endif