#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iObject
    @ingroup Interfaces
    @brief interface for abstract object manipulation
*/

#ifndef Included_iObject_H
#define Included_iObject_H

#include "ContainersPP/Types/Types.h"
#include "ContainersPP/Types/KeyString.h"

namespace ContainersPP {

    class iObject {
    public:
        virtual Types::baseTypes Type() const = 0;
        virtual void* Ptr() = 0;
        virtual const void* Ptr() const = 0;
        template <typename TYPE>
        bool GetValue(TYPE& GetTo) const;
        template <typename TYPE>
        bool SetValue(const TYPE& setFrom);
    };

    template <typename ObjectListPtr>
    class ObjectListRef;   

    class Object;

    class iObjectList : public iObject {
    public:
        virtual void* Begin(uint64_t Index = 0) = 0;
        virtual const void* Begin(uint64_t Index = 0) const = 0;
        //virtual void* Ptr(uint64_t Index) = 0;
        virtual bool Remove(uint32_t Index) = 0;
        virtual uint64_t Count() const = 0;
        Object GetIndex(uint64_t Index);
        Object GetIndex(uint64_t Index) const;
        template <typename TYPE>
        bool GetIndexValue(uint32_t Index, TYPE& GetTo) const;
        template <typename TYPE>
        bool SetIndexValue(uint32_t Index, const TYPE& setFrom);
        template <typename TYPE>
        bool PushFront(const TYPE& setFrom);
        template <typename TYPE>
        bool PushBack(const TYPE& setFrom);
        template <typename TYPE>
        bool Insert(uint32_t Index, const TYPE& setFrom);
    protected:
        virtual bool pushBack() = 0;
        virtual bool pushFront() = 0;
        virtual bool insert(uint32_t Index) = 0;
    };

    template <typename ObjectListPtr>
    class ObjectListRef : public iObject {
    public:
        ObjectListRef(ObjectListPtr List, uint64_t Index) : list(List), index(Index) {};
        virtual Types::baseTypes Type() const override { return list->Type(); };
        virtual void* Ptr() override { return const_cast<void*>(list->Begin(index)); };
        virtual const void* Ptr() const override { return list->Begin(index); };
    private:
        ObjectListPtr list = 0;
        uint64_t index = 0;
    };

    

    class iCatalogue : public iObjectList {
    public:
        iCatalogue& GetRef(const Types::KeyString& key);
        iCatalogue& GetRef(const Types::KeyChain& keys, uint32_t keyindex = 0);
        iCatalogue& GetRef(const char* key);

        virtual iCatalogue& GetRef(const uint8_t* Key, uint8_t KeyLen) = 0;
        virtual iCatalogue& GetRefByIndex(uint64_t Index) = 0;
        
        Object Get(const Types::KeyString& key);
        Object Get(const Types::KeyChain& keys, uint32_t keyindex = 0);
        Object Get(const char* key);

        virtual Object Get(const uint8_t* Key, uint8_t KeyLen) = 0;
        virtual Object GetByIndex(uint64_t Index) = 0;

        virtual iCatalogue& CDRRef(uint64_t BaseIndex) = 0;
        virtual Object CDR(uint64_t BaseIndex) = 0;
    };

    template<typename TYPE>
    inline bool iObjectList::GetIndexValue(uint32_t Index, TYPE& GetTo) const
    {
        if (Index < Count()) {
            ObjectListRef<const iObjectList*> ref(this, Index);
            return ref.GetValue(GetTo);
        }
        return false;
    }

    template<typename TYPE>
    inline bool iObjectList::SetIndexValue(uint32_t Index, const TYPE& setFrom)
    {
        if (Index < Count()) {
            ObjectListRef<iObjectList*> ref(this, Index);
            return ref.SetValue(setFrom);
        }
        return false;
    }

    class iProfile {
    public:
        virtual iCatalogue& operator[](uint64_t index) = 0;
        virtual const iCatalogue& operator[](uint64_t index) const = 0;
        virtual uint64_t New() = 0;
        virtual bool Delete(uint64_t ID) = 0;
    };

}
#endif