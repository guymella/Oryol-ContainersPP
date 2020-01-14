//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "ContainersPP/Interfaces/iEntity.h"
#include "ContainersPP/Types/Schema.h"
#include "ContainersPP/Types/BitPointer.h"
#include "TypeBuffer.h"
#include "Allocator.h"

namespace ContainersPP {

	class Entity : public iEntity{
	public:
		Entity(Types::Schema* SchemaPointer);
	protected:
		virtual const Types::Schema& Schema() const override;
		virtual Buffer& getBuffer(uint64_t index) override;
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