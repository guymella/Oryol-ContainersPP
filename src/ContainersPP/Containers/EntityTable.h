//------------------------------------------------------------------------------
//  Entity.h
//------------------------------------------------------------------------------

#include "Schema.h"
#include "BitPointer.h"
#include "Structures/Containers/Table.h"

class Entity {
public:
	Entity(Schema* SchemaPointer);
	BitItr<uint8_t, 1> BooleanFlags(const size_t& offset = 0);
	Itr<uint8_t> FixedBegin(const size_t& offset = 0);
	BitItr<uint8_t,1> NullableFlags(const size_t& offset = 0);
	Itr<uint8_t> NullableBegin(const size_t& offset = 0);
	BitItr<uint8_t, 1> SparseFlags(const size_t& offset = 0);
	Itr<uint8_t> SparseBegin(const size_t& offset = 0);
	void* Get(const size_t& column);
	void* Get(const KeyString& key);
	void* Get(const char* key);
	const void* Get(const size_t& column) const;
	const void* Get(const KeyString& key) const;
	const void* Get(const char* key) const;
private:
	Schema* schemaPtr;
	Table data;
};

Entity::Entity(Schema* SchemaPointer) : schemaPtr(SchemaPointer)
{
	data.Push(schemaPtr->SizeOfFixed());
}

inline Itr<uint8_t> Entity::FixedBegin(const size_t& offset)
{
	return data.GetPartition(0).begin(offset);
}
