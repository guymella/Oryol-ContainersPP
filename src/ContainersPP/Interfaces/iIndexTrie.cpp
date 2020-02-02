#include "iIndexTrie.h"

namespace ContainersPP {
	PackKey::PackKey(const uint8_t* key, uint8_t size) : KeyLen(size)
	{
		Oryol::Memory::Copy(key, Key(), (int)KeyLen);
	}
	ContainersPP::PackKeyEnum::ValTypes ContainersPP::PackKey::ValueType() const
	{
		Types::BitRef<uint8_t, 2,const uint8_t*> vt = { &flags,0 };
		return (PackKeyEnum::ValTypes)(uint8_t)vt;
	}

	ContainersPP::PackKeyEnum::ValTypes ContainersPP::PackKey::PostType() const
	{
		Types::BitRef<uint8_t, 2, const uint8_t*> pt = { &flags,2 };
		return (PackKeyEnum::ValTypes)(uint8_t)pt;
	}

	uint8_t PackKey::ValueSize() const
	{
		switch (ValueType())
		{
		case ContainersPP::PackKeyEnum::none:
			return 0;
		case ContainersPP::PackKeyEnum::tiny:
			return sizeof(uint8_t);
		case ContainersPP::PackKeyEnum::small:
			return sizeof(uint16_t);
		case ContainersPP::PackKeyEnum::full:
			return sizeof(uint32_t);
		default:
			break;
		};
		return 0;
	}

	uint8_t PackKey::PostSize() const
	{
		switch (PostType())
		{
		case ContainersPP::PackKeyEnum::none:
			return 0;
		case ContainersPP::PackKeyEnum::tiny:
			return sizeof(uint8_t);
		case ContainersPP::PackKeyEnum::small:
			return sizeof(uint16_t);
		case ContainersPP::PackKeyEnum::full:
			return sizeof(uint32_t);
		default:
			break;
		};
		return 0;
	}

	uint32_t PackKey::PostID() const
	{
		switch (PostType())
		{
		case ContainersPP::PackKeyEnum::none:
			return 0;
		case ContainersPP::PackKeyEnum::tiny:
			return *(uint8_t*)(Key() + KeyLen + ValueSize());
		case ContainersPP::PackKeyEnum::small:
			return *(uint16_t*)(Key() + KeyLen + ValueSize());
		case ContainersPP::PackKeyEnum::full:
			return *(uint32_t*)(Key() + KeyLen + ValueSize());
		default:
			break;
		};
		return std::numeric_limits<uint32_t>::max();
	}

	void PackKeyAdapter::Insert(const uint8_t* key, uint8_t size)
	{
		new (block.AddInsert(startOffset, sizeof(PackKey) + size)) PackKey(key, size);
	}

	void PackKeyAdapter::Truncate(uint8_t Tare, uint32_t PostID, bool clearValue)
	{
		Key().KeyLen -= Tare;
		block.Remove(startOffset + sizeof(PackKey), Tare);
		SetPostID(PostID);
		if (clearValue)
			ClearValue();
	}

	void PackKeyAdapter::SetPostID(uint32_t ID)
	{
		if (ID = 0) {
			ClearPostID();
			return;
		};

		uint8_t min = Types::GetMinSize(ID);
		if (min < Key().PostSize())
			block.Remove(Key().PostPtr() - block.Data(), Key().PostSize() - min);
		else if (min > Key().PostSize())
			block.AddInsert(Key().PostPtr() - block.Data(), min - Key().PostSize());
			
		Types::BitRef<uint8_t, 2, uint8_t*> pt = { &Key().flags,2 };
		switch (min)
		{
		case 1:
			pt = ContainersPP::PackKeyEnum::tiny;
			*Key().PostPtr() = (uint8_t)ID;
		case 2:
			pt = ContainersPP::PackKeyEnum::small;
			*(uint16_t*)Key().PostPtr() = (uint16_t)ID;
		case 4:
			pt = ContainersPP::PackKeyEnum::full;
			*(uint32_t*)Key().PostPtr() = (uint32_t)ID;
		default:
			break;
		};
	}

	bool PackKeyAdapter::ClearPostID()
	{
		if (!Key().PostSize()) {
			return false;
		};

		block.Remove(Key().PostPtr() - block.Data(), Key().PostSize());

		Types::BitRef<uint8_t, 2, uint8_t*> pt = { &Key().flags,2 };
		pt = ContainersPP::PackKeyEnum::none;			

		return true;
	}

	uint32_t PackKey::Value() const
	{
		switch (ValueType())
		{		
		case ContainersPP::PackKeyEnum::tiny:
			return *(uint8_t*)(Key() + KeyLen);
		case ContainersPP::PackKeyEnum::small:
			return *(uint16_t*)(Key() + KeyLen);
		case ContainersPP::PackKeyEnum::full:
			return *(uint32_t*)(Key() + KeyLen);
		default:
			break;
		};
		return std::numeric_limits<uint32_t>::max();
	}

	void PackKeyAdapter::SetValue(uint32_t val)
	{
		if (val = std::numeric_limits<uint32_t>::max()) {
			ClearValue();
			return;
		};

		uint8_t min = Types::GetMinSize(val);
		if (min < Key().ValueSize())
			block.Remove(Key().ValuePtr() - block.Data(), Key().ValueSize() - min);
		else if (min > Key().ValueSize())
			block.AddInsert(Key().ValuePtr() - block.Data(), min - Key().ValueSize());

		Types::BitRef<uint8_t, 2, uint8_t*> vt = { &Key().flags,0 };
		switch (min)
		{
		case 1:
			vt = ContainersPP::PackKeyEnum::tiny;
			*Key().ValuePtr() = (uint8_t)val;
		case 2:
			vt = ContainersPP::PackKeyEnum::small;
			*(uint16_t*)Key().ValuePtr() = (uint16_t)val;
		case 4:
			vt = ContainersPP::PackKeyEnum::full;
			*(uint32_t*)Key().ValuePtr() = (uint32_t)val;
		default:
			break;
		};
	}

	bool PackKeyAdapter::ClearValue()
	{
		if (!Key().ValueSize()) {
			return false;
		};

		block.Remove(Key().ValuePtr() - block.Data(), Key().ValueSize());

		Types::BitRef<uint8_t, 2, uint8_t*> pt = { &Key().flags,0 };
		pt = ContainersPP::PackKeyEnum::none;

		return true;
	}

	const PackKey* iIndexTrieNode::Find(const uint8_t* key) const
	{
		PackKey* itr = (PackKey*)Block().Data(1);
		uint8_t count = Count();
		for (uint8_t i = 0; i < count; i++) 
			if(itr->Key()[0] >= key[0])
				return itr;				

		return nullptr;
	}

	PackKey* iIndexTrieNode::Find(const uint8_t* key)
	{
		PackKey* itr = (PackKey*)Block().Data(1);
		uint8_t count = Count();
		for (uint8_t i = 0; i < count; i++)
			if (itr->Key()[0] >= key[0])
				return itr;

		return nullptr;
	}

	void iIndexTrieNode::Erase(PackKey* key)
	{
		o_assert_dbg((uint64_t)((uint8_t*)key - Block().Data()) < Block().Size());
		Block().Remove((uint8_t*)key - Block().Data(), key->Size());
		--Count();
	}

	PackKeyAdapter iIndexTrieNode::InsertNewKey(const uint8_t* key, uint8_t Keylen, uint32_t val, uint32_t PostID)
	{
		PackKey* k = Find(key);
		o_assert_dbg(*key != *(k->Key()));
		PackKeyAdapter a(Block(), (uint32_t)((uint8_t*)k - Block().Data()));
		a.Insert(key, Keylen);
		a.SetPostID(PostID);
		a.SetValue(val);
		++Count();

		return a; // &(a.Key());
	}

	PackKeyAdapter iIndexTrieNode::InsertKey(const PackKey* CopyKey, uint8_t Tare)
	{
		uint32_t pid = CopyKey->PostID();
		uint32_t val = CopyKey->Value();

		return InsertNewKey(CopyKey->Key() + Tare, CopyKey->KeyLen - Tare, val, pid);		
	}

	PackKeyAdapter iIndexTrieNode::PushDown(PackKey* breakKey, uint8_t prefixLen, iIndexTrieNode& PushNode)
	{
		PushNode.InsertKey(breakKey, prefixLen);
		PackKeyAdapter k(Block(), (uint32_t)((uint8_t*)breakKey - Block().Data()));
		k.Truncate(prefixLen, PushNode.ID());

		return k;
	}

	uint64_t iIndexTrieNode::GetKeyOffset(uint64_t index)
	{
		o_assert_dbg(*(Block().Data()) >= index); //index is less than count
	
		return (uint8_t*)GetKey(index) - Block().Data();
	}

	PackKey* iIndexTrieNode::GetKey(uint64_t index)
	{
		o_assert_dbg(*(Block().Data()) >= index); //index is less than count

		PackKey* itr = (PackKey*)Block().Data(1);
		for (uint8_t i = 0; i < index; i++)
			itr = itr->Next();
		
		return itr;
	}


	const PackKey* iIndexTrie::Find(const uint8_t* key, uint8_t KeyLen) const
	{
		Node_Adapter node = Nodes(0);
		while (KeyLen) {
			PackKey* k = node.Find(key);
			Types::KeyCompare c(k->Key(), key, k->KeyLen, KeyLen);
			if (c.Equal())
				return k;
			if (c.lhsPostfix || !k->PostID()) //no match
				return nullptr;			

			node = Nodes(k->PostID());			
			key += c.commonPrefix;
			KeyLen -= (uint16_t)c.commonPrefix;
		}
		return nullptr;
	}

	PackKeyAdapter iIndexTrie::Get(const uint8_t* key, uint8_t KeyLen)
	{
		Node_Adapter node = Nodes(0);
		while (KeyLen) {
			PackKey* k = node.Find(key);
			Types::KeyCompare c(k->Key(), key, k->KeyLen, KeyLen);
			if (c.Equal()) //Match
				return PackKeyAdapter(node.Block(), (uint32_t)((uint8_t*)k - node.Block().Data()));
			if (!c.commonPrefix) {// no match insert in this node;
				return node.InsertNewKey(key, (uint8_t)KeyLen);
			}
			key += c.commonPrefix;
			KeyLen -= (uint8_t)c.commonPrefix;
			if (c.lhsPostfix) { //Partial match
				Node_Adapter na = NewNode();
				PackKeyAdapter ka = node.PushDown(k, (uint8_t)c.commonPrefix, na);
				if (c.rhsPostfix) {// goes in new node
					return na.InsertNewKey(key, KeyLen);
				}
				//goes in this key
				return ka;
			}
			//partial match keep searching
			node = Nodes(k->PostID());			
		}
		
		o_assert_dbg(false);
		//return PackKeyAdapter();
	}

	void iIndexTrie::Erase(const uint8_t* key, uint8_t KeyLen)
	{
		Node_Adapter node = Nodes(0);
		while (KeyLen) {
			PackKey* k = node.Find(key);
			Types::KeyCompare c(k->Key(), key, k->KeyLen, KeyLen);
			if (c.Equal()) { // erase this key
				node.Erase(k);
				return;
			}
			if (c.lhsPostfix || !k->PostID()) //no match
				return;

			node = Nodes(k->PostID());
			key += c.commonPrefix;
			KeyLen -= (uint8_t)c.commonPrefix;
		}
	}

	
}