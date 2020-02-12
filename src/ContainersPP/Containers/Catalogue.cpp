#include "Catalogue.h"
#include "Catalogue.h"

namespace ContainersPP {

	bool ContainersPP::CatalogueBase::Define(const uint8_t* Key, uint8_t KeyLen, Types::baseTypes Type, uint16_t Size)
	{
		if (Index().Contains(Key, KeyLen))
			return false;

		Index().Get(Key, KeyLen).SetValue(addAttribute(Type, Size));
		return true;
	}

	Object CatalogueBase::Get(const uint8_t* Key, uint8_t KeyLen)
	{
		PackKey* k = Index().Find(Key, KeyLen);
		if (k)
			return GetByIndex(k->Value());

		return Object();
	}

	uint32_t ContainersPP::CatalogueBase::addAttribute(Types::baseTypes Type, uint16_t Size)
	{
		uint32_t count = (uint32_t)(Schema()[0].Size() / sizeof(CatTypeDescr));
		Schema()[0].AddBack(sizeof(CatTypeDescr));
		SchemaList()[count] = { Type,Size,(Size) ? (uint16_t)Data()[0].Size() : (uint16_t)Data().Count() };
		if (!Size)
			Data().New();
		else
			Data()[0].CopyBack(Types::DefaultOf(Type), Types::SizeOf(Type));

		return count;
	}


	FileCatalogue::FileCatalogue(const char* FolderName, uint64_t FileID) : table(FolderName, FileID)
	{
		if (table.Count() > 2) {//exists so assign subtables
			data = InlineSubTable(&table, 0);
			schema = InlineSubTable(&table, 1);
			index = SubIndexTrie(&table, 2);
		}
		else {
			table.Clear();
			table.New();
			table.New();
			table.New();
			data = InlineSubTable(&table, 0, 1);
			schema = InlineSubTable(&table, 1, 1);
			index = SubIndexTrie(&table, 2);
		}
	}

}
