#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Catalogue
    @ingroup Core
    @brief growable memory buffer for raw data
*/


#ifndef Included_CATALOGUE_H
#define Included_CATALOGUE_H



//#include "ContainersPP/Types/TypeDescr.h"
#include "Object.h"
#include "TypeTrie.h"
//#include "Core/Assertion.h"
//#include "Core/Memory/Memory.h"
//#include "FileBlock.h"
//#include "TypeBuffer.h"
//#include "Allocator.h"

namespace ContainersPP {

    struct CatTypeDescr {
        Types::baseTypes type;
        uint16_t size = 1;
        uint16_t offset = 0;
    };


    class iCatalogueStruct : public iCatalogue {
    public:
        virtual bool Define(const uint8_t* Key, uint8_t KeyLen, Types::baseTypes Type, uint16_t Size = 1) = 0;
        bool DefineAttribute(const char* Key, Types::baseTypes Type, uint16_t Size = 1) { return Define((const uint8_t*)Key, (uint8_t)std::strlen(Key),Type,Size); };
        bool DefineAttribute(const Types::KeyString& Key, Types::baseTypes Type, uint16_t Size = 1) { return Define(Key.Data(), (uint8_t)Key.Size(),Type,Size); };

    };

    class CatalogueBase : public iCatalogueStruct {
    public:
        virtual bool Define(const uint8_t* Key, uint8_t KeyLen, Types::baseTypes Type, uint16_t Size = 1) override;
        //void Inherit();
        //void InheritAs();
        //void UsingAttribute();
        using iCatalogue::Get;

        virtual Types::baseTypes Type() const override { return Types::baseTypes::Struct; };
        virtual void* Ptr() override { return DataBlock().Data(); };
        virtual const void* Ptr() const override { return DataBlock().Data(); };
        virtual void* Begin(uint64_t Index = 0) override { return nullptr; };//TODO::
        virtual const void* Begin(uint64_t Index = 0) const override { return nullptr; };//TODO::
        virtual bool Remove(uint32_t Index) override { return false; };//TODO::
        virtual uint64_t Count() const override { return 0; };//TODO::
        virtual iCatalogue& GetRef(const uint8_t* Key, uint8_t KeyLen) { return NULLOBJECT; };//TODO::
        virtual iCatalogue& GetRefByIndex(uint64_t Index) override { return NULLOBJECT; };//TODO::
        virtual iCatalogue& CDRRef(uint64_t BaseIndex) override { return NULLOBJECT; };//TODO::
        virtual Object Get(const uint8_t* Key, uint8_t KeyLen) override;
        virtual Object GetByIndex(uint64_t Index) override { return Object(SchemaList()[Index].type, this, SchemaList()[Index].offset); };//TODO::
        virtual Object CDR(uint64_t BaseIndex) override { return Object(); };//TODO::
        CatTypeDescr* SchemaList() { return (CatTypeDescr*)Schema()[0].Data(); };
        const CatTypeDescr* SchemaList() const { return (CatTypeDescr*)Schema()[0].Data(); };
    protected:
        virtual bool pushBack() override { return false; };//TODO::
        virtual bool pushFront() override { return false; };//TODO::
        virtual bool insert(uint32_t Index) override { return false; };//TODO::

        uint32_t addAttribute(Types::baseTypes Type, uint16_t Size);
        InlinePartition DataBlock(uint16_t ColumnIndex = 0) { return Data()[ColumnIndex]; };
        const InlinePartition DataBlock(uint16_t ColumnIndex = 0) const { return Data()[ColumnIndex]; };

        virtual iInlineTable& Table() = 0;
        virtual iInlineTable& Data() = 0;
        virtual const iInlineTable& Data() const = 0;
        virtual iIndexTrie& Index() = 0;
        virtual iInlineTable& Schema() = 0;
        virtual const iInlineTable& Schema() const = 0;
    };

    class Catalogue : public CatalogueBase {
    public:
        Catalogue() : table(3), data(&table, 0,1), schema(&table, 1,1), index(&table, 2) {};
        
    protected:        

        virtual iInlineTable& Table() override { return table; };
        virtual iInlineTable& Data() override { return data; };
        virtual const iInlineTable& Data() const override { return data; };
        virtual iIndexTrie& Index() override { return index; };
        virtual iInlineTable& Schema() override { return schema; };
        virtual const iInlineTable& Schema() const override { return schema; };
    private:
        
        
        
        //uint16_t EndOffset();
        //uint16_t ColumnCount();
        //iBlockD& cdr_Block();
        //iBlockD& Instances_Block();

        InlineTable table;
        SubIndexTrie index;
        InlineSubTable schema;
        InlineSubTable data;
    };

    class FileCatalogue : public CatalogueBase {
    public:
        FileCatalogue(const char* FolderName, uint64_t FileID);
        bool Save() { return table.Save(); };
        bool Delete() { return table.Delete(); };
    protected:

        virtual iInlineTable& Table() override { return table; };
        virtual iInlineTable& Data() override { return data; };
        virtual const iInlineTable& Data() const override { return data; };
        virtual iIndexTrie& Index() override { return index; };
        virtual iInlineTable& Schema() override { return schema; };
        virtual const iInlineTable& Schema() const override { return schema; };
    private:



        //uint16_t EndOffset();
        //uint16_t ColumnCount();
        //iBlockD& cdr_Block();
        //iBlockD& Instances_Block();

        InlineFileTable table;
        SubIndexTrie index;
        InlineSubTable schema;
        InlineSubTable data;
    };




} // namespace Oryol


#endif