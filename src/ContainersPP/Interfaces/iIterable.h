#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Interfaces::iIterable
    @ingroup Interfaces
    @brief interface for structures that can be Iterated
*/

//#include "Structures/Types/Iterator.h"

template <typename TYPE>
class iCIterable {
public:
	/// C++ conform begin
	virtual TYPE* begin(const int64_t& offset = 0) = 0;
	/// C++ conform begin
	virtual const TYPE* begin(const int64_t& offset = 0) const = 0;
	/// C++ conform end
	virtual TYPE* end(const int64_t& offset = 0) = 0;
	/// C++ conform end
	virtual const TYPE* end(const int64_t& offset = 0) const = 0;
};

//
//template <typename TYPE>
//class iIterable {
//public:
//	/// C++ conform begin
//	virtual Itr<TYPE> begin(const int64_t& offset = 0) = 0;
//	/// C++ conform begin
//	virtual Itr<const TYPE> begin(const int64_t& offset = 0) const = 0;
//	/// C++ conform end
//	virtual Itr<TYPE> end(const int64_t& offset = 0) = 0;
//	/// C++ conform end
//	virtual Itr<const TYPE> end(const int64_t& offset = 0) const = 0;
//};

