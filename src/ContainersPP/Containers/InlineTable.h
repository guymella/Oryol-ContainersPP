//------------------------------------------------------------------------------
//  Table.h
//------------------------------------------------------------------------------

#ifndef _INLINETABLE_DEFINED
#define _INLINETABLE_DEFINED

//#include "ContainersPP/Interfaces/iTable.h"
#include "Buffer.h"

namespace ContainersPP {
    class InlinePartition;

	class iInlineTable {
    public:
        //iInlineTable();

        InlinePartition operator[](uint64_t index);
        const InlinePartition operator[](uint64_t index) const;

        uint64_t New();
        uint64_t New(uint64_t newSize);
        void Insert(uint64_t index);
        void Insert(uint64_t index, uint64_t newSize);
        void Remove(uint64_t index);

        /// get number of buffers
        uint64_t Count() const { return Buffer().Size() ? *(uint64_t*)Buffer().Data() : 0; };

        friend class InlinePartition;
    protected:
        uint64_t StartOffset(uint64_t BlockID) const;
        uint64_t Size(uint64_t BlockID) const;   
        uint64_t* Index() { return ((uint64_t*)Buffer().Data(sizeof(uint64_t))); };
        const uint64_t* Index() const { return ((uint64_t*)Buffer().Data(sizeof(uint64_t))); };

        void UpdateFollowingOffsets(uint64_t index, int64_t offsetDelta);
        virtual iBlockD& Buffer() = 0;
        virtual const iBlockD & Buffer() const = 0;
    private:
        uint64_t& count() { return *(uint64_t*)Buffer().Data(); };
        //ContainersPP::Buffer block;
    };

   
    class InlinePartition : public iBlockD {
    public:
        InlinePartition() {};
        InlinePartition(iInlineTable* Table, uint64_t BlockID) : table(Table), PartitionID(BlockID) {};

        InlinePartition(InlinePartition&& rhs) {
            table = rhs.table;
            PartitionID = rhs.PartitionID;
        }

        void operator=(InlinePartition&& rhs) {
            table = rhs.table; 
            PartitionID = rhs.PartitionID;
        }

        InlinePartition(const InlinePartition& rhs) {
            table = rhs.table;
            PartitionID = rhs.PartitionID;
        }

        void operator=(const InlinePartition& rhs) {
            table = rhs.table;
            PartitionID = rhs.PartitionID;
        }
        

        /// get number of bytes in buffer
        virtual uint64_t Size() const override { return table->Size(PartitionID); };
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override;

        /// add bytes to Block
        //virtual void CopyBack(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numBytes);
        /// add bytes to front of Block
        //virtual void CopyFront(const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numBytes);
        /// copy bytes into Block move contents to make room
        //virtual void CopyInsert(uint64_t offset, const uint8_t* data, uint64_t numBytes);
        /// insert uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddInsert(uint64_t offset, uint64_t numBytes);
        /// overwite bytes inside of Block
        //virtual void CopyOver(uint64_t offset, uint64_t numReplace, const uint8_t* data, uint64_t numBytes);
        /// add uninitialized bytes to front of Block, return pointer to start
        //virtual uint8_t* AddOver(uint64_t offset, uint64_t numReplace, uint64_t numBytes);
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numBytes);
        /// Reverse (Transposes) the data at the offset
        //virtual uint8_t* Reverse(uint64_t offset, uint64_t numBytes);
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear();

        friend class iTable;
        friend class Coalator;

        uint64_t ID() const { return PartitionID; };
    private:
        uint64_t StartOffset() const { return table->StartOffset(PartitionID); }
        iInlineTable* table = 0;
        uint64_t PartitionID = 0;        
    };


    class InlineTable : public iInlineTable {
    public:
        InlineTable();
    protected:
        virtual iBlockD& Buffer() override { return block; };
        virtual const iBlockD& Buffer() const override { return block; };
    private:
        ContainersPP::Buffer block;
    };

    class InlineSubTable : public iInlineTable {
    public:
        InlineSubTable(iInlineTable* Table, uint64_t BlockID);
    protected:
        virtual iBlockD& Buffer() override { return block; };
        virtual const iBlockD& Buffer() const override { return block; };
    private:
        InlinePartition block;
    };
}//contanersPP

#endif //defined