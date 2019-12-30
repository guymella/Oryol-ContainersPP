#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iCountable
    @ingroup Interfaces
    @brief interface for structures that have countable size
*/

#include "ContainersPP/Types/Types.h"

namespace ContainersPP {

    class iCountable {
    public:
        virtual uint64_t Size() const = 0;
        virtual bool Empty() const { return !Size(); };
    };

}
