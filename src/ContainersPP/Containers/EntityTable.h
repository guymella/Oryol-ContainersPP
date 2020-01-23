//------------------------------------------------------------------------------
//  EntityTable.h
//------------------------------------------------------------------------------

#ifndef _ENTITYTABLE_DEF
#define _ENTITYTABLE_DEF

#include "ContainersPP/Interfaces/iEntityTable.h"
#include "ContainersPP/Interfaces/iEntity.h"
//#include "Entity.h"

namespace ContainersPP {

	class EntityRef : public iEntity {
	public:
		EntityRef(iEntityTable* Table, uint64_t EntityID) : table(Table), ID(EntityID) {};
	protected:
		virtual const Types::Schema& Schema() const override { return table->Schema(); }
		virtual iBlockD& MainBuffer() override { return table->GetEntityMainBuffer(ID); }
		virtual uint8_t* Columnar(uint64_t index) { return table->GetEntityColumn(ID, index); };
		virtual iBlockD& ColumnBuffer(uint64_t index) { return table->GetEntityColumnBuffer(ID, index); };
		virtual iAllocator& ColumnAllocator(uint64_t index) { return table->GetEntityColumnAllocator(ID, index); }
	private:
		//Types::Schema* schemaPtr;
		iEntityTable* table;
		uint64_t ID;
	};

	class EntityTable : public iEntityTable {
	public:
		EntityTable(Types::Schema* SchemaPtr);

		EntityRef operator[](uint64_t EntityID);

		virtual uint64_t Insert();

		virtual iBlockD& GetEntityMainBuffer(uint64_t EntityID) { return ColumnBuffers[0][EntityID]; };
		virtual const iBlockD& GetEntityMainBuffer(uint64_t EntityID) const { return ColumnBuffers[0][EntityID]; };

		virtual uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex) { return ColumnBuffers[ColumnIndex][EntityID].Data(); };
		virtual const uint8_t* GetEntityColumn(uint64_t EntityID, uint64_t ColumnIndex) const { return ColumnBuffers[ColumnIndex][EntityID].Data(); };

		virtual iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex) { return ColumnBuffers[ColumnIndex][EntityID]; };
		virtual const iBlockD& GetEntityColumnBuffer(uint64_t EntityID, uint64_t ColumnIndex) const { return ColumnBuffers[ColumnIndex][EntityID]; };

		virtual iAllocator& GetEntityColumnAllocator(uint64_t EntityID, uint64_t ColumnIndex) { return ColumnAllocators[ColumnIndex][EntityID]; };
		virtual const iAllocator& GetEntityColumnAllocator(uint64_t EntityID, uint64_t ColumnIndex) const { return ColumnAllocators[ColumnIndex][EntityID]; };

		virtual const Types::Schema& Schema() const { return *schemaPtr; };

		virtual uint64_t Count() const { return ColumnBuffers[0].Count(); };
	private:
		Types::Schema* schemaPtr;
		Allocator allocator;
		TypeBlock<CoAllocator> ColumnBuffers;
		TypeBlock<TypeVector<CoAllocator>> ColumnAllocators;
	};

	ContainersPP::EntityTable::EntityTable(Types::Schema* SchemaPtr) : schemaPtr(SchemaPtr)
	{
		//buffers.New(Schema().SizeOfFixed());
		//Schema().WriteDefaults(MainBuffer());
		//add column buffers
		for (uint64_t i = 0; i < Schema().SeperatedColumnCount()+1; i++)
			ColumnBuffers.PushBack(CoAllocator(&allocator));

		//add multivar allocators vectors
		ColumnAllocators.AddBack(Schema().BlockCount(7));
	}

	inline EntityRef EntityTable::operator[](uint64_t EntityID)
	{
		o_assert_dbg(EntityID < Count());
		return EntityRef(this, EntityID);
	}

	inline uint64_t EntityTable::Insert()
	{
		uint64_t newID = ColumnBuffers[0].New(Schema().SizeOfFixed());

		Schema().WriteDefaults(ColumnBuffers[0][newID]);
		
		//add column buffers
		for (uint64_t i = 0; i < Schema().SeperatedColumnCount(); i++)
			ColumnBuffers[1 + i].New();

		//add multivar allocators
		for (uint64_t i = 0; i < ColumnAllocators.Size(); i++)
			ColumnAllocators[i].PushBack(CoAllocator(&allocator));
		
		//Write Culumnar defaults
		uint64_t colend = Schema().SequenceStart(Types::TypeSequence::Multi);
		for (uint64_t i = Schema().SequenceStart(Types::TypeSequence::Columnar); i < colend; i++)
			ColumnBuffers[Schema().GetOffset(i)][newID].CopyBack(Schema().GetTypeDescr(i).DefaultValue().data, Schema().GetElmSize(i));

		return newID;
	}

}//namespace

#endif