#include "InlineTable.h"

ContainersPP::InlineTable::InlineTable(uint32_t PartitionCount)
{
    Buffer().CopyBack((uint8_t*)Types::BASEDEFAULT, sizeof(uint64_t)*2);
    Index()[0] = sizeof(uint64_t) * 2;
    for (uint32_t i = 0; i < PartitionCount; i++) 
        New();
}
ContainersPP::InlinePartition ContainersPP::iInlineTable::operator[](uint64_t index)
{
    return InlinePartition(this,index);
}

const ContainersPP::InlinePartition ContainersPP::iInlineTable::operator[](uint64_t index) const
{
    return InlinePartition(const_cast<iInlineTable*>(this), index);
}

uint64_t ContainersPP::iInlineTable::New()
{
    Buffer().AddInsert(sizeof(uint64_t) * (Count()+2), sizeof(uint64_t));
    ++count();
    Index()[Count()] = Index()[Count() - 1];
    //table shifted update all offsets
    UpdateFollowingOffsets(0, sizeof(uint64_t));

    return Count()-1;
}

uint64_t ContainersPP::iInlineTable::New(uint64_t newSize)
{
    
    New();
    Index()[Count()] += newSize;
    Buffer().AddBack(newSize);

    return Count() - 1;
}

void ContainersPP::iInlineTable::Insert(uint64_t index)
{
#ifdef DEFENSE
    if (index >= Count())
        return;
#endif // DEFENSE
    Buffer().AddInsert(sizeof(uint64_t) * (index + 2), sizeof(uint64_t));
    Index()[index + 1] = Index()[index];
    ++count();
    UpdateFollowingOffsets(0, sizeof(uint64_t));    
}

void ContainersPP::iInlineTable::Insert(uint64_t index, uint64_t newSize)
{
    #ifdef DEFENSE
    if (index >= Count())
        return;
    #endif // DEFENSE
    
    Insert(index);
    Buffer().AddInsert(StartOffset(index), newSize);
    UpdateFollowingOffsets(index+1, newSize); 
}

void ContainersPP::iInlineTable::Remove(uint64_t index)
{
#ifdef DEFENSE
    if (index >= Count())
        return;
#endif // DEFENSE
    Buffer().Remove(StartOffset(index), Size(index));
    UpdateFollowingOffsets(index, -(int64_t)Size(index));
    Buffer().Remove(sizeof(uint64_t) * (index + 1), sizeof(uint64_t));
    --count();
    UpdateFollowingOffsets(0, -(int64_t)sizeof(uint64_t));
}

uint64_t ContainersPP::iInlineTable::StartOffset(uint64_t BlockID) const
{
	#ifdef DEFENSE
	if (BlockID > Count())
			return std::numeric_limits<uint64_t>::max();
	#endif // DEFENSE	

	return Index()[BlockID];
}

uint64_t ContainersPP::iInlineTable::Size(uint64_t BlockID) const
{
	#ifdef DEFENSE
	if (BlockID >= Count())
		return 0;
	#endif // DEFENSE

	return StartOffset(BlockID + 1) - StartOffset(BlockID);
}

void ContainersPP::iInlineTable::UpdateFollowingOffsets(uint64_t index, int64_t offsetDelta)
{
	#ifdef DEFENSE
		if (index >= Count())
			return 0;
	#endif // DEFENSE
	uint64_t* IndexPtr = Index();
	for (index; index <=Count() ; index++)
		IndexPtr[index] += offsetDelta;
}





/////////////////////////////////////

const uint8_t* ContainersPP::InlinePartition::Data(uint64_t offset) const
{
    return table->Buffer().Data(StartOffset() + offset);
}
uint8_t* ContainersPP::InlinePartition::Data(uint64_t offset)
{
    return table->Buffer().Data(StartOffset() + offset);
}

uint8_t* ContainersPP::InlinePartition::AddBack(uint64_t numBytes)
{
    uint64_t oldSize = Size();
    table->Buffer().AddInsert(StartOffset() + Size(), numBytes);
    table->UpdateFollowingOffsets(PartitionID+1, numBytes);
    return Data(oldSize);
}

uint8_t* ContainersPP::InlinePartition::AddFront(uint64_t numBytes)
{
    table->Buffer().AddInsert(StartOffset(), numBytes);
    table->UpdateFollowingOffsets(PartitionID+1, numBytes);
    return Data();
}

uint8_t* ContainersPP::InlinePartition::AddInsert(uint64_t offset, uint64_t numBytes)
{
    table->Buffer().AddInsert(StartOffset() + offset, numBytes);
    table->UpdateFollowingOffsets(PartitionID+1, numBytes);
    return Data(offset);
}

uint64_t ContainersPP::InlinePartition::Remove(uint64_t offset, uint64_t numBytes)
{
#ifdef DEFENSE
    if (offset > Size()) return 0;
    if (offset + numBytes > Size()) numBytes = Size() - offset;
#endif // DEFENSE

    table->Buffer().Remove(StartOffset() + offset, numBytes);
    table->UpdateFollowingOffsets(PartitionID+1, -(int64_t)numBytes);
    return numBytes;
}

void ContainersPP::InlinePartition::Clear()
{
    Remove(0, Size());
}

ContainersPP::InlineSubTable::InlineSubTable(iInlineTable* Table, uint64_t BlockID, uint32_t PartitionCount) : block(Table, BlockID)
{
    Buffer().CopyBack((uint8_t*)Types::BASEDEFAULT, sizeof(uint64_t) * 2);
    Index()[0] = sizeof(uint64_t) * 2;
    for (uint32_t i = 0; i < PartitionCount; i++) 
        New();
}
