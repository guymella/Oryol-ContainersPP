#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Buffer
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"


namespace ContainersPP {

    template <uint64 CAPACITY>
    class BufferStatic {
    public:
        /// default constructor
        BufferStatic();
        /// move constructor
        BufferStatic(BufferStatic&& rhs);
        /// destructor
        ~BufferStatic();

        /// always force move-construct
        BufferStatic(const BufferStatic& rhs) = delete;
        /// always force move-assign
        void operator=(const BufferStatic& rhs) = delete;

        /// move-assignment
        void operator=(BufferStatic&& rhs);

        /// get number of bytes in buffer
        int Size() const;
        /// return true if empty
        bool Empty() const;
        /// get capacity in bytes of buffer
        int Capacity() const;
        /// get number of free bytes at back
        int Spare() const;

        /// make room for N more bytes
        void Reserve(int numBytes);
        /// add bytes to buffer
        void Add(const uint8_t* data, int numBytes);
        /// add uninitialized bytes to buffer, return pointer to start
        uint8_t* Add(int numBytes);
        /// remove a chunk of data from the buffer, return number of bytes removed
        int Remove(int offset, int numBytes);
        /// clear the buffer (deletes content, keeps capacity)
        void Clear();
        /// get read-only pointer to content (throws assert if would return nullptr)
        const uint8_t* Data() const;
        /// get read/write pointer to content (throws assert if would return nullptr)
        uint8_t* Data();

    private:
        /// destroy buffer
        void destroy();
        /// append-copy content into currently allocated buffer, bump size
        void copy(const uint8_t* ptr, int numBytes);

        int size;
        //int capacity;
        uint8_t data[CAPACITY];
    };

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline BufferStatic<CAPACITY>::BufferStatic () :
        size(0),
        data(nullptr) 
    {
        // empty
    }

    //------------------------------------------------------------------------------
    //template <uint64 CAPACITY>
    //inline
    //    BufferStatic<CAPACITY> ::BufferStatic (BufferStatic && rhs) :
    //    size(rhs.size),
    //    data(rhs.data) {
    //    rhs.size = 0;
    //    rhs.capacity = 0;
    //    rhs.data = nullptr;
    //}

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline
        BufferStatic<CAPACITY>::~BufferStatic () {
        this->destroy();
    }

    //------------------------------------------------------------------------------
   /* template <uint64 CAPACITY>
    inline void
        BufferStatic ::alloc(int newCapacity) {
        o_assert_dbg(newCapacity > this->capacity);
        o_assert_dbg(newCapacity > this->size);

        uint8_t* newBuf = (uint8_t*)Oryol::Memory::Alloc(newCapacity);
        if (this->size > 0) {
            o_assert_dbg(this->data);
            Oryol::Memory::Copy(this->data, newBuf, this->size);
        }
        if (this->data) {
            Oryol::Memory::Free(this->data);
        }
        this->data = newBuf;
        this->capacity = newCapacity;
    }*/

    ////------------------------------------------------------------------------------
    //template <uint64 CAPACITY>
    //inline void
    //    BufferStatic ::destroy() {
    //    if (this->data) {
    //        Oryol::Memory::Free(this->data);
    //    }
    //    this->data = nullptr;
    //    this->size = 0;
    //    this->capacity = 0;
    //}

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline void
        BufferStatic<CAPACITY>::copy(const uint8_t* ptr, int numBytes) {
        // NOTE: it is valid to call copy with numBytes==0
        o_assert_dbg(this->data);
        o_assert_dbg((this->size + numBytes) <= this->capacity);
        Oryol::Memory::Copy(ptr, this->data + this->size, numBytes);
        this->size += numBytes;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline void
        BufferStatic<CAPACITY>::operator=(BufferStatic && rhs) {
        this->destroy();
        this->size = rhs.size;
        this->capacity = rhs.capacity;
        this->data = rhs.data;
        rhs.size = 0;
        rhs.capacity = 0;
        rhs.data = nullptr;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline int
        BufferStatic<CAPACITY>::Size() const {
        return this->size;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline bool
        BufferStatic<CAPACITY>::Empty() const {
        return 0 == this->size;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline int
        BufferStatic<CAPACITY>::Capacity() const {
        return CAPACITY;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline int
        BufferStatic<CAPACITY>::Spare() const {
        return CAPACITY - this->size;
    }

    //------------------------------------------------------------------------------
    //template <uint64 CAPACITY>
    //inline void
    //    BufferStatic ::Reserve(int numBytes) {
    //    // need to grow?
    //    if ((this->size + numBytes) > this->capacity) {
    //        const int newCapacity = this->size + numBytes;
    //        this->alloc(newCapacity);
    //    }
    //}

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline void
        BufferStatic<CAPACITY>::Add(const uint8_t* data, int numBytes) {
        this->Reserve(numBytes);
        this->copy(data, numBytes);
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline uint8_t*
        BufferStatic<CAPACITY>::Add(int numBytes) {
        this->Reserve(numBytes);
        uint8_t* ptr = this->data + this->size;
        this->size += numBytes;
        return ptr;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline void
        BufferStatic<CAPACITY>::Clear() {
        this->size = 0;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline int
        BufferStatic<CAPACITY>::Remove(int offset, int numBytes) {
        o_assert_dbg(offset >= 0);
        o_assert_dbg(numBytes >= 0);
        if (offset >= this->size) {
            return 0;
        }
        if ((offset + numBytes) >= this->size) {
            numBytes = this->size - offset;
        }
        o_assert_dbg((offset + numBytes) <= this->size);
        o_assert_dbg(numBytes >= 0);
        if (numBytes > 0) {
            int bytesToMove = this->size - (offset + numBytes);
            if (bytesToMove > 0) {
                Oryol::Memory::Move(this->data + offset + numBytes, this->data + offset, bytesToMove);
            }
            this->size -= numBytes;
            o_assert_dbg(this->size >= 0);
        }
        return numBytes;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline const uint8_t*
        BufferStatic<CAPACITY>::Data() const {
        o_assert(data);
        return data;
    }

    //------------------------------------------------------------------------------
    template <uint64 CAPACITY>
    inline uint8_t*
        BufferStatic<CAPACITY>::Data() {
        o_assert(data);
        return data;
    }

} // namespace Oryol
