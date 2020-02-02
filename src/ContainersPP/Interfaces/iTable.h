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
		virtual void IncrementPartitions(uint64_t index);
		virtual void DecrementPartitions(uint64_t index);
		virtual void UpdateFollowingOffsets(uint64_t index, int64_t offsetDelta);
		virtual uint64_t EndOffset() const;

		virtual TypeBuffer<Partition>& Index() = 0;
		virtual const TypeBuffer<Partition>& Index() const = 0;
		virtual iBlockD& Buffer() = 0;
		virtual const iBlockD& Buffer() const = 0;
	};	

	

}//contanersPP

#endif //defined