//------------------------------------------------------------------------------
//  Partition.cpp
//------------------------------------------------------------------------------

#include "Partition.h"



//namespace ContainersPP {

const uint8_t* ContainersPP::Partition::Data(uint64_t offset) const
{
    return table->Buffer().Data(StartOffset + offset);
}
uint8_t* ContainersPP::Partition::Data(uint64_t offset)
{
    return table->Buffer().Data(StartOffset+ offset);
}

uint8_t* ContainersPP::Partition::AddBack(uint64_t numBytes)
    {
        uint64_t oldSize = size;
        table->Buffer().AddInsert(StartOffset + size, numBytes);
        table->UpdateFollowingOffsets(PartitionID, numBytes);
        size += numBytes;
        return Data(oldSize);
    }

    uint8_t* ContainersPP::Partition::AddFront(uint64_t numBytes)
    {
        table->Buffer().AddInsert(StartOffset, numBytes);
        table->UpdateFollowingOffsets(PartitionID, numBytes);
        size += numBytes;
        return Data();
    }

    uint8_t* ContainersPP::Partition::AddInsert(uint64_t offset, uint64_t numBytes)
    {
        table->Buffer().AddInsert(StartOffset + offset, numBytes);
        table->UpdateFollowingOffsets(PartitionID, numBytes);
        size += numBytes;
        return Data(offset);
    }

    uint64_t ContainersPP::Partition::Remove(uint64_t offset, uint64_t numBytes)
    {
        #ifdef DEFENSE
        if (offset > Size()) return 0;
        if (offset + numBytes > Size()) numBytes = Size() - offset;
        #endif // DEFENSE

        table->Buffer().Remove(StartOffset + offset, numBytes);
        table->UpdateFollowingOffsets(PartitionID, -(int64_t)numBytes);
        size -= numBytes;
        return numBytes;
    }

    void ContainersPP::Partition::Clear()
    {
        Remove(0, Size());
    }

//}//contanersPP

//#endif //defined