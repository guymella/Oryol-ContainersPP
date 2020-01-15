//------------------------------------------------------------------------------
//  iAllocator.h
//------------------------------------------------------------------------------

#ifndef _IALLOCATOR_DEF
#define _IALLOCATOR_DEF
#include "iBlock.h"

namespace ContainersPP {

	class iAllocator {
	public:
		virtual iBlockD& operator[](uint64_t index) = 0;
		virtual const iBlockD& operator[](uint64_t index) const = 0;

		virtual uint64_t New() = 0;
		virtual uint64_t New(uint64_t newSize) = 0;

		/// get number of buffers
		virtual uint64_t Count() const = 0;
		/// return true if empty
		bool Empty() const { return !Count(); };
	};

	

}//contanersPP

#endif