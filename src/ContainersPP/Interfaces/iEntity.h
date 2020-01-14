//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "ContainersPP/Types/Schema.h"
#include "ContainersPP/Types/BitPointer.h"
#include "TypeBuffer.h"
#include "Allocator.h"

namespace ContainersPP {

	class iEntity {
	public:
		
	protected:
		virtual const Types::Schema& Schema() const;
		virtual Buffer& getBuffer(uint64_t index);	
	};

}//contanersPP