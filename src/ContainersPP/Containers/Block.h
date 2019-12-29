#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Block
    @ingroup Core
    @brief growable memory Block for raw data
*/

#include "Core/Memory/Memory.h"
#include "BlockStatic.h"

namespace ContainersPP {

    class Block : public iBlockD {
    public:
        /// default constructor
        Block() {};
        /// Copy construct
        Block(const Block& rhs);
        /// move constructor
        Block(Block&& rhs);
        /// destructor
        ~Block();

        /// move-assignment
        void operator=(Block&& rhs);
        
        /// get number of bytes in Block
        virtual uint64_t Size() const override;  
        using iBlockD::CopyBack;
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes) override;
        using iBlockD::CopyFront;
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes) override;
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes) override;
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear() override;

        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override;

    protected:
        /// (re-)allocate Block
        void alloc(uint64_t newCapacity, uint64_t offset = 0);
        /// (re-)allocate Block
        void allocRemove(uint64_t offset, uint64_t sizeRemove);
        /// destroy Block
        void destroy();
        // Move
        void move(Block&& rhs);
        
        uint64_t size = 0;
        uint8_t* data = nullptr;
    };

    //------------------------------------------------------------------------------
    inline Block::Block(Block&& rhs)
    {
        move(std::move(rhs));
    }

    //------------------------------------------------------------------------------
    inline Block::~Block() {
        destroy();
    }

    //------------------------------------------------------------------------------
    inline void Block::alloc(uint64_t newCapacity ,uint64_t offset) {
        o_assert_dbg(newCapacity > Size());

        uint8_t* newBuf = (uint8_t*)Oryol::Memory::Alloc((int)newCapacity);
        if (size > 0) {
            o_assert_dbg(data);
            Oryol::Memory::Copy(data, newBuf+offset, (int)size);
        }
        if (data) {
            Oryol::Memory::Free(data);
        }
        data = newBuf;
        size = newCapacity;
    }   

    inline void Block::allocRemove(uint64_t offset, uint64_t sizeRemove)
    {
        if (offset >= Size())
            return;
        if (offset+sizeRemove > Size())
            sizeRemove = Size() - offset;
        o_assert_dbg(offset + sizeRemove <= Size());
        if (!(Size() - sizeRemove)) {
            return Clear();
        }

        uint8_t* newBuf = (uint8_t*)Oryol::Memory::Alloc((int)(Size() - sizeRemove));
        if (size > 0) {
            o_assert_dbg(data);
            Oryol::Memory::Copy(data, newBuf, (int)offset);
            Oryol::Memory::Copy(data+offset+sizeRemove, newBuf+offset, (int)(Size()-(offset+sizeRemove)));
        }
        if (data) {
            Oryol::Memory::Free(data);
        }
        data = newBuf;
        size -= sizeRemove;
    }

    //------------------------------------------------------------------------------
    inline void Block::destroy() {
        if (data) {
            Oryol::Memory::Free(data);
        }
        data = nullptr;
        size = 0;
    }

    inline void Block::move(Block&& rhs)
    {
        size = rhs.size;
        data = rhs.data;
        rhs.size = 0;
        rhs.data = nullptr;
    }

    //------------------------------------------------------------------------------
    inline void Block::operator=(Block&& rhs) {
        this->destroy();
        move(std::move(rhs));
    }

    //------------------------------------------------------------------------------
    inline  uint64_t Block::Size() const {
        return size;
    }

    //------------------------------------------------------------------------------
    inline uint8_t* Block::AddBack(uint64_t numBytes) {
        uint64_t oldSize = Size();
        alloc(Size() + numBytes);
        return Data(oldSize);
    }

    inline uint8_t* Block::AddFront(uint64_t numBytes)
    {
        alloc(Size()+numBytes, numBytes);
        return Data();
    }

    //------------------------------------------------------------------------------
    inline void Block::Clear() {
        destroy();
    }

    //------------------------------------------------------------------------------
    inline  uint64_t Block::Remove(uint64_t offset, uint64_t numBytes) {
        if (offset >= size) {
            return 0;
        }
        if ((offset + numBytes) >= size) {
            numBytes = size - offset;
        }
        o_assert_dbg((offset + numBytes) <= size);
        allocRemove(offset, numBytes);
        return numBytes;
    }

    //------------------------------------------------------------------------------
    inline const uint8_t* Block::Data(uint64_t offset) const {
        o_assert(data);
        return data+offset;
    }

    //------------------------------------------------------------------------------
    inline uint8_t* Block::Data(uint64_t offset) {
        o_assert(data);
        return data+offset;
    }

} // namespace Oryol
