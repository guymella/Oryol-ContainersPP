//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "ContainersPP/Types/Schema.h"
#include "ContainersPP/Types/BitPointer.h"
#include "TypeBuffer.h"
#include "Allocator.h"

namespace ContainersPP {

	class iEntityTable {
	public:

	protected:
		const Types::Schema& schema() const;
	private:
		Types::Schema* schemaPtr;
		TypeBuffer<BufferPtr> buffers;
	};

	Entity::Entity(Types::Schema* SchemaPointer) : schemaPtr(SchemaPointer)
	{
		data.Push(schemaPtr->SizeOfFixed());
	}

	inline Itr<uint8_t> Entity::FixedBegin(const size_t& offset)
	{
		return data.GetPartition(0).begin(offset);
	}


}//contanersPP