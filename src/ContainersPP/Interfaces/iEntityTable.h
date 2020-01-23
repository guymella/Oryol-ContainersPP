//------------------------------------------------------------------------------
//  iEntityTable.h
//------------------------------------------------------------------------------

#ifndef _IENTITYTABLE_DEF
#define _IENTITYTABLE_DEF

#include "ContainersPP/Types/Schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "TypeBuffer.h"
#include "ContainersPP/Containers/Allocator.h"
#include "ContainersPP/Containers/Coalator.h"

namespace ContainersPP {

	class iEntityTable {
	public:

		virtual uint64_t Insert() = 0;


		virtual iBlockD& GetEntityMainBuffer(uint64_t EntityID) = 0;
		virtual const iBlockD& GetEntityMainBuffer(uint64_t EntityID) const = 0;

		virtual uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex) = 0;
		virtual const uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex) const = 0;

		virtual iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex) = 0;
		virtual const iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex) const = 0;

		virtual iAllocator& GetEntityColumnAllocator(uint64_t EntityID, uint64_t ColumnIndex) = 0;
		virtual const iAllocator& GetEntityColumnAllocator(uint64_t EntityID, uint64_t ColumnIndex) const = 0;

		virtual const Types::Schema& Schema() const = 0;
	protected:
		//iAllocator& MainBuffers();
		//iAllocator& ColumnBuffers(uint64_t ColumnIndex);
		//iAllocator& ColumnAllocators(uint64_t ColumnIndex);
	private:
		//Types::Schema* schemaPtr;

		
	};

	


}//contanersPP

#endif