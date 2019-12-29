#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Buffer
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"

#include "ContainersPP/Interfaces/iBlock.h"

namespace ContainersPP {

    template <uint64_t SIZE>
    class BlockStatic : public iBlockS {
    public:
        /// default constructor
        BlockStatic() {};
        /// Copy Constructor
        BlockStatic(const BlockStatic& rhs);
        
        /// get number of bytes in buffer
        virtual uint64_t Size() const override;
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const override;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0) override;

    private:
        uint8_t data[SIZE];
    };
    
    template<uint64_t SIZE>
    inline BlockStatic<SIZE>::BlockStatic(const BlockStatic& rhs)
    {
        copy(rhs.Data(), rhs.Size());
    }

    //------------------------------------------------------------------------------
    template <uint64_t SIZE>
    inline uint64_t
        BlockStatic<SIZE>::Size() const {
        return SIZE;
    }

    //------------------------------------------------------------------------------
    template <uint64_t SIZE>
    inline const uint8_t*
        BlockStatic<SIZE>::Data(uint64_t offset) const {
        o_assert(data);
        return data + offset;
    }

    //------------------------------------------------------------------------------
    template <uint64_t SIZE>
    inline uint8_t*
        BlockStatic<SIZE>::Data(uint64_t offset) {
        o_assert(data);
        return data + offset;
    }

} // namespace Oryol
