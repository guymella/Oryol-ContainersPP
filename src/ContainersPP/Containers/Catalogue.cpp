#include "Catalogue.h"
#include "Catalogue.h"

namespace ContainersPP {

	bool ContainersPP::Catalogue::Define(const uint8_t* Key, uint8_t KeyLen, Types::baseTypes Type, uint16_t Size)
	{
		if (index.Contains(Key, KeyLen))
			return false;

		index.Get(Key, KeyLen).SetValue(addAttribute(Type, Size));
		return true;
	}

	Object Catalogue::Get(const uint8_t* Key, uint8_t KeyLen)
	{
		PackKey* k = index.Find(Key, KeyLen);
		if (k)
			return GetByIndex(k->Value());

		return Object();
	}

	uint32_t ContainersPP::Catalogue::addAttribute(Types::baseTypes Type, uint16_t Size)
	{
		uint32_t count = (uint32_t)(schema[0].Size() / sizeof(CatTypeDescr));
		schema[0].AddBack(sizeof(CatTypeDescr));
		SchemaList()[count] = { Type,Size,(Size) ? (uint16_t)data[0].Size() : (uint16_t)data.Count() };
		if (!Size)
			data.New();
		else
			data[0].CopyBack(Types::DefaultOf(Type), Types::SizeOf(Type));

		return count;
	}


}
