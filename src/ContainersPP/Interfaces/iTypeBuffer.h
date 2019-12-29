#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::iTypeBuffer
    @ingroup Interfaces
    @brief interface for Type specific Buffer
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "ContainersPP/Interfaces/iBuffer.h"

namespace ContainersPP {
    class iSizeBlock {
    public:
        /// CopyAssign
        virtual void operator=(const iSizeBlock& rhs);

        /// get number of bytes in buffer
        virtual uint64_t Size() const;
        /// return true if empty
        virtual bool Empty() const;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0);

        /// add bytes to Block
        virtual void CopyBack(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numElements);
        /// add bytes to front of Block
        virtual void CopyFront(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numElements);
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numElements);
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear();
    protected:
        /// copy content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0);

        virtual uint64_t UnitSize() const = 0;
        virtual iBlockD& Buffer() = 0;
        virtual const iBlockD& Buffer() const = 0;
    };

    class iSizeVector : public iSizeBlock {
    public:
        /// get capacity in bytes of buffer
        virtual uint64_t Capacity() const;
        /// get number of free bytes at back
        virtual uint64_t SpareBack() const;
        /// make room for N more bytes
        virtual void ReserveBack(uint64_t numElements); 
    protected:
        virtual iBuffer& Buffer() override = 0;
        virtual const iBuffer& Buffer() const override = 0;
    };

    class iSizeBuffer : public iSizeVector {
    public:
        /// get number of free bytes at front
        virtual uint64_t SpareFront() const;
        /// make room for N more bytes at front
        virtual void ReserveFront(uint64_t numElements); 
    protected:
        virtual iBufferDbl& Buffer() override = 0;
        virtual const iBufferDbl& Buffer() const override = 0;
    };

    template<typename TYPE>
    class iTypeBlock : public iSizeBlock {
    protected:
        virtual uint64_t UnitSize() const override;        
    };
    template<typename TYPE>
    class iTypeVector : public iSizeVector {
    protected:
        virtual uint64_t UnitSize() const override;
    };

    template<typename TYPE>
    class iTypeBuffer : public iSizeBuffer {
    protected:
        virtual uint64_t UnitSize() const override;
    };

    uint64_t ContainersPP::iSizeVector::Capacity() const
    {
        o_assert_dbg((Buffer().Capacity() % UnitSize()) == 0);

        return Buffer().Capacity() / UnitSize();
    }

    inline uint64_t iSizeVector::SpareBack() const
    {
        o_assert_dbg((Buffer().SpareBack() % UnitSize()) == 0);
        return Buffer().SpareBack() / UnitSize();
    }

    inline void iSizeVector::ReserveBack(uint64_t numElements)
    {
        Buffer().ReserveBack(numElements * UnitSize());
    }

    inline uint64_t iSizeBuffer::SpareFront() const
    {
        o_assert_dbg((Buffer().SpareFront() % UnitSize()) == 0);
        return Buffer().SpareFront() / UnitSize();
    }

    inline void iSizeBuffer::ReserveFront(uint64_t numElements)
    {
        Buffer().ReserveFront(numElements * UnitSize());
    }

    inline void iSizeBlock::operator=(const iSizeBlock& rhs)
    {
        copy(rhs.Data(), rhs.Size());
    }

    inline uint64_t iSizeBlock::Size() const
    {
        o_assert_dbg((Buffer().Size() % UnitSize()) == 0);
        return Buffer().Size() / UnitSize();
    }

    inline bool iSizeBlock::Empty() const
    {
        return !Size();
    }

    inline const uint8_t* iSizeBlock::Data(uint64_t offset) const
    {
        return Buffer().Data(offset * UnitSize());
    }

    inline uint8_t* iSizeBlock::Data(uint64_t offset)
    {
        return Buffer().Data(offset * UnitSize());
    }

    inline void iSizeBlock::CopyBack(const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyBack(data, numElements * UnitSize());
    }

    inline uint8_t* iSizeBlock::AddBack(uint64_t numElements)
    {
        return Buffer().AddBack(numElements * UnitSize());
    }

    inline void iSizeBlock::CopyFront(const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyFront(data, numElements * UnitSize());
    }

    inline uint8_t* iSizeBlock::AddFront(uint64_t numElements)
    {
        return Buffer().AddFront(numElements * UnitSize());
    }

    inline uint64_t iSizeBlock::Remove(uint64_t offset, uint64_t numElements)
    {
        return Buffer().Remove(offset * UnitSize(), numElements * UnitSize()) / UnitSize();
    }

    inline void iSizeBlock::Clear()
    {
        Buffer().Clear();
    }

    inline void iSizeBlock::copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset)
    {
        Buffer().Clear();
        Buffer().AddBack(offset);
        Buffer().CopyBack(ptr, numBytes * UnitSize());
    }

    template<typename TYPE>
    inline uint64_t iTypeBlock<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }
    template<typename TYPE>
    inline uint64_t iTypeVector<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }
    template<typename TYPE>
    inline uint64_t iTypeBuffer<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }

} // namespace 
