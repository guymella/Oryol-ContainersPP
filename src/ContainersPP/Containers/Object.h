#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iCountable
    @ingroup Interfaces
    @brief interface for structures that have countable size
*/

#ifndef Included_Object_H
#define Included_Object_H


#include "ContainersPP/Interfaces/iObject.h"
#include "ContainersPP/Interfaces/iAllocator.h"
#include "InlineTable.h"

namespace ContainersPP {

    class NullObject : public iCatalogue {
    public:
        virtual Types::baseTypes Type() const override { return Types::baseTypes::Void; };
        virtual void* Ptr() override { return nullptr; };
        virtual const void* Ptr() const override { return nullptr; };
        virtual void* Begin(uint64_t Index = 0) override { return nullptr; };
        virtual const void* Begin(uint64_t Index = 0) const override { return nullptr; };
        virtual bool Remove(uint32_t Index) override { return false; };
        virtual uint64_t Count() const override { return 0; };
        virtual iCatalogue& GetRef(const uint8_t* Key, uint8_t KeyLen) override;
        virtual iCatalogue& GetRefByIndex(uint64_t Index) override;
        virtual iCatalogue& CDRRef(uint64_t BaseIndex) override;
        virtual Object Get(const uint8_t* Key, uint8_t KeyLen) override;
        virtual Object GetByIndex(uint64_t Index) override;
        virtual Object CDR(uint64_t BaseIndex) override;
    protected:
        virtual bool pushBack() override { return false; };
        virtual bool pushFront() override { return false; };
        virtual bool insert(uint32_t Index) override { return false; };
    };

    static NullObject NULLOBJECT;

    class Primitive : public NullObject {
    public:
        Primitive(Types::baseTypes valueType) : type(valueType) {};
        virtual Types::baseTypes Type() const override { return type; };
        virtual void* Ptr() override { return &uint_8; };
        virtual const void* Ptr() const override { return &uint_8; };

        friend Object;
    private:
        Types::baseTypes type;
        union {
            uint8_t uint_8;
            uint16_t uint_16;
            uint32_t uint_32;
            uint64_t uint_64 = 0;
            int8_t int_8;
            int16_t int_16;
            int32_t int_32;
            int64_t int_64;
            float float_32;
            double float_64;
            bool boolean_8;
        };
    };

    class Primitive_Ref: public NullObject {
    public:
        Primitive_Ref(Types::baseTypes valueType, void* Ptr) : type(valueType), ptr(Ptr) {};
        Primitive_Ref(const Primitive_Ref& rhs) : type(rhs.type), ptr(rhs.ptr) {};
        virtual Types::baseTypes Type() const override { return type; };
        virtual void* Ptr() override { return ptr; };
        virtual const void* Ptr() const override { return ptr; };

        friend Object;
    private:
        Types::baseTypes type;
        void* ptr;
    };

    class Attribute_Ref : public NullObject {
    public:
        Attribute_Ref(Types::baseTypes valueType, iCatalogue* Catalogue, uint16_t DataOffset, uint32_t Count = 1) : type(valueType), ptr(Catalogue), offset(DataOffset),count(Count) {};
        Attribute_Ref(const Attribute_Ref& rhs) : type(rhs.type), ptr(rhs.ptr), offset(rhs.offset) {};
        virtual Types::baseTypes Type() const override { return type; };
        virtual void* Ptr() override { return (uint8_t*)(ptr->Ptr()) + offset; };
        virtual const void* Ptr() const override { return (uint8_t*)(ptr->Ptr()) + offset; };
        virtual void* Begin(uint64_t Index = 0) override { return (count > 1) ? (uint8_t*)Ptr() + (Types::SizeOf(Type())*Index): nullptr; };
        virtual const void* Begin(uint64_t Index = 0) const override { return (count > 1) ? (uint8_t*)Ptr() + (Types::SizeOf(Type()) * Index) : nullptr; };
        //virtual bool Remove(uint32_t Index) override { return false; };
        virtual uint64_t Count() const override { return (count > 1) ? count : 0; };


        friend Object;
    private:
        Types::baseTypes type;
        iCatalogue* ptr;
        uint16_t offset;
        uint32_t count;
    };

    class Primitive_List_Static_Ref : public NullObject {
    public:
        Primitive_List_Static_Ref(Types::baseTypes valueType, void* Data, uint64_t Count ) : type(valueType), data(Data), count(Count) {};
        Primitive_List_Static_Ref(const Primitive_List_Static_Ref& rhs) : type(rhs.type), data(rhs.data), count(rhs.count) {};
        virtual Types::baseTypes Type() const override { return type; };
        virtual void* Ptr() override { return nullptr; };
        virtual void* Begin(uint64_t Index = 0) override { return (uint8_t*)data+Index*Types::SizeOf(Type()); }; 
        virtual const void* Begin(uint64_t Index = 0) const override { return (uint8_t*)data + Index * Types::SizeOf(Type()); };
        virtual uint64_t Count() const override { return count; };

        friend Object;
    private:
        Types::baseTypes type;
        void* data;
        uint64_t count;
    };

    class Primitive_List_Ref : public NullObject {
    public:
        Primitive_List_Ref(Types::baseTypes valueType, iAllocator* Allocator, uint64_t BlockID) : type(valueType), allocatorPtr(Allocator), blockID(BlockID) {};
        Primitive_List_Ref(Types::baseTypes valueType, iBlockD& Block) : type(valueType), blockPtr(&Block), blockID(std::numeric_limits<uint64_t>::max()) {};
        Primitive_List_Ref(Types::baseTypes valueType, InlinePartition Partition) : type(valueType), partition(Partition), blockPtr(&partition), blockID(std::numeric_limits<uint64_t>::max()) {};
        Primitive_List_Ref(const Primitive_List_Ref& rhs) : type(rhs.type), blockPtr(rhs.blockPtr), blockID(rhs.blockID) {};

        virtual Types::baseTypes Type() const override { return type; };
        virtual void* Ptr() override { return nullptr; };
        virtual void* Begin(uint64_t Index = 0) override { return Block().Data(Index * UnitSize()); };
        virtual const void* Begin(uint64_t Index = 0) const override { return Block().Data(Index * UnitSize()); };

        virtual uint64_t Count() const override { return Block().Size() / UnitSize(); }
        virtual bool Remove(uint32_t Index) override;

        friend Object;
    protected:
        virtual bool pushBack() override;
        virtual bool pushFront() override;
        virtual bool insert(uint32_t Index) override;
    private:
        iBlockD& Block() { return (blockID < std::numeric_limits<uint64_t>::max()) ? (*allocatorPtr)[blockID] : *blockPtr; };
        const iBlockD& Block() const { return (blockID < std::numeric_limits<uint64_t>::max()) ? (*allocatorPtr)[blockID] : *blockPtr; };
        uint64_t UnitSize() const { return Types::SizeOf(Type()); };
        Types::baseTypes type;
        uint64_t blockID;
        InlinePartition partition;
        union {
            iAllocator* allocatorPtr;
            iBlockD* blockPtr;            
        };
        
    };






    
    namespace ObjectEnums {
        enum objectTypes : uint8_t {
            null,
            primitive,
            primitive_ref,
            attribute_ref,
            list,
            list_static,
            list_item,
            const_list_item
        };
    }

    class Object : public iCatalogue {
    public:
       
        Object() {};
        Object(Primitive& primative) : prim(&primative), wrapperType(ObjectEnums::objectTypes::primitive) {};

        Object(Primitive_Ref& primative) : ptr(primative), wrapperType(ObjectEnums::objectTypes::primitive_ref) {};
        Object(Types::baseTypes valueType, void* Ptr) : ptr(valueType, Ptr), wrapperType(ObjectEnums::objectTypes::primitive_ref) {};

        Object(Attribute_Ref& Attribute) : atr(Attribute), wrapperType(ObjectEnums::objectTypes::attribute_ref) {};
        Object(Types::baseTypes valueType, iCatalogue* Catalogue, uint16_t DataOffset, uint32_t Count = 1) : atr(valueType, Catalogue, DataOffset, Count), wrapperType(ObjectEnums::objectTypes::attribute_ref) {};
        

        Object(Primitive_List_Static_Ref& List) : list_ptr(List), wrapperType(ObjectEnums::objectTypes::list_static) {};
        Object(Types::baseTypes valueType, void* Ptr, uint64_t Count) : list_ptr(valueType, Ptr, Count), wrapperType(ObjectEnums::objectTypes::list_static) {};

        Object(Primitive_List_Ref& List) : list(List), wrapperType(ObjectEnums::objectTypes::list) {};
        Object(Types::baseTypes valueType, iAllocator* Allocator, uint64_t BlockID) : list(valueType, Allocator, BlockID), wrapperType(ObjectEnums::objectTypes::list) {};
        Object(Types::baseTypes valueType, iBlockD& Block) : list(valueType, Block), wrapperType(ObjectEnums::objectTypes::list) {};
        Object(Types::baseTypes valueType, InlinePartition Block) : list(valueType, Block), wrapperType(ObjectEnums::objectTypes::list) {};

        Object(ObjectListRef<iObjectList*>& List_Item) : list_item(List_Item), wrapperType(ObjectEnums::objectTypes::list_item) {};
        Object(iObjectList* list_ptr, uint64_t Index) : list_item(list_ptr, Index), wrapperType(ObjectEnums::objectTypes::list_item) {};
        Object(ObjectListRef<const iObjectList*>& List_Item) : const_list_item(List_Item), wrapperType(ObjectEnums::objectTypes::const_list_item) {};
        Object(const iObjectList* list_ptr, uint64_t Index) : const_list_item(list_ptr, Index), wrapperType(ObjectEnums::objectTypes::const_list_item) {};


        Object(const Object& rhs) {};
        Object(Object&& rhs) {};
        virtual Types::baseTypes Type() const override;
        virtual void* Ptr() override;
        virtual const void* Ptr() const override;
        virtual void* Begin(uint64_t Index = 0) override;
        virtual const void* Begin(uint64_t Index = 0) const override;
        virtual bool Remove(uint32_t Index) override;
        virtual uint64_t Count() const override;
        virtual iCatalogue& GetRef(const uint8_t* Key, uint8_t KeyLen) override;
        virtual iCatalogue& GetRefByIndex(uint64_t Index) override;
        virtual iCatalogue& CDRRef(uint64_t BaseIndex) override;
        virtual Object Get(const uint8_t* Key, uint8_t KeyLen) override;
        virtual Object GetByIndex(uint64_t Index) override;
        virtual Object CDR(uint64_t BaseIndex) override;
    protected:
        virtual bool pushBack() override;
        virtual bool pushFront() override;
        virtual bool insert(uint32_t Index) override;
    private:
        
        ObjectEnums::objectTypes wrapperType = ObjectEnums::null;
        union {
            Primitive* prim;
            Primitive_Ref ptr;
            Attribute_Ref atr;
            Primitive_List_Ref list;
            Primitive_List_Static_Ref list_ptr;   
            ObjectListRef<iObjectList*> list_item;
            ObjectListRef<const iObjectList*> const_list_item;
        };

        

    };

}


#endif