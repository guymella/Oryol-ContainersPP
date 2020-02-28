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

	Object ContainersPP::CatalogueBase::GetByIndex(uint64_t Index)
	{//TODO::
		
		if (SchemaList()[Index].size == 0)//flex list
			return Object(SchemaList()[Index].type, Data()[SchemaList()[Index].offset]);
		
		//fixed list
		return Object(SchemaList()[Index].type, this, SchemaList()[Index].offset, SchemaList()[Index].size);
	};

	uint32_t ContainersPP::CatalogueBase::addAttribute(Types::baseTypes Type, uint16_t Size)
	{
		uint32_t count = (uint32_t)(Schema()[0].Size() / sizeof(CatTypeDescr));//attribute count
		Schema()[0].AddBack(sizeof(CatTypeDescr));//make room for typedescr
		SchemaList()[count] = { Type,Size,(Size) ? (uint16_t)Data()[0].Size() : (uint16_t)Data().Count() }; //write typedescr fixed size / struct offset : column Index
		if (!Size)//add new flex column
			Data().New();
		else //add new fixed attribute
			Data()[0].CopyBack(Types::DefaultOf(Type), Types::SizeOf(Type)*Size);

		return count;
	}


	FileCatalogue::FileCatalogue(const char* FolderName, uint64_t FileID, iProfile* ProfilePtr) : profile(ProfilePtr), table(FolderName, FileID)
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
