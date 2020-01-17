//------------------------------------------------------------------------------
//  iEntityTable.h
//------------------------------------------------------------------------------

#include "ContainersPP/Types/Schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "TypeBuffer.h"
#include "ContainersPP/Containers/Allocator.h"
#include "ContainersPP/Containers/Coalator.h"

namespace ContainersPP {

	class iEntityTable {
	public:

		iBlockD& GetEntityMainBuffer(uint64_t EntityID);
		const iBlockD& GetEntityMainBuffer(uint64_t EntityID) const;

		iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex);
		const iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex) const;

		uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex);
		const uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex) const;

		const Types::Schema& Schema() const;
	protected:
		iAllocator& MainBuffers();
		Coalator& ColumnBuffers(uint64_t ColumnIndex);		
	private:
		//Types::Schema* schemaPtr;

		
	};

	


}//contanersPP