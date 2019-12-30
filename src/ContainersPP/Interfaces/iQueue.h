#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iQueue
    @ingroup Interfaces
    @brief interface for structures that can access front and back
*/


template <typename TYPE>
class iQueueReadFront {
public:
	virtual TYPE& Front() = 0;
	virtual const TYPE& Front() const = 0;
};

template <typename TYPE>
class iQueueReadBack {
public:
	virtual TYPE& Back() = 0;
	virtual const TYPE& Back() const = 0;
};

template <typename TYPE>
class iQueueRead : public iQueueReadFront<TYPE>, public iQueueReadBack<TYPE> {};

template <typename TYPE>
class iQueueEnFront {
public:
	//copy add element at front
	virtual TYPE& PushFront(const TYPE& elem) = 0;
	/// move-add element to back of array
	virtual TYPE& PushFront(TYPE&& elm) = 0;
	
};

template <typename TYPE>
class iQueueEnBack {
public:
	//Copy add element to back
	virtual TYPE& PushBack(const TYPE& elem) = 0;
	/// move-add element to back of array
	virtual TYPE& PushBack(TYPE&& elm) = 0;
};

template <typename TYPE>
class iQueueDeFront {
public:
	/// pop the first element
	virtual TYPE PopFront() = 0;
	/// Pop num Elements from Front of Array and return last element popped
	virtual TYPE PopFront(size_t numElements) = 0;
};

template <typename TYPE>
class iQueueDeBack {
public:
	/// pop the last element
	virtual TYPE PopBack() = 0;
	//remove num elements from back of array and return the last to be removed
	virtual TYPE PopBack(size_t numElements) = 0;
};

template <typename TYPE>
class iQueue : public iQueueEnBack<TYPE>, public iQueueDeFront<TYPE>, public iQueueRead<TYPE> {};

template <typename TYPE>
class iQueueDouble : public iQueue<TYPE> , public iQueueEnFront<TYPE>, public iQueueDeBack<TYPE> {};
