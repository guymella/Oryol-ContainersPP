//------------------------------------------------------------------------------
//  Partition.h
//------------------------------------------------------------------------------

#ifndef _PARTITION_DEF
#define _PARTITION_DEF

#include "ContainersPP/Types/Schema.h"
#include "ContainersPP/Types/BitPointer.h"
#include "TypeBuffer.h"
#include "Allocator.h"
//#include "ContainersPP/Interfaces/iTable.h"

namespace ContainersPP {

    class iTable;

	class Partition : public iBlockD {
	public:
		Partition(iTable* Table, uint64_t BlockID, uint64_t StartingOffset, uint64_t Size) : table(Table), PartitionID(BlockID), StartOffset(StartingOffset), size(Size) {};

        /// get number of bytes in buffer
        virtual uint64_t Size() const override { return size; };
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override { return table->Buffer().Data(StartOffset); };
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override { return table->Buffer().Data(StartOffset); };

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
	private:
		iTable* table;
		uint64_t PartitionID;
        uint64_t StartOffset;
        uint64_t size;
	};

    uint8_t* ContainersPP::Partition::AddBack(uint64_t numBytes)
    {
        uint64_t oldSize = size;
        table->Buffer().AddInsert(StartOffset + size, numBytes);
        table->UpdateOffsets(PartitionID+1, numBytes);
        size += numBytes;
        return Data(oldSize);
    }

    inline uint8_t* Partition::AddFront(uint64_t numBytes)
    {
        table->Buffer().AddInsert(StartOffset, numBytes);
        table->UpdateOffsets(PartitionID + 1, numBytes);
        size += numBytes;
        return Data();
    }

    inline uint8_t* Partition::AddInsert(uint64_t offset, uint64_t numBytes)
    {
        table->Buffer().AddInsert(StartOffset + offset, numBytes);
        table->UpdateOffsets(PartitionID + 1, numBytes);
        size += numBytes;
        return Data(offset);
    }

    inline uint64_t Partition::Remove(uint64_t offset, uint64_t numBytes)
    {
        #ifdef DEFENSE
        if (offset > Size()) return 0;
        if (offset + numBytes > Size()) numBytes = Size() - offset;
        #endif // DEFENSE

        table->Buffer().Remove(StartOffset + offset, numBytes);
        table->UpdateOffsets(PartitionID + 1, -numBytes);
        size -= numBytes;
        return numBytes;
    }

    inline void Partition::Clear()
    {
        Remove(0, Size());
    }

}//contanersPP

#endif //defined