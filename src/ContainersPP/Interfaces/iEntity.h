//------------------------------------------------------------------------------
//  iEntity.h
//------------------------------------------------------------------------------

#include "ContainersPP/Types/Schema.h"
//#include "ContainersPP/Types/BitPointer.h"
#include "TypeBuffer.h"
#include "Allocator.h"

namespace ContainersPP {

	class iEntity {
	public:
		//template<typename TYPE>
		//const TYPE* GetValue(uint64_t columnIndex) const;
		template<typename TYPE>
		TYPE* GetValue(uint64_t columnIndex);		
		template<typename TYPE>
		TYPE* GetValue(const Types::KeyString key) { return GetValue<TYPE>(Schema().FindIndex(key)); };
		template<typename TYPE>
		TYPE* GetValue(const char* key) { return GetValue<TYPE>(Types::MakeKey(key)); };
		//const iBlockD* GetVar(uint64_t columnIndex) const;
		//iBlockD* GetVar(uint64_t columnIndex);

		//template<typename TYPE>
		//TODO:: const TypeBlockRef<TYPE> GetArray(uint64_t columnIndex) const;

		//template<typename TYPE>
		//void SetValue(uint64_t columnIndex, const TYPE& value);
		//void SetVar(uint64_t columnIndex, const iBlockD& value);
	protected:
		virtual const Types::Schema& Schema() const = 0;
		virtual iBlockD& MainBuffer() = 0;
		virtual uint8_t* Columnar(uint64_t index) = 0;
		virtual iBlockD& ColumnBuffer(uint64_t index) = 0;
	};

	template<typename TYPE>
	inline TYPE* iEntity::GetValue(uint64_t columnIndex)
	{
		Types::TypeDescr td = Schema().GetTypeDescr(columnIndex);
		Types::TypeSequence ts = td.getTypeSequence();
		if (ts < Types::TypeSequence::Columnar) { // is in mainblock
			//if column is bool it will return null
			return (TYPE*)Schema().GetValuePointer(columnIndex, MainBuffer());
		}
		else if(ts < Types::TypeSequence::Multi) {// is in columnar
			return (TYPE*)Columnar(Schema().GetOffset(columnIndex));
		}
		//not a singular value or is an uncached value
		o_assert_dbg(false); //
		return nullptr;
	}


}//contanersPP