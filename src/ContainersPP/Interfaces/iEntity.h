//------------------------------------------------------------------------------
//  iEntity.h
//------------------------------------------------------------------------------

#ifndef _IENTITY_DEF
#define _IENTITY_DEF

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

		Types::BitItr<uint8_t, 1> GetBool(uint64_t columnIndex);
		Types::BitItr<uint8_t, 1> GetBool(const Types::KeyString key) { return GetBool(Schema().FindIndex(key)); };
		Types::BitItr<uint8_t, 1> GetBool(const char* key) { return GetBool(Types::MakeKey(key)); };

		template<typename TYPE>
		void SetValue(uint64_t columnIndex, const TYPE& value);
		template<typename TYPE>
		void SetValue(const Types::KeyString key, const TYPE& value) { SetValue<TYPE>(Schema().FindIndex(key), value); };
		template<typename TYPE>
		void SetValue(const char* key, const TYPE& value) { SetValue<TYPE>(Types::MakeKey(key), value); };

		void SetNull(uint64_t columnIndex);
		void SetNull(const Types::KeyString key) { SetNull(Schema().FindIndex(key)); };
		void SetNull(const char* key) { SetNull(Types::MakeKey(key)); };

		//const iBlockD* GetVar(uint64_t columnIndex) const;
		iBlockD& GetVar(uint64_t columnIndex);
		iBlockD& GetVar(const Types::KeyString key) { return GetVar(Schema().FindIndex(key)); };
		iBlockD& GetVar(const char* key) { return GetVar(Types::MakeKey(key)); };

		//template<typename TYPE>
		//TODO:: const TypeBlockRef<TYPE> GetMulti(uint64_t columnIndex) const;
		template<typename TYPE>
		TypeBlockRef<TYPE> GetMulti(uint64_t columnIndex);
		template<typename TYPE>
		TypeBlockRef<TYPE> GetMulti(const Types::KeyString key) { return GetMulti<TYPE>(Schema().FindIndex(key)); };
		template<typename TYPE>
		TypeBlockRef<TYPE> GetMulti(const char* key) { return GetMulti<TYPE>(Types::MakeKey(key)); };

		iAllocator& GetMultiVar(uint64_t columnIndex);
		iAllocator& GetMultiVar(const Types::KeyString key) { return GetMultiVar(Schema().FindIndex(key)); };
		iAllocator& GetMultiVar(const char* key) { return GetMultiVar(Types::MakeKey(key)); };
		
		//void SetVar(uint64_t columnIndex, const iBlockD& value);
	protected:
		virtual const Types::Schema& Schema() const = 0;
		virtual iBlockD& MainBuffer() = 0;
		virtual uint8_t* Columnar(uint64_t index) = 0;
		virtual iBlockD& ColumnBuffer(uint64_t index) = 0;
		virtual iAllocator& ColumnAllocator(uint64_t index) = 0;
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

	template<typename TYPE>
	inline void iEntity::SetValue(uint64_t columnIndex, const TYPE& value)
	{
		TYPE* ptr = (TYPE*)Schema().SetColumnValue(columnIndex, MainBuffer());
		new (ptr) TYPE(value);
	}

	template<typename TYPE>
	inline TypeBlockRef<TYPE> iEntity::GetMulti(uint64_t columnIndex)
	{
		o_assert_dbg(Schema().GetTypeDescr(columnIndex).getTypeSequence() >= Types::TypeSequence::Multi && Schema().GetTypeDescr(columnIndex).getTypeSequence() < Types::TypeSequence::Var);
		return TypeBlockRef<TYPE>(&ColumnBuffer(Schema().GetOffset(columnIndex)));
	}

	Types::BitItr<uint8_t, 1> ContainersPP::iEntity::GetBool(uint64_t columnIndex)
	{
		return Schema().GetBoolPointer(columnIndex, MainBuffer());
	}

	inline void iEntity::SetNull(uint64_t columnIndex)
	{
		Schema().SetColumnNull(columnIndex, MainBuffer());
	}

	inline iBlockD& iEntity::GetVar(uint64_t columnIndex)
	{
		o_assert_dbg(Schema().GetTypeDescr(columnIndex).getTypeSequence() == Types::TypeSequence::Var || Schema().GetTypeDescr(columnIndex).getTypeSequence() == Types::TypeSequence::CachedVar);
		return ColumnBuffer(Schema().GetOffset(columnIndex));
	}

	inline iAllocator& iEntity::GetMultiVar(uint64_t columnIndex)
	{
		return ColumnAllocator(Schema().GetOffset(columnIndex));
	}


}//contanersPP

#endif