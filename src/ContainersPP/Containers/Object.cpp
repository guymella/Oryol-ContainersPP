#include "Object.h"

namespace ContainersPP {

	

	iCatalogue& NullObject::GetRef(const uint8_t* Key, uint8_t KeyLen)
	{
		return NULLOBJECT;
	}

	iCatalogue& NullObject::GetRefByIndex(uint64_t Index)
	{
		return NULLOBJECT;
	}

	iCatalogue& NullObject::CDRRef(uint64_t BaseIndex)
	{
		return NULLOBJECT;
	}

	Object NullObject::Get(const uint8_t* Key, uint8_t KeyLen)
	{
		return Object();
	}

	Object NullObject::GetByIndex(uint64_t Index)
	{
		return Object();
	}

	Object NullObject::CDR(uint64_t BaseIndex)
	{
		return Object();
	}




#define switchallObject(fn) switch (wrapperType)\
	{\
		case ObjectEnums::null:\
			return NULLOBJECT.fn;\
		case ObjectEnums::primitive:\
			return prim->fn;\
		case ObjectEnums::primitive_ref:\
			return ptr.fn;\
		case ObjectEnums::attribute_ref:\
			return atr.fn;\
		case ObjectEnums::list:\
			return list.fn;\
		case ObjectEnums::list_static:\
			return list_ptr.fn;\
		default:\
			break;\
	}

	Types::baseTypes Object::Type() const
	{
		switchallObject(Type())
		return NULLOBJECT.Type();
	}

	#define switchPrimObject(fn) switch (wrapperType)\
	{\
		case ObjectEnums::primitive:\
			return prim->fn;\
		case ObjectEnums::primitive_ref:\
			return ptr.fn;\
		case ObjectEnums::attribute_ref:\
			return atr.fn;\
		default:\
			break;\
	}

	void* Object::Ptr()
	{
		switchPrimObject(Ptr())
		return nullptr;
	}

	const void* Object::Ptr() const
	{
		switchPrimObject(Ptr())
		return nullptr;
	}

	#define switchListObject(fn) switch (wrapperType)\
	{\
		case ObjectEnums::list:\
			return list.fn;\
		case ObjectEnums::list_static:\
			return list_ptr.fn;\
		default:\
			break;\
	}

	void* Object::Begin(uint64_t Index)
	{
		switchListObject(Begin(Index))
		return nullptr;
	}

	const void* Object::Begin(uint64_t Index) const
	{
		switchListObject(Begin(Index))
		return nullptr;
	}

	bool Object::Remove(uint32_t Index)
	{
		switchListObject(Remove(Index))
		return nullptr;
	}

	uint64_t Object::Count() const
	{
		switchListObject(Count())
		return 0;
	}

	iCatalogue& Object::GetRef(const uint8_t* Key, uint8_t KeyLen)
	{
		return NULLOBJECT;
	}
	
	iCatalogue& Object::GetRefByIndex(uint64_t Index)
	{
		return NULLOBJECT;
	}

	iCatalogue& Object::CDRRef(uint64_t BaseIndex)
	{
		return NULLOBJECT;
	}

	Object Object::Get(const uint8_t* Key, uint8_t KeyLen)
	{
		return Object();
	}		

	Object Object::GetByIndex(uint64_t Index)
	{
		return Object();
	}

	Object Object::CDR(uint64_t BaseIndex)
	{
		return Object();
	}

	bool Object::pushBack()
	{
		switchListObject(pushBack())
		return false;
	}

	bool Object::pushFront()
	{
		switchListObject(pushFront())
		return false;
	}

	bool Object::insert(uint32_t Index)
	{
		switchListObject(insert(Index))
		return false;
	}

	bool Primitive_List_Ref::Remove(uint32_t Index)
	{
		if (Index < Count()) {
			Block().Remove(Index * UnitSize(), UnitSize());
		}
		return false;
	}

	bool Primitive_List_Ref::pushBack()
	{
		Block().CopyBack(Types::DefaultOf(Type()), UnitSize());
		return true;
	}

	bool Primitive_List_Ref::pushFront()
	{
		Block().CopyFront(Types::DefaultOf(Type()), UnitSize());
		return true;
	}

	bool Primitive_List_Ref::insert(uint32_t Index)
	{
		Block().CopyInsert(Index*UnitSize(),Types::DefaultOf(Type()), UnitSize());
		return true;
	}

}