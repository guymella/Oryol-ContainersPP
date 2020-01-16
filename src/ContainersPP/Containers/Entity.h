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
		virtual const Types::Schema& Schema() const override { return *schemaPtr; }
		virtual Buffer& getBuffer(uint64_t index) override { return buffers[index]; }
	private:
		Types::Schema* schemaPtr;
		TypeBuffer<Buffer> buffers;
	};

	Entity::Entity(Types::Schema* SchemaPointer) : schemaPtr(SchemaPointer)
	{
		buffers.PushBack(Schema().SizeOfFixed());
	}

}//contanersPP