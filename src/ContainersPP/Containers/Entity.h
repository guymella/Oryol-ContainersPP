//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "ContainersPP/Interfaces/iEntity.h"
#include "ContainersPP/Interfaces/iEntityTable.h"
//#include "ContainersPP/Types/Schema.h"
//#include "ContainersPP/Types/BitPointer.h"
//#include "TypeBuffer.h"
//#include "Allocator.h"

namespace ContainersPP {

	class Entity : public iEntity{
	public:
		Entity(Types::Schema* SchemaPointer);
	protected:
		virtual const Types::Schema& Schema() const override { return *schemaPtr; }
		virtual iBlockD& MainBuffer() override { return buffers[0]; }
		virtual uint8_t* Columnar(uint64_t index) { return buffers[index].Data(); }
		virtual iBlockD& ColumnBuffer(uint64_t index) { return buffers[index]; };
	private:
		Types::Schema* schemaPtr;
		TypeBuffer<Buffer> buffers;
	};

	Entity::Entity(Types::Schema* SchemaPointer) : schemaPtr(SchemaPointer)
	{
		buffers.PushBack(Schema().SizeOfFixed());
		Schema().WriteDefaults(MainBuffer());
		//add column buffers
		buffers.AddBack(Schema().SeperatedColumnCount());
		//Write Culumnar defaults
		uint64_t colend = Schema().SequenceStart(Types::TypeSequence::Multi);
		for (uint64_t i = Schema().SequenceStart(Types::TypeSequence::Columnar); i < colend; i++)
			buffers[Schema().GetOffset(i)].CopyBack(Schema().GetTypeDescr(i).DefaultValue().data, Schema().GetElmSize(i));
	}

	class EntityRef : public iEntity {
	public:
		EntityRef(iEntityTable* Table, uint64_t EntityID) : table(Table) , ID(EntityID) {};
	protected:
		virtual const Types::Schema& Schema() const override { return table->Schema(); }
		virtual iBlockD& MainBuffer() override { return table->GetEntityMainBuffer(ID); }
		virtual uint8_t* Columnar(uint64_t index) { table->GetEntityColumn(ID, index); };
		virtual iBlockD& ColumnBuffer(uint64_t index) { table->GetEntityColumnBuffer(ID, index); };
	private:
		//Types::Schema* schemaPtr;
		iEntityTable* table;
		uint64_t ID;
	};

	

}//contanersPP