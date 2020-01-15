//------------------------------------------------------------------------------
//  iTable.h
//------------------------------------------------------------------------------

#ifndef _ITABLE_DEFINED
#define _ITABLE_DEFINED

//#include "ContainersPP/Types/Schema.h"
//#include "ContainersPP/Types/BitPointer.h"
#include "ContainersPP/Containers/TypeBuffer.h"
#ifndef _PARTITION_DEF
#include "ContainersPP/Containers/Partition.h"
#endif // !_PARTITION_DEF


#include "iAllocator.h"




namespace ContainersPP {
	//#ifndef _PARTITION_DEF
	//class Partition;
	//#endif // !_PARTITION_DEF
	class iTable : public iAllocator {
	public:
		virtual iBlockD& operator[](uint64_t index) override;
		virtual const iBlockD& operator[](uint64_t index) const override;

		virtual uint64_t New() override;
		virtual uint64_t New(uint64_t newSize) override;
		virtual iBlockD& Insert(uint64_t index, uint64_t newSize);
		virtual void Remove(uint64_t index);

		/// get number of buffers
		virtual uint64_t Count() const override { return Index().Size(); };

		friend class Partition;
	protected:
		void IncrementPartitions(uint64_t index);
		void DecrementPartitions(uint64_t index);
		void UpdateOffsets(uint64_t index, int64_t offsetDelta);
		uint64_t EndOffset() const;

		virtual TypeBuffer<Partition>& Index() = 0;
		virtual const TypeBuffer<Partition>& Index() const = 0;
		virtual iBlockD& Buffer() = 0;
		virtual const iBlockD& Buffer() const = 0;
	};	

	/*uint64_t ContainersPP::iTable::New()
	{
		Index().PushBack(Partition(this, Count(), EndOffset(), 0));
		return Index().Size() - 1;
	}

	inline uint64_t iTable::New(uint64_t newSize)
	{
		Buffer().AddBack(newSize);
		Index().PushBack(Partition(this, Count(), EndOffset(), newSize));
		return Index().Size() - 1;
	}

	inline iBlockD& iTable::Insert(uint64_t index, uint64_t newSize)
	{
		Buffer().AddInsert(Index()[index].Data()-Buffer().Data(),newSize);
		Index().Insert(index, Partition(this, index, EndOffset(), newSize));
		UpdateOffsets(index + 1, newSize);
		IncrementPartitions(index + 1);
		return Index()[index];
	}

	inline void iTable::Remove(uint64_t index)
	{
		Buffer().Remove(Index()[index].StartOffset, Index()[index].Size());		
		UpdateOffsets(index + 1, -(int64_t)(Index()[index].Size()));
		Index().Erase(index);
		DecrementPartitions(index);
	}

	inline void iTable::IncrementPartitions(uint64_t index)
	{
		for (++index; index < Count(); ++index) {
			++(Index()[index].PartitionID);
		}
	}

	inline void iTable::DecrementPartitions(uint64_t index)
	{
		for (++index; index < Count(); ++index) {
			--(Index()[index].PartitionID);
		}
	}

	inline void iTable::UpdateOffsets(uint64_t index, int64_t offsetDelta)
	{
		for (++index; index < Count(); ++index) {
			Index()[index].StartOffset += offsetDelta;
		}
	}*/

}//contanersPP

#endif //defined