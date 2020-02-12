#pragma once
//------------------------------------------------------------------------------


//#include "iObject.h"
#include "ContainersPP/Containers/Object.h"

namespace ContainersPP {

    template <>
    bool ContainersPP::iObject::GetValue<uint64_t>(uint64_t& GetTo) const
    {
        if (!Ptr())
            return false;

        if (Type() == Types::baseTypes::uint8 || Type() == Types::baseTypes::uint16 || Type() == Types::baseTypes::uint32 || Type() == Types::baseTypes::uint64) {
            GetTo = *(uint64_t*)Ptr();
            return true;
        }
        return false;
    }

    template<>
    bool ContainersPP::iObject::SetValue<uint64_t>(const uint64_t& setFrom)
    {
        if (!Ptr())
            return false;

        if (Type() == Types::baseTypes::uint8 || Type() == Types::baseTypes::uint16 || Type() == Types::baseTypes::uint32 || Type() == Types::baseTypes::uint64) {
            *(uint64_t*)Ptr() = setFrom;
            return true;
        }
        return false;
    }









    /////////////////Object list


    Object iObjectList::GetIndex(uint64_t Index)
    {
        return Object(this, Index);// ObjectListRef<iObjectList*>(this, Index);
    }
    Object iObjectList::GetIndex(uint64_t Index) const
    {
        return Object(this, Index); //ObjectListRef<const iObjectList*>(this, Index);
    }
    ;

    template <>
    bool ContainersPP::iObjectList::PushFront(const uint64_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint64 && pushFront()) {
            *(uint64_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const uint64_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint64 && pushBack()) {
            *(uint64_t*)Begin(Count()-1) = setFrom;
            return true;
        }
        return false;
    }
    template<> 
    bool iObjectList::Insert(uint32_t Index, const uint64_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint64 && insert(Index)) {
            *(uint64_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
	iCatalogue& iCatalogue::GetRef(const Types::KeyString& key)
	{
        return GetRef(key.Data(), (uint8_t)key.Size());
	}
	iCatalogue& iCatalogue::GetRef(const Types::KeyChain& keys, uint32_t keyindex)
	{
        if(keyindex == keys.Size()-1)
            return GetRef(keys[keyindex].Key.get().Data(), (uint8_t)keys[keyindex].Key.get().Size());
        return GetRef(keys[keyindex].Key.get().Data(), (uint8_t)keys[keyindex].Key.get().Size()).GetRef(keys,keyindex+1);
	}
	iCatalogue& iCatalogue::GetRef(const char* key)
	{
        return GetRef((const uint8_t*)key, (uint8_t)std::strlen(key));
	}
	Object iCatalogue::Get(const Types::KeyString& key)
	{
        return Get(key.Data(), (uint8_t)key.Size());
	}
	Object iCatalogue::Get(const Types::KeyChain& keys, uint32_t keyindex)
	{
        if (keyindex == keys.Size() - 1)
            return Get(keys[keyindex].Key.get().Data(), (uint8_t)keys[keyindex].Key.get().Size());
        return GetRef(keys[keyindex].Key.get().Data(), (uint8_t)keys[keyindex].Key.get().Size()).Get(keys, keyindex + 1);
	}
	Object iCatalogue::Get(const char* key)
	{
        return Get((const uint8_t*)key, (uint8_t)std::strlen(key));
	}
}
