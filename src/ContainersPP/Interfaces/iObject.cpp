#pragma once
//------------------------------------------------------------------------------


//#include "iObject.h"
#include "ContainersPP/Containers/Object.h"

namespace ContainersPP {


    static bool tryGet(ContainersPP::Types::baseTypes type, void* ptr, uint64_t& GetTo) {
        return false;
    }

#define getpos(t) if (*(t*)Ptr() >= 0) {\
    GetTo = *(t*)Ptr();\
    return true;\
    }\
    return false;

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
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            getpos(int16_t);
        case ContainersPP::Types::uint32:
            GetTo = *(uint32_t*)Ptr();
            return true;
        case ContainersPP::Types::int32:
            getpos(int32_t);
        case ContainersPP::Types::uint64:
            GetTo = *(uint64_t*)Ptr();
            return true;
        case ContainersPP::Types::int64:
            getpos(int64_t);
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
       
        return false;
    }

#define SetMax(t,tt) if (setFrom <= (tt)std::numeric_limits<t>::max()) {\
    *(t*)Ptr() = (t)setFrom;\
    return true;\
    }\
    return false;\

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
            SetMax(uint8_t,uint64_t);
        case ContainersPP::Types::int8:            
            SetMax(int8_t, uint64_t);
        case ContainersPP::Types::uint16:
            SetMax(uint16_t, uint64_t);
        case ContainersPP::Types::int16:
            SetMax(int16_t, uint64_t);
        case ContainersPP::Types::uint32:
            SetMax(uint32_t, uint64_t);
        case ContainersPP::Types::int32:
            SetMax(int32_t, uint64_t);
        case ContainersPP::Types::uint64:
            *(uint64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int64:
            SetMax(int64_t, uint64_t);
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
        return false;
    }



    /////////////////////
#define GetMax(t,tt) if (*(t*)Ptr() <= (t)std::numeric_limits<tt>::max()) {\
    GetTo = (tt) * (t*)Ptr();\
    return true;\
    }\
    return false;

#define GetPosMax(t,tt) if (*(t*)Ptr() <= (t)std::numeric_limits<tt>::max() && *(t*)Ptr()>=0) {\
    GetTo = (tt) * (t*)Ptr();\
    return true;\
    }\
    return false;

    template <>
    bool ContainersPP::iObject::GetValue<uint32_t>(uint32_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            getpos(int16_t);
        case ContainersPP::Types::uint32:
            GetTo = *(uint32_t*)Ptr();
            return true;
        case ContainersPP::Types::int32:
            getpos(int32_t);
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, uint32_t)
        case ContainersPP::Types::int64:
            GetPosMax(int64_t, uint32_t)
        case ContainersPP::Types::float32:
            GetPosMax(float_t, uint32_t)
        case ContainersPP::Types::float64:
            GetPosMax(double_t, uint32_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<uint32_t>(const uint32_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetMax(uint8_t, uint32_t);
        case ContainersPP::Types::int8:
            SetMax(int8_t, uint32_t);
        case ContainersPP::Types::uint16:
            SetMax(uint16_t, uint32_t);
        case ContainersPP::Types::int16:
            SetMax(int16_t, uint32_t);
        case ContainersPP::Types::uint32:
            *(uint32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int32:
            SetMax(int32_t, uint32_t);
        case ContainersPP::Types::uint64:
            *(uint64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }

    //////////////////
    template <>
    bool ContainersPP::iObject::GetValue<uint16_t>(uint16_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            getpos(int16_t);
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, uint16_t)
        case ContainersPP::Types::int32:
            GetPosMax(int32_t, uint16_t)
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, uint16_t)
        case ContainersPP::Types::int64:
            GetPosMax(int64_t, uint16_t)
        case ContainersPP::Types::float32:
            GetPosMax(float_t, uint16_t)
        case ContainersPP::Types::float64:
            GetPosMax(double_t, uint16_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<uint16_t>(const uint16_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetMax(uint8_t, uint16_t);
        case ContainersPP::Types::int8:
            SetMax(int8_t, uint16_t);
        case ContainersPP::Types::uint16:
            *(uint16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int16:
            SetMax(int16_t, uint16_t);
        case ContainersPP::Types::uint32:
            *(uint32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            *(uint64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }



    //////////////////
    template <>
    bool ContainersPP::iObject::GetValue<uint8_t>(uint8_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetMax(uint16_t, uint8_t)
        case ContainersPP::Types::int16:
            GetPosMax(int16_t, uint8_t)
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, uint8_t)
        case ContainersPP::Types::int32:
            GetPosMax(int32_t, uint8_t)
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, uint8_t)
        case ContainersPP::Types::int64:
            GetPosMax(int64_t, uint8_t)
        case ContainersPP::Types::float32:
            GetPosMax(float_t, uint8_t)
        case ContainersPP::Types::float64:
            GetPosMax(double_t, uint8_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<uint8_t>(const uint8_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            *(uint8_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int8:
            SetMax(int8_t, uint8_t);
        case ContainersPP::Types::uint16:
            *(uint16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int16:
            *(int16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint32:
            *(uint32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            *(uint64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }
    //////////////////
    template <>
    bool ContainersPP::iObject::GetValue<char>(char& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetMax(uint16_t, uint8_t)
        case ContainersPP::Types::int16:
            GetPosMax(int16_t, uint8_t)
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, uint8_t)
        case ContainersPP::Types::int32:
            GetPosMax(int32_t, uint8_t)
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, uint8_t)
        case ContainersPP::Types::int64:
            GetPosMax(int64_t, uint8_t)
        case ContainersPP::Types::float32:
            GetPosMax(float_t, uint8_t)
        case ContainersPP::Types::float64:
            GetPosMax(double_t, uint8_t)
        case ContainersPP::Types::chr: //todo:: check parse
            GetTo = *(char*)Ptr();
            return true;
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<char>(const char& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            *(uint8_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int8:
            SetMax(int8_t, uint8_t);
        case ContainersPP::Types::uint16:
            *(uint16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int16:
            *(int16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint32:
            *(uint32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            *(uint64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::float32:
            *(float_t*)Ptr() = (float_t)setFrom;
            return true;
        case ContainersPP::Types::float64:
            *(double_t*)Ptr() = (double_t)setFrom;
            return true;
        case ContainersPP::Types::chr: //todo:: check parse
            *(char*)Ptr() = setFrom;
            return true;
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
        return false;
    }
    
    ///////////////////////////////////////////////INT
    template <>
    bool ContainersPP::iObject::GetValue<int64_t>(int64_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            GetTo = *(int8_t*)Ptr();
            return true;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            GetTo = *(int16_t*)Ptr();
            return true;
        case ContainersPP::Types::uint32:
            GetTo = *(uint32_t*)Ptr();
            return true;
        case ContainersPP::Types::int32:
            GetTo = *(int32_t*)Ptr();
            return true;
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, int64_t);
        case ContainersPP::Types::int64:
            GetTo = *(int64_t*)Ptr();
            return true;
        case ContainersPP::Types::float32:
            GetTo = (int64_t)*(float_t*)Ptr();
                return true;
        case ContainersPP::Types::float64:
            GetTo = (int64_t)*(double_t*)Ptr();
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

        return false;
    }

#define  SetPos(t) if (setFrom >= 0) {\
    *(t*)Ptr() = (t)setFrom;\
    return true;\
    }\
    return false;

#define SetPosMax(t) if (setFrom >= 0 && setFrom <= std::numeric_limits<t>::max()) {\
    *(t*)Ptr() = (t)setFrom;\
    return true;\
    }\
    return false;

#define SetMinMax(t) if (setFrom >= std::numeric_limits<t>::min() && setFrom <= std::numeric_limits<t>::max()) {\
    *(t*)Ptr() = (t)setFrom;\
    return true;\
    }\
    return false;

    template<>
    bool ContainersPP::iObject::SetValue<int64_t>(const int64_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPosMax(uint8_t);
        case ContainersPP::Types::int8:
            SetMinMax(int8_t);
        case ContainersPP::Types::uint16:
            SetPosMax(uint16_t);
        case ContainersPP::Types::int16:
            SetMinMax(int16_t);
        case ContainersPP::Types::uint32:
            SetPosMax(uint32_t);
        case ContainersPP::Types::int32:
            SetMinMax(int32_t);
        case ContainersPP::Types::uint64:
            SetPos(uint64_t);            
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }



    /////////////////////
#define GetMaxMin(t,tt) if (*(t*)Ptr() <= std::numeric_limits<tt>::max() && *(t*)Ptr() >= std::numeric_limits<tt>::min()) {\
    GetTo = (tt) * (t*)Ptr();\
    return true;\
    }\
    return false;


    template <>
    bool ContainersPP::iObject::GetValue<int32_t>(int32_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            GetTo = *(int8_t*)Ptr();
            return true;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            GetTo = *(int16_t*)Ptr();
            return true;
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, int32_t)
        case ContainersPP::Types::int32:
            GetTo = *(int32_t*)Ptr();
            return true;
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, int32_t)
        case ContainersPP::Types::int64:
            GetMaxMin(int64_t, int32_t)
        case ContainersPP::Types::float32:
            GetMaxMin(float_t, int32_t)
        case ContainersPP::Types::float64:
            GetMaxMin(double_t, int32_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<int32_t>(const int32_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPosMax(uint8_t);
        case ContainersPP::Types::int8:
            SetMinMax(int8_t);
        case ContainersPP::Types::uint16:
            SetPosMax(uint16_t);
        case ContainersPP::Types::int16:
            SetMinMax(int16_t);
        case ContainersPP::Types::uint32:
            SetPos(uint32_t);
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            SetPos(uint64_t);
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }

    //////////////////
    template <>
    bool ContainersPP::iObject::GetValue<int16_t>(int16_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            GetTo = *(int8_t*)Ptr();
            return true;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            GetTo = *(int16_t*)Ptr();
            return true;
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, int16_t)
        case ContainersPP::Types::int32:
            GetMaxMin(int32_t, int16_t)
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, int16_t)
        case ContainersPP::Types::int64:
            GetMaxMin(int64_t, int16_t)
        case ContainersPP::Types::float32:
            GetMaxMin(float_t, int16_t)
        case ContainersPP::Types::float64:
            GetMaxMin(double_t, int16_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<int16_t>(const int16_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPosMax(uint8_t);
        case ContainersPP::Types::int8:
            SetMinMax(int8_t);
        case ContainersPP::Types::uint16:
            SetPos(uint16_t);
        case ContainersPP::Types::int16:
            *(int16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint32:
            SetPos(uint32_t);
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            SetPos(uint64_t);
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }



    //////////////////
    template <>
    bool ContainersPP::iObject::GetValue<int8_t>(int8_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            getpos(int8_t);
        case ContainersPP::Types::uint16:
            GetMax(uint16_t, uint8_t)
        case ContainersPP::Types::int16:
            GetPosMax(int16_t, uint8_t)
        case ContainersPP::Types::uint32:
            GetMax(uint32_t, uint8_t)
        case ContainersPP::Types::int32:
            GetPosMax(int32_t, uint8_t)
        case ContainersPP::Types::uint64:
            GetMax(uint64_t, uint8_t)
        case ContainersPP::Types::int64:
            GetPosMax(int64_t, uint8_t)
        case ContainersPP::Types::float32:
            GetPosMax(float_t, uint8_t)
        case ContainersPP::Types::float64:
            GetPosMax(double_t, uint8_t)
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<int8_t>(const int8_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPos(uint8_t);
        case ContainersPP::Types::int8:
            *(int8_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint16:
            SetPos(uint16_t);
        case ContainersPP::Types::int16:
            *(int16_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint32:
            SetPos(uint32_t);
        case ContainersPP::Types::int32:
            *(int32_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::uint64:
            SetPos(uint64_t);
        case ContainersPP::Types::int64:
            *(int64_t*)Ptr() = setFrom;
            return true;
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
        return false;
    }








    ///////////////////////////////////////////////Float
    template <>
    bool ContainersPP::iObject::GetValue<double_t>(double_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (*(bool*)Ptr()) ? 1 : 0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            GetTo = *(int8_t*)Ptr();
            return true;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            GetTo = *(int16_t*)Ptr();
            return true;
        case ContainersPP::Types::uint32:
            GetTo = *(uint32_t*)Ptr();
            return true;
        case ContainersPP::Types::int32:
            GetTo = *(int32_t*)Ptr();
            return true;
        case ContainersPP::Types::uint64:
            GetTo = (double_t)*(uint64_t*)Ptr();
            return true;
        case ContainersPP::Types::int64:
            GetTo = (double_t)*(int64_t*)Ptr();
            return true;
        case ContainersPP::Types::float32:
            GetTo = *(float_t*)Ptr();
            return true;
        case ContainersPP::Types::float64:
            GetTo = *(double_t*)Ptr();
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<double_t>(const double_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPosMax(uint8_t);
        case ContainersPP::Types::int8:
            SetMinMax(int8_t);
        case ContainersPP::Types::uint16:
            SetPosMax(uint16_t);
        case ContainersPP::Types::int16:
            SetMinMax(int16_t);
        case ContainersPP::Types::uint32:
            SetPosMax(uint32_t);
        case ContainersPP::Types::int32:
            SetMinMax(int32_t);
        case ContainersPP::Types::uint64:
            SetPosMax(uint64_t);
        case ContainersPP::Types::int64:
            SetMinMax(int64_t);
        case ContainersPP::Types::float32:
            SetMinMax(float_t);
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
        return false;
    }



    /////////////////////



    template <>
    bool ContainersPP::iObject::GetValue<float_t>(float_t& GetTo) const
    {
        if (!Ptr())
            return false;

        //Types::baseTypes z;
        switch (Type())
        {
        case ContainersPP::Types::boolean:
            GetTo = (float_t)(*(bool*)Ptr()) ? (float_t)1.0 : (float_t)0.0;
            return true;
            break;
        case ContainersPP::Types::uint8:
            GetTo = *(uint8_t*)Ptr();
            return true;
        case ContainersPP::Types::int8:
            GetTo = *(int8_t*)Ptr();
            return true;
        case ContainersPP::Types::uint16:
            GetTo = *(uint16_t*)Ptr();
            return true;
        case ContainersPP::Types::int16:
            GetTo = *(int16_t*)Ptr();
            return true;
        case ContainersPP::Types::uint32:
            GetTo = (float_t) *(uint32_t*)Ptr();
            return true;
        case ContainersPP::Types::int32:
            GetTo = (float_t) *(int32_t*)Ptr();
            return true;
        case ContainersPP::Types::uint64:
            GetTo = (float_t) *(uint64_t*)Ptr();
            return true;
        case ContainersPP::Types::int64:
            GetTo = (float_t) *(int64_t*)Ptr();
            return true;
        case ContainersPP::Types::float32:
            GetTo = *(float_t*)Ptr();
            return true;
        case ContainersPP::Types::float64:
            GetTo = (float_t) *(double_t*)Ptr();
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

        return false;
    }


    template<>
    bool ContainersPP::iObject::SetValue<float_t>(const float_t& setFrom)
    {
        if (!Ptr())
            return false;

        switch (Type())
        {
        case ContainersPP::Types::boolean:
            *(bool*)Ptr() = (setFrom) ? true : false;
            return true;
        case ContainersPP::Types::uint8:
            SetPosMax(uint8_t);
        case ContainersPP::Types::int8:
            SetMinMax(int8_t);
        case ContainersPP::Types::uint16:
            SetPosMax(uint16_t);
        case ContainersPP::Types::int16:
            SetMinMax(int16_t);
        case ContainersPP::Types::uint32:
            SetPosMax(uint32_t);
        case ContainersPP::Types::int32:
            SetMinMax(int32_t);
        case ContainersPP::Types::uint64:
            SetPosMax(uint64_t);
        case ContainersPP::Types::int64:
            SetMinMax(int64_t);
        case ContainersPP::Types::float32:
            *(float_t*)Ptr() = setFrom;
            return true;
        case ContainersPP::Types::float64:
            *(double_t*)Ptr() = setFrom;
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
    ///////////////////////////////////////////
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
    ////////////////////////////////////////////////////////
    template <>
    bool ContainersPP::iObjectList::PushFront(const uint32_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint32 && pushFront()) {
            *(uint32_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const uint32_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint32 && pushBack()) {
            *(uint32_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const uint32_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint32 && insert(Index)) {
            *(uint32_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////
    template <>
    bool ContainersPP::iObjectList::PushFront(const uint16_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint16 && pushFront()) {
            *(uint16_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const uint16_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint16 && pushBack()) {
            *(uint16_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const uint16_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint16 && insert(Index)) {
            *(uint16_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////

    template <>
    bool ContainersPP::iObjectList::PushFront(const uint8_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint8 && pushFront()) {
            *(uint8_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const uint8_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint8 && pushBack()) {
            *(uint8_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const uint8_t& setFrom)
    {
        if (Type() == Types::baseTypes::uint8 && insert(Index)) {
            *(uint8_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    ////////////////////////////////////////////////////////
     ///////////////////////////////////////////

    template <>
    bool ContainersPP::iObjectList::PushFront(const char& setFrom)
    {
        if (Type() == Types::baseTypes::chr && pushFront()) {
            *(char*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const char& setFrom)
    {
        if (Type() == Types::baseTypes::chr && pushBack()) {
            *(char*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const char& setFrom)
    {
        if (Type() == Types::baseTypes::chr && insert(Index)) {
            *(char*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    ////////////////////////////////////////////////////////
     ///////////////////////////////////////////

    template <>
    bool ContainersPP::iObjectList::PushFront(const int64_t& setFrom)
    {
        if (Type() == Types::baseTypes::int64 && pushFront()) {
            *(int64_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const int64_t& setFrom)
    {
        if (Type() == Types::baseTypes::int64 && pushBack()) {
            *(int64_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const int64_t& setFrom)
    {
        if (Type() == Types::baseTypes::int64 && insert(Index)) {
            *(int64_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    ////////////////////////////////////////////////////////
    template <>
    bool ContainersPP::iObjectList::PushFront(const int32_t& setFrom)
    {
        if (Type() == Types::baseTypes::int32 && pushFront()) {
            *(int32_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const int32_t& setFrom)
    {
        if (Type() == Types::baseTypes::int32 && pushBack()) {
            *(int32_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const int32_t& setFrom)
    {
        if (Type() == Types::baseTypes::int32 && insert(Index)) {
            *(int32_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////
    template <>
    bool ContainersPP::iObjectList::PushFront(const int16_t& setFrom)
    {
        if (Type() == Types::baseTypes::int16 && pushFront()) {
            *(int16_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const int16_t& setFrom)
    {
        if (Type() == Types::baseTypes::int16 && pushBack()) {
            *(int16_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const int16_t& setFrom)
    {
        if (Type() == Types::baseTypes::int16 && insert(Index)) {
            *(int16_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////

    template <>
    bool ContainersPP::iObjectList::PushFront(const int8_t& setFrom)
    {
        if (Type() == Types::baseTypes::int8 && pushFront()) {
            *(int8_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const int8_t& setFrom)
    {
        if (Type() == Types::baseTypes::int8 && pushBack()) {
            *(int8_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const int8_t& setFrom)
    {
        if (Type() == Types::baseTypes::int8 && insert(Index)) {
            *(int8_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    ///////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    template <>
    bool ContainersPP::iObjectList::PushFront(const float_t& setFrom)
    {
        if (Type() == Types::baseTypes::float32 && pushFront()) {
            *(float_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const float_t& setFrom)
    {
        if (Type() == Types::baseTypes::float32 && pushBack()) {
            *(float_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const float_t& setFrom)
    {
        if (Type() == Types::baseTypes::float32 && insert(Index)) {
            *(float_t*)Begin(Index) = setFrom;
            return true;
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////

    template <>
    bool ContainersPP::iObjectList::PushFront(const double_t& setFrom)
    {
        if (Type() == Types::baseTypes::float64 && pushFront()) {
            *(double_t*)Begin() = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::PushBack(const double_t& setFrom)
    {
        if (Type() == Types::baseTypes::float64 && pushBack()) {
            *(double_t*)Begin(Count() - 1) = setFrom;
            return true;
        }
        return false;
    }
    template<>
    bool iObjectList::Insert(uint32_t Index, const double_t& setFrom)
    {
        if (Type() == Types::baseTypes::float64 && insert(Index)) {
            *(double_t*)Begin(Index) = setFrom;
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
