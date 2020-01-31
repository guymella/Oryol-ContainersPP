#pragma once
//------------------------------------------------------------------------------


#include "iObject.h"

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


    ObjectListRef<iObjectList*> iObjectList::GetIndex(uint64_t Index)
    {
        return ObjectListRef<iObjectList*>(this, Index);
    }
    ObjectListRef<const iObjectList*> iObjectList::GetIndex(uint64_t Index) const
    {
        return ObjectListRef<const iObjectList*>(this, Index);
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
}
