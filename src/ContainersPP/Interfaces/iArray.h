#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iArray
    @ingroup Interfaces
    @brief interface for Procedural Memory inline Array
*/

//#include "iDynamic.h"
#include "iCountable.h"
#include "iIndexable.h"
#include "iIterable.h"
#include "iQueue.h"

#include <memory>


#include "ContainersPP/Types/Types.h"

namespace ContainersPP {

	template <typename TYPE>
	class iArray : public iCountable, public iREditable<TYPE>, public iCIterable<TYPE>, public iSearchable<TYPE>
	{
	public:
		//virtual size_t FindIndexLinear(const TYPE& elm, size_t startIndex = 0, size_t endIndex = std::numeric_limits<size_t>::max()) const override;
		/// read/write access to indexed item
		virtual TYPE& operator[](size_t index) override;
		/// read-only access to indexed item
		virtual const TYPE& operator[](size_t index) const override;
		virtual bool operator==(const iArray<TYPE>& rhs) const;
		//virtual iArray& operator<<(size_t amount);
		//virtual iArray& operator>>(size_t amount);
		virtual void MoveRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmmount) override;
		virtual void MoveRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmmount) override;
		/// Shuffle num elements fill gap by moving overritten ellements in.
		//virtual void ShuffleRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmmount) override;
		/// Shuffle num elements fill gap by moving overritten ellements in.
		//virtual void ShuffleRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmmount) override;
		/// Shift Elemnts Left with wrap
		//virtual void ShiftLeft(const size_t& shiftAmmount) override;
		///Shift ELements right with wrap
		//virtual void ShiftRight(const size_t& shiftAmmount) override;
		//CLear The Array
		virtual void Clear() override;
		/// find element index with slow linear search, return Size() if not found
		virtual size_t FindFirstIndexOf(const TYPE& elm, size_t startIndex = 0, size_t endIndex = std::numeric_limits<size_t>::max()) const override;
	};

	template <typename TYPE>
	class iArrayD : public iArray<TYPE>, public iDEditable<TYPE>, public iQueueDouble<TYPE> {
	public:
		//copy add element at front
		virtual TYPE& PushFront(const TYPE& elem) override = 0;
		/// move-add element to back of array
		virtual TYPE& PushFront(TYPE&& elm) override = 0;
		//Copy add element to back
		virtual TYPE& PushBack(const TYPE& elem) override = 0;
		/// move-add element to back of array
		virtual TYPE& PushBack(TYPE&& elm) override = 0;
		/// pop the last element
		virtual TYPE PopBack() override = 0;
		///remove num elements from back of array and return the last to be removed
		virtual TYPE PopBack(size_t numElements) override = 0;
		/// pop the first element
		virtual TYPE PopFront() override = 0;
		/// Pop num Elements from Front of Array and return last element popped
		virtual TYPE PopFront(size_t numElements) override = 0;
		/// copy-insert element at index, keep array order
		virtual void Insert(size_t index, const TYPE& elm) override = 0;
		/// move-insert element at index, keep array order
		virtual void Insert(size_t index, TYPE&& elm) override = 0;
		/// insert default initialized elements into array
		virtual void insertBlank(const size_t& index, size_t count = 1) override = 0;
		/// erase a range of elements, keep element order
		virtual void EraseRange(size_t index, size_t num = std::numeric_limits<size_t>::max()) override = 0;
		/*/// Shift num elements by shift ammount
		virtual void MoveRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount) override = 0;
		/// Shuffle num elements fill gap by moving overritten ellements in.
		virtual void ShuffleRange(size_t StartIndex, size_t numElements, const int64_t& shiftAmmount) override = 0;*/


		virtual TYPE& Front() override;
		/// read-only access to first element (must exist)
		virtual const TYPE& Front() const override;
		/// read/write access to last element (must exist)
		virtual TYPE& Back() override;
		/// read-only access to last element (must exist)
		virtual const TYPE& Back() const override;
		/// erase element at index, keep element order
		virtual TYPE Erase(size_t index) override;
		/// erase element at index, swap-in front or back element (destroys element ordering)
		virtual TYPE EraseSwap(size_t index) override;
		/// erase element at index, always swap-in from back (destroys element ordering)
		virtual TYPE EraseSwapBack(size_t index) override;
		/// erase element at index, always swap-in from front (destroys element ordering)
		virtual TYPE EraseSwapFront(size_t index) override;
		//Erase all elements from the array
		void Clear() override;

	};

	template <typename TYPE>
	class iDDArray : public iArrayD<TYPE>, public iDynamic {
	public:


	};
	template <typename TYPE>
	class iSparseArray : public iArrayD<TYPE>, public iSparse<TYPE> {
	public:


	};

	//template<class TYPE> 
	//size_t iArray<TYPE>::FindIndexLinear(const TYPE& elm, size_t startIndex, size_t endIndex) const {
	//	if (!Empty()) {
	//		if (endIndex > Size())
	//			endIndex = Size();
	//
	//		const TYPE* bgn = begin();
	//		for (size_t i = startIndex; i < endIndex; i++) {
	//			if (elm == bgn[i]) {
	//				return i;
	//			}
	//		}
	//	}
	//	// fallthrough: not found
	//	return 0;
	//}

	//------------------------------------------------------------------------------
	template<typename TYPE> TYPE&
		iArray<TYPE>::operator[](size_t index) {
		//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
		return begin()[index];
	}

	//------------------------------------------------------------------------------
	template<typename TYPE> const TYPE&
		iArray<TYPE>::operator[](size_t index) const {
		//o_assert_dbg(this->basePtr && (index >= 0) && (index < this->num));
		return begin()[index];
	}

	template<typename TYPE>
	inline bool iArray<TYPE>::operator==(const iArray<TYPE>& rhs) const
	{
		if (rhs.Size() != Size())
			return false;
		for (size_t i = 0; i < Size(); i++)
			if (!(rhs[i] == (*this)[i]))
				return false;
		return true;
	}

	template<typename TYPE>
	inline void iArray<TYPE>::MoveRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmount)
	{
		//TODO:: use Block moverange
		if ((int64_t)startIndex - shiftAmount < 0)
			startIndex += shiftAmount - startIndex;
		if (startIndex > Size())//will not Change this array
			return;
		if (startIndex + numElements > Size())
			numElements = Size() - startIndex; // rest of array

		memmove(begin(startIndex - shiftAmount), begin(startIndex), numElements * sizeof(TYPE));
	}
	//
	template<typename TYPE>
	inline void iArray<TYPE>::MoveRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmount)
	{
		//TODO:: use Block moverange
		if (startIndex + shiftAmount > Size())
			return;
		if (startIndex + numElements + shiftAmount > Size())
			numElements -= (startIndex + numElements + shiftAmount) - Size();

		memmove(begin(startIndex + shiftAmount), begin(startIndex), shiftAmount);

	}
	//
	//template<typename TYPE>
	//inline void iArray<TYPE>::ShuffleRangeLeft(size_t startIndex, size_t numElements, const size_t& shiftAmount)
	//{
	//
	//	size_t shuffleStart = (startIndex > shiftAmount) ? startIndex - shiftAmount : 0;
	//	size_t shuffleNum = startIndex - shuffleStart;
	//	size_t shuffleShift = numElements;
	//
	//	if (shuffleStart + shuffleShift > Size()) // will not change this array
	//		return;
	//
	//	if (shuffleStart + shuffleNum + shuffleShift > Size())
	//		shuffleNum -= (shuffleStart + shuffleNum + shuffleShift) - Size();
	//
	//    void* tmp = std::malloc(shuffleNum*sizeof(TYPE));
	//	std::memcpy(tmp, begin(shuffleStart), shuffleNum * sizeof(TYPE));
	//
	//	MoveRangeLeft(startIndex, numElements, shiftAmount);
	//	
	//	std::memcpy(begin(shuffleStart + shuffleShift), tmp, shuffleNum * sizeof(TYPE));
	//	std::free(tmp);
	//}
	//
	//template<typename TYPE>
	//inline void iArray<TYPE>::ShuffleRangeRight(size_t startIndex, size_t numElements, const size_t& shiftAmount)
	//{
	//	if (startIndex > Size())
	//		return;
	//
	//	size_t shuffleStart = startIndex + numElements;	
	//	size_t shuffleNum = shiftAmount;
	//	size_t shuffleShift = numElements;
	//
	//	if (shuffleStart > Size()) // nothing to shuffle
	//		return MoveRangeRight(startIndex, numElements, shiftAmount);
	//		
	//	if (shuffleStart + shuffleNum > Size())
	//		shuffleNum = Size() - shuffleStart;
	//
	//	void* tmp = std::malloc(shuffleNum * sizeof(TYPE));
	//	std::memcpy(tmp, begin(shuffleStart), shuffleNum * sizeof(TYPE));
	//
	//	MoveRangeRight(startIndex, numElements, shiftAmount);
	//
	//	std::memcpy(begin(startIndex), tmp, shuffleNum * sizeof(TYPE));
	//	std::free(tmp);
	//}



	template<typename TYPE>
	inline size_t iArray<TYPE>::FindFirstIndexOf(const TYPE& elm, size_t startIndex, size_t endIndex) const
	{
		const TYPE* e = end(endIndex);
		for (const TYPE* b = begin(startIndex); b < e; b++)
			if (elm == *b)
				return b - begin();
		return Size();
	}

	//------------------------------------------------------------------------------
	template<class TYPE> TYPE&
		iArrayD<TYPE>::Front() {
		//TODO:: handle empty
		return *begin();
	}

	//------------------------------------------------------------------------------
	template<class TYPE> const TYPE&
		iArrayD<TYPE>::Front() const {
		//TODO:: handle empty
		return *begin();
	}

	//------------------------------------------------------------------------------
	template<class TYPE> TYPE&
		iArrayD<TYPE>::Back() {
		//TODO:: handle empty
		TYPE* e = end();
		return *(--e);
	}

	//------------------------------------------------------------------------------
	template<class TYPE> const TYPE&
		iArrayD<TYPE>::Back() const {
		//TODO:: handle empty
		const TYPE* e = end();
		return *(--e);
	}

	//------------------------------------------------------------------------------
	template<class TYPE>
	TYPE iArrayD<TYPE>::Erase(size_t index) {
		TYPE tmp = begin()[index - 1];
		EraseRange(index, 1);
		return tmp;
	}

	//------------------------------------------------------------------------------
	template<class TYPE>
	TYPE iArrayD<TYPE>::EraseSwap(size_t index) {
		TYPE tmp = begin()[index - 1];
		EraseSwapBack(index);
		return tmp;
	}

	//------------------------------------------------------------------------------
	template<class TYPE>
	TYPE iArrayD<TYPE>::EraseSwapBack(size_t index) {
		TYPE tmp = begin()[index - 1];
		begin()[index] = this->PopBack();
		return tmp;
	}

	//------------------------------------------------------------------------------
	template<class TYPE>
	TYPE iArrayD<TYPE>::EraseSwapFront(size_t index) {
		TYPE tmp = begin()[index - 1];
		begin()[index - 1] = this->PopFront();
		return tmp;
	}

	template<typename TYPE>
	inline void iArrayD<TYPE>::Clear()
	{
		EraseRange(0, Size());
	}

	//template<typename TYPE>
	//inline void iArray<TYPE>::ShiftLeft(const size_t& shiftAmmount)
	//{
	//	ShuffleRangeLeft(shiftAmmount, Size() - shiftAmmount, shiftAmmount);
	//}
	//
	//template<typename TYPE>
	//inline void iArray<TYPE>::ShiftRight(const size_t& shiftAmmount)
	//{
	//	ShuffleRangeRight(0, Size() - shiftAmmount, shiftAmmount);
	//}

	template<typename TYPE>
	inline void iArray<TYPE>::Clear()
	{
		for (size_t i = 0; i < Size(); i++)
			begin()[i] = TYPE();
	}

}