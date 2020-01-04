#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::iTypeBuffer
    @ingroup Interfaces
    @brief interface for Type specific Buffer
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "iBuffer.h"
#include "iArray.h"

namespace ContainersPP {
    template<typename TYPE>
    class iTypeBlock : public iArrayD<TYPE>, public iCIterable<TYPE> {
    public:
        /// CopyAssign
        virtual void operator=(const iTypeBlock& rhs);
        // retrieve element at index
        virtual TYPE& operator[](uint64_t index) override;
        // retrieve element at index
        virtual const TYPE& operator[](uint64_t index) const override;
        /// get number of bytes in buffer
        virtual uint64_t Size() const override;



        //copy add element at front
        virtual TYPE& PushFront(const TYPE& elem) override;
        /// move-add element to back of array
        virtual TYPE& PushFront(TYPE&& elm) override;
        //Copy add element to back
        virtual TYPE& PushBack(const TYPE& elem) override;
        /// move-add element to back of array
        virtual TYPE& PushBack(TYPE&& elm) override;
        /// pop the last element
        virtual TYPE PopBack() override;
        ///remove num elements from back of array and return the last to be removed
        virtual TYPE PopBack(uint64_t numElements) override;
        /// pop the first element
        virtual TYPE PopFront() override;
        /// Pop num Elements from Front of Array and return last element popped
        virtual TYPE PopFront(uint64_t numElements) override;
        /// copy-insert element at index, keep array order
        virtual void Insert(uint64_t index, const TYPE& elm) override;
        /// move-insert element at index, keep array order
        virtual void Insert(uint64_t index, TYPE&& elm) override;
        /// insert default initialized elements into array
        virtual void InsertBlank(const uint64_t& index, uint64_t count = 1) override;
        /// erase a range of elements, keep element order
        virtual void EraseRange(uint64_t index, uint64_t num = std::numeric_limits<uint64_t>::max()) override;

               
        /// get read-only pointer to content (throws assert if would return nullptr)
        virtual const uint8_t* Data(uint64_t offset = 0) const;
        /// get read/write pointer to content (throws assert if would return nullptr)
        virtual uint8_t* Data(uint64_t offset = 0);

        /// add bytes to Block
        virtual void CopyBack(const uint8_t* data, uint64_t numElements);
        /// add uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddBack(uint64_t numElements);
        /// add bytes to front of Block
        virtual void CopyFront(const uint8_t* data, uint64_t numElements);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddFront(uint64_t numElements);
        /// copy bytes into Block move contents to make room
        virtual void CopyInsert(uint64_t offset, const uint8_t* data, uint64_t numElements);
        /// insert uninitialized bytes to Block, return pointer to start
        virtual uint8_t* AddInsert(uint64_t offset, uint64_t numElements);
        /// overwite bytes inside of Block
        virtual void CopyOver(uint64_t offset, uint64_t numReplace, const uint8_t* data, uint64_t numElements);
        /// add uninitialized bytes to front of Block, return pointer to start
        virtual uint8_t* AddOver(uint64_t offset, uint64_t numReplace, uint64_t numElements);
        /// remove a chunk of data from the Block, return number of bytes removed
        virtual uint64_t Remove(uint64_t offset, uint64_t numElements);
        /// clear the Block (deletes content, keeps capacity)
        virtual void Clear();

        /// C++ conform begin
        virtual TYPE* begin(const int64_t& offset = 0) override;
        /// C++ conform begin
        virtual const TYPE* begin(const int64_t& offset = 0) const override;
        /// C++ conform end
        virtual TYPE* end(const int64_t& offset = 0) override;
        /// C++ conform end
        virtual const TYPE* end(const int64_t& offset = 0) const override;
    protected:
        /// copy content into currently allocated buffer, bump size
        virtual void copy(const uint8_t* ptr, uint64_t numBytes, uint64_t offset = 0);

        virtual uint64_t UnitSize() const;
        virtual iBlockD& Buffer() = 0;
        virtual const iBlockD& Buffer() const = 0;
    };

    template<typename TYPE>
    class iTypeVector : public iTypeBlock<TYPE> {
    public:
        /// Force Allocate the buffer
        virtual void Allocate(uint64_t newCapacity);
        /// get capacity in bytes of buffer
        virtual uint64_t Capacity() const;
        /// get number of free bytes at back
        virtual uint64_t SpareBack() const;
        /// make room for N more bytes
        virtual void ReserveBack(uint64_t numElements); 
    protected:
        virtual iBufferV& Buffer() override = 0;
        virtual const iBufferV& Buffer() const override = 0;
    };

    template<typename TYPE>
    class iTypeBuffer : public iTypeVector<TYPE> {
    public:
        /// Force Allocate the buffer
        virtual void Allocate(uint64_t newCapacity, uint64_t frontSpare);
        /// get number of free bytes at front
        virtual uint64_t SpareFront() const;
        /// make room for N more bytes at front
        virtual void ReserveFront(uint64_t numElements); 

        // override to balance spare
        virtual TYPE EraseSwap(uint64_t index) override;
    protected:
        virtual iBufferD& Buffer() override = 0;
        virtual const iBufferD& Buffer() const override = 0;
    };

    template<typename TYPE>
    inline void iTypeVector<TYPE>::Allocate(uint64_t newCapacity)
    {
        Buffer().Allocate(newCapacity * UnitSize());
    }

    template<typename TYPE>
    uint64_t ContainersPP::iTypeVector<TYPE>::Capacity() const
    {
        o_assert_dbg((Buffer().Capacity() % UnitSize()) == 0);

        return Buffer().Capacity() / UnitSize();
    }

    template<typename TYPE>
    inline uint64_t iTypeVector<TYPE>::SpareBack() const
    {
        o_assert_dbg((Buffer().SpareBack() % UnitSize()) == 0);
        return Buffer().SpareBack() / UnitSize();
    }

    template<typename TYPE>
    inline void iTypeVector<TYPE>::ReserveBack(uint64_t numElements)
    {
        Buffer().ReserveBack(numElements * UnitSize());
    }

    template<typename TYPE>
    inline void iTypeBuffer<TYPE>::Allocate(uint64_t newCapacity, uint64_t frontSpare)
    {
        Buffer().Allocate(newCapacity * UnitSize(), UnitSize() * frontSpare);
    }

    template<typename TYPE>
    inline uint64_t iTypeBuffer<TYPE>::SpareFront() const
    {
        o_assert_dbg((Buffer().SpareFront() % UnitSize()) == 0);
        return Buffer().SpareFront() / UnitSize();
    }

    template<typename TYPE>
    inline void iTypeBuffer<TYPE>::ReserveFront(uint64_t numElements)
    {
        Buffer().ReserveFront(numElements * UnitSize());
    }

    template<typename TYPE>
    inline TYPE iTypeBuffer<TYPE>::EraseSwap(uint64_t index)
    {
        if (SpareFront() > SpareBack())
            return EraseSwapBack(index);
        return EraseSwapFront(index);
        
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::operator=(const iTypeBlock& rhs)
    {
        if (rhs.Size())
            copy(rhs.Data(), rhs.Size());
        else
            Clear();
    }

    template<typename TYPE>
    inline TYPE& iTypeBlock<TYPE>::operator[](uint64_t index)
    {
        return begin()[index];
    }

    template<typename TYPE>
    inline const TYPE& iTypeBlock<TYPE>::operator[](uint64_t index) const
    {
        return begin()[index];
    }

    template<typename TYPE>
    inline uint64_t iTypeBlock<TYPE>::Size() const
    {
        o_assert_dbg((Buffer().Size() % UnitSize()) == 0);
        return Buffer().Size() / UnitSize();
    }

    /*template<typename TYPE>
    inline bool iTypeBlock<TYPE>::Empty() const
    {
        return !Size();
    }*/

    template<typename TYPE>
    inline TYPE& iTypeBlock<TYPE>::PushFront(const TYPE& elem)
    {
        new (AddFront(1)) TYPE(elem);
        return Front();
    }

    template<typename TYPE>
    inline TYPE& iTypeBlock<TYPE>::PushFront(TYPE&& elm)
    {
        new (AddFront(1)) TYPE(std::move(elm));
        return Front();
    }

    template<typename TYPE>
    inline TYPE& iTypeBlock<TYPE>::PushBack(const TYPE& elem)
    {
        new (AddBack(1)) TYPE(elem);
        return Back();
    }

    template<typename TYPE>
    inline TYPE& iTypeBlock<TYPE>::PushBack(TYPE&& elm)
    {
        new (AddBack(1)) TYPE(std::move(elm));
        return Back();
    }

    template<typename TYPE>
    inline TYPE iTypeBlock<TYPE>::PopBack()
    {
        TYPE r = Back();
        Remove(Size() - 1, 1);
        return r;
    }

    template<typename TYPE>
    inline TYPE iTypeBlock<TYPE>::PopBack(uint64_t numElements)
    {
        TYPE r = Back();
        Remove(Size() - numElements, numElements);
        return r;
        return TYPE();
    }

    template<typename TYPE>
    inline TYPE iTypeBlock<TYPE>::PopFront()
    {
        TYPE r = Front();
        Remove(0, 1);
        return r;
    }

    template<typename TYPE>
    inline TYPE iTypeBlock<TYPE>::PopFront(uint64_t numElements)
    {
        TYPE r = Front();
        Remove(0, numElements);
        return r;
        return TYPE();
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::Insert(uint64_t index, const TYPE& elm)
    {
        new (AddInsert(index, 1)) TYPE(elm);
        //CopyInsert(index, (uint8_t*)(&elm), 1);
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::Insert(uint64_t index, TYPE&& elm)
    {
        new (AddInsert(index, 1)) TYPE(std::move(elm));
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::InsertBlank(const uint64_t& index, uint64_t count)
    {
        AddInsert(index, count);
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::EraseRange(uint64_t index, uint64_t num)
    {
        Remove(index, num);
    }

    template<typename TYPE>
    inline const uint8_t* iTypeBlock<TYPE>::Data(uint64_t offset) const
    {
        return Buffer().Data(offset * UnitSize());
    }

    template<typename TYPE>
    inline uint8_t* iTypeBlock<TYPE>::Data(uint64_t offset)
    {
        return Buffer().Data(offset * UnitSize());
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::CopyBack(const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyBack(data, numElements * UnitSize());
    }
    
    template<typename TYPE>
    inline void iTypeBlock<TYPE>::CopyFront(const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyFront(data, numElements * UnitSize());
    }
   
    template<typename TYPE>
    inline void iTypeBlock<TYPE>::CopyInsert(uint64_t offset, const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyInsert(offset * UnitSize(), data, numElements * UnitSize());
    }
   
    template<typename TYPE>
    inline void iTypeBlock<TYPE>::CopyOver(uint64_t offset, uint64_t numReplace, const uint8_t* data, uint64_t numElements)
    {
        Buffer().CopyOver(offset * UnitSize(), numReplace*UnitSize(), data, numElements * UnitSize());
    }
   
    
    template<typename TYPE>
    inline void iTypeBlock<TYPE>::copy(const uint8_t* ptr, uint64_t numElements, uint64_t offset)
    {
        Buffer().Clear();
        Buffer().AddBack((offset+ numElements)* UnitSize());
        TYPE* cpyPtr = (TYPE*)ptr;
        TYPE* myPtr = begin();
        for (uint64_t i = 0; i < offset; i++)
            new (myPtr++) TYPE();
        for (uint64_t i = offset; i < numElements; i++)
            new (myPtr++) TYPE(*cpyPtr++);
        //Buffer().CopyBack(ptr, numElements * UnitSize());
    }

    template<typename TYPE>
    inline uint64_t iTypeBlock<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }
    
    template<typename TYPE>
    inline uint8_t* iTypeBlock<TYPE>::AddBack(uint64_t numElements)
    {
        uint8_t* rtn = Buffer().AddBack(numElements * UnitSize());
        TYPE* bld = (TYPE*)rtn;
        for (uint64_t i = 0; i < numElements; i++) {
            new (bld) TYPE(); //construct elements
            ++bld;
        }
        return rtn;
    }

    template<typename TYPE>
    inline uint8_t* iTypeBlock<TYPE>::AddFront(uint64_t numElements)
    {
        uint8_t* rtn = Buffer().AddFront(numElements*UnitSize());
        TYPE* bld = (TYPE*)rtn;
        for (uint64_t i = 0; i < numElements; i++) {
            new (bld) TYPE(); //construct elements
            ++bld;
        }
        return rtn;
    }
    template<typename TYPE>
    inline uint8_t* iTypeBlock<TYPE>::AddInsert(uint64_t offset, uint64_t numElements)
    {
        uint8_t* rtn = Buffer().AddInsert(offset * UnitSize(), numElements * UnitSize());
        TYPE* bld = (TYPE*)rtn;
        for (uint64_t i = 0; i < numElements; i++) {
            new (bld) TYPE(); //construct elements
            ++bld;
        }
        return rtn;
    }

    template<typename TYPE>
    inline uint8_t* ContainersPP::iTypeBlock<TYPE>::AddOver(uint64_t offset, uint64_t numReplace, uint64_t numElements)
    {
        uint8_t* rtn = Buffer().AddOver(offset * UnitSize(), numReplace * UnitSize(), numElements * UnitSize());
        TYPE* bld = (TYPE*)rtn;
        for (uint64_t i = 0; i < numElements; i++) {
            new (bld) TYPE(); //construct elements
            ++bld;
        }
        return rtn;
    }
    template<typename TYPE>
    inline uint64_t iTypeBlock<TYPE>::Remove(uint64_t offset, uint64_t numElements)
    {
        #ifdef DEFENSE
            if (offset >= Size())
                return 0;        
            if (offset + numElements > Size())
                numElements = Size() - offset;
        #endif // DEFENSE
        o_assert_dbg(offset < Size());
        o_assert_dbg(offset + numElements <= Size());

        //deconstruct elements
        TYPE* dst = begin(offset);
        for (uint64_t i = 0; i < numElements; i++) {
            dst->~TYPE();
            ++dst;
        }
        Buffer().Remove(offset * UnitSize(), numElements * UnitSize());

        return numElements;
    }

    template<typename TYPE>
    inline void iTypeBlock<TYPE>::Clear()
    {
        uint64_t s = Size();
        if (s) {
            TYPE* dst = begin();
            for (uint64_t i = 0; i < s; i++) {
                dst->~TYPE();
                ++dst;
            }
        }
        Buffer().Clear();
    }

    template<typename TYPE>
    inline TYPE* iTypeBlock<TYPE>::begin(const int64_t& offset)
    {
        return (TYPE*)Data() + offset;
    }

    template<typename TYPE>
    inline const TYPE* iTypeBlock<TYPE>::begin(const int64_t& offset) const
    {
        return (TYPE*)Data() + offset;
    }

    template<typename TYPE>
    inline TYPE* iTypeBlock<TYPE>::end(const int64_t& offset)
    {
        return begin(Size() + offset);
    }

    template<typename TYPE>
    inline const TYPE* iTypeBlock<TYPE>::end(const int64_t& offset) const
    {
        return begin(Size() + offset);
    }

    /*template<typename TYPE>
    inline TYPE* iTypeBlock<TYPE>::begin(uint64_t offset)
    {
        return ((TYPE*)Data()) + offset;
    }

    template<typename TYPE>
    inline const TYPE* iTypeBlock<TYPE>::begin(uint64_t offset) const
    {
        return ((TYPE*)Data()) + offset;
    }*/

 /*   template<typename TYPE>
    inline uint64_t iTypeBlock<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }
    template<typename TYPE>
    inline uint64_t iTypeVector<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }
    template<typename TYPE>
    inline uint64_t iTypeBuffer<TYPE>::UnitSize() const
    {
        return sizeof(TYPE);
    }*/

} // namespace 
