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

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr())?1:0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            if (*(int8_t*)Ptr() >= 0) {
                GetTo = *(int8_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            if (*(int16_t*)Ptr() >= 0) {
                GetTo = *(int16_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::uint32:
            GetTo = *(uint32_t*)Ptr();
            return true;
            break;
        case ContainersPP::Types::int32:
            if (*(int32_t*)Ptr() >= 0) {
                GetTo = *(int32_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::uint64:
            GetTo = *(uint64_t*)Ptr();
            return true;
            break;
        case ContainersPP::Types::int64:
            if (*(int64_t*)Ptr() >= 0) {
                GetTo = *(int64_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::float32:
            if (*(float_t*)Ptr() >= 0) {
                GetTo = (uint64_t)*(float_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::float64:
            if (*(double_t*)Ptr() >= 0) {
                GetTo = (uint64_t) * (double_t*)Ptr();
                return true;
            }
            return false;
        case ContainersPP::Types::chr: //todo:: check parse
            break;
        case ContainersPP::Types::String: //todo:: check parse
            break;
        case ContainersPP::Types::SRef://todo:: check parse
            break;
        case ContainersPP::Types::PRef://todo:: check parse
            break;
        case ContainersPP::Types::LRef://todo:: check parse
            break;        
        default:
            break;
        }

        /*if (Type() == Types::baseTypes::uint8 || Type() == Types::baseTypes::uint16 || Type() == Types::baseTypes::uint32 || Type() == Types::baseTypes::uint64) {
            GetTo = *(uint64_t*)Ptr();
            return true;
        }*/
       
        return false;
    }

    template<>
    bool ContainersPP::iObject::SetValue<uint64_t>(const uint64_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            if (setFrom <= std::numeric_limits<uint8_t>::max()) {
                *(uint8_t*)Ptr() = (uint8_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::int8:            
            if (setFrom <= std::numeric_limits<int8_t>::max()) {
                *(int8_t*)Ptr() = (int8_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::uint16:
            if (setFrom <= std::numeric_limits<uint16_t>::max()) {
                *(uint16_t*)Ptr() = (uint16_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::int16:
            if (setFrom <= std::numeric_limits<int16_t>::max()) {
                *(int16_t*)Ptr() = (int16_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::uint32:
            if (setFrom <= std::numeric_limits<uint32_t>::max()) {
                *(uint32_t*)Ptr() = (uint32_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::int32:
            if (setFrom <= std::numeric_limits<int32_t>::max()) {
                *(int32_t*)Ptr() = (int32_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::uint64:
            *(uint8_t*)Ptr() = (int8_t)setFrom;
            return true;
        case ContainersPP::Types::int64:
            if (setFrom <= (uint64_t)std::numeric_limits<int64_t>::max()) {
                *(int64_t*)Ptr() = (int64_t)setFrom;
                return true;
            }
            return false;
        case ContainersPP::Types::float32:
            *(float_t*)Ptr() = (float_t)setFrom;
            return true;            
        case ContainersPP::Types::float64:
            *(double_t*)Ptr() = (double_t)setFrom;
            return true;
        case ContainersPP::Types::chr: //todo:: check parse
            break;
        case ContainersPP::Types::String: //todo:: check parse
            break;
        case ContainersPP::Types::SRef://todo:: check parse
            break;
        case ContainersPP::Types::PRef://todo:: check parse
            break;
        case ContainersPP::Types::LRef://todo:: check parse
            break;
        default:
            break;
        }

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
