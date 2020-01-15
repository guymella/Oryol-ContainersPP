//------------------------------------------------------------------------------
//  iTable.h
//------------------------------------------------------------------------------

#include "iTable.h"
//#include "ContainersPP/Containers/Partition.h"


//namespace ContainersPP {

ContainersPP::iBlockD& ContainersPP::iTable::operator[](uint64_t index)
{
	return Index()[index];
}
const ContainersPP::iBlockD& ContainersPP::iTable::operator[](uint64_t index) const
{
	return Index()[index];
}

uint64_t ContainersPP::iTable::New()
	{
		Index().PushBack(Partition(this, Count(), EndOffset(), 0));
		return Index().Size() - 1;
	}

	uint64_t ContainersPP::iTable::New(uint64_t newSize)
	{
		Buffer().AddBack(newSize);
		Index().PushBack(Partition(this, Count(), EndOffset(), newSize));
		return Index().Size() - 1;
	}

	ContainersPP::iBlockD& ContainersPP::iTable::Insert(uint64_t index, uint64_t newSize)
	{
		Buffer().AddInsert(Index()[index].Data()-Buffer().Data(),newSize);
		Index().Insert(index, Partition(this, index, EndOffset(), newSize));
		UpdateOffsets(index + 1, newSize);
		IncrementPartitions(index + 1);
		return Index()[index];
	}

	void ContainersPP::iTable::Remove(uint64_t index)
	{
		Buffer().Remove(Index()[index].StartOffset, Index()[index].Size());		
		UpdateOffsets(index + 1, -(int64_t)(Index()[index].Size()));
		Index().Erase(index);
		DecrementPartitions(index);
	}

	void ContainersPP::iTable::IncrementPartitions(uint64_t index)
	{
		for (++index; index < Count(); ++index) {
			++(Index()[index].PartitionID);
		}
	}

	void ContainersPP::iTable::DecrementPartitions(uint64_t index)
	{
		for (++index; index < Count(); ++index) {
			--(Index()[index].PartitionID);
		}
	}

	void ContainersPP::iTable::UpdateOffsets(uint64_t index, int64_t offsetDelta)
	{
		for (++index; index < Count(); ++index) {
			Index()[index].StartOffset += offsetDelta;
		}
	}

	uint64_t ContainersPP::iTable::EndOffset() const
	{
		if (Empty()) 
			return 0;
		return Index().Back().Data(Index().Back().Size()) - Buffer().Data();
	}

//}//contanersPP

//#endif //defined