#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Trie
    @ingroup Core
    @brief a compressed suffix Retrieval

    compressed suffix Retrieval that stores values that can be accessed by a variable length key of uint8
     
    @see Array, ArrayMap, Map
*/
//#include <algorithm>
#include "ContainersPP/Containers/Allocator.h"
#include "ContainersPP/Types/Types.h"
#include "ContainersPP/Types/BitPointer.h"
#include "ContainersPP/Types/KeyString.h"


namespace ContainersPP {
   
    namespace CatValEnums {
        enum CatValTypes : uint8_t {
            prm,
            stc,
            lst,
            sub
        };
    }
    struct CatVal {
        Types::baseTypes type;
        union {
            uint8_t uint_8;
            uint16_t uint_16;
            uint32_t uint_32;
            uint64_t uint_64 = 0;
            int8_t int_8;
            int16_t int_16;
            int32_t int_32;
            int64_t int_64;
            float float_32;
            double float_64;
            bool boolean_8;
        };
    };

    struct CatKey {
        CatKey(const Types::KeyString& key, uint64_t tare = 0);
        uint8_t flags = 0;
        uint64_t KeyLen = 0;
        CatValEnums::CatValTypes Type() const;
        void Type(CatValEnums::CatValTypes type);

        bool HasValue() const { return flags & 0x01;  };
        bool HasPosts()  const { return flags & 0x02; };
        uint64_t ValueSize() const;
        uint64_t* PostID() { return HasPosts() ? (uint64_t*)(Key() + KeyLen) : nullptr; };
        CatVal* Value() { return (CatVal*)(HasValue() ? Key() + KeyLen + (HasPosts() ? sizeof(uint64_t):0): nullptr); }
        const CatVal* Value() const { return (CatVal*)(HasValue() ? Key() + KeyLen + (HasPosts() ? sizeof(uint64_t) : 0) : nullptr); }
        uint8_t* Key() { return (uint8_t*)(this + 1); }
        const uint8_t* Key() const { return (uint8_t*)(this + 1); }
        CatKey* Next() { return (CatKey*)(Key() + KeyLen + (HasValue() ? ValueSize() : 0) + (HasPosts() ? sizeof(uint64_t) : 0)); }
        const CatKey* Next() const { return (CatKey*)(Key() + KeyLen + (HasValue() ? ValueSize() : 0) + (HasPosts() ? sizeof(uint64_t) : 0)); }
        uint64_t Size() const { return (uint8_t*)Next() - (uint8_t*)this; };
    };

    CatValEnums::CatValTypes ContainersPP::CatKey::Type() const
    {
        const Types::BitRef<uint8_t, 2, const uint8_t* > tpr = { &flags,2 };
        
        return (CatValEnums::CatValTypes)tpr.Value();
    }

    inline void CatKey::Type(CatValEnums::CatValTypes type)
    {
        Types::BitRef<uint8_t, 2> tpr = { &flags,2 };
        tpr = type;
    }

    inline uint64_t CatKey::ValueSize() const
    {
        if (!HasValue())
            return 0;

        if (Type() == CatValEnums::CatValTypes::stc)
            return sizeof(CatVal) + Value()->uint_64;

        return sizeof(CatVal);
    }


    inline ContainersPP::CatKey::CatKey(const Types::KeyString& key, uint64_t tare)
    {
        KeyLen = key.Size() - tare;
        Oryol::Memory::Copy(key.Data(tare), Key(), (int)KeyLen);
    }
    

    class iCatTrie 
    {
    public:
        /// get number of elements in array
        uint64_t Size() const { return 0; };
        /// return true if empty
        bool Empty() const { return !Size(); };
        /// clear the array (deletes elements, keeps capacity)
        void Clear() {};
        /// test if an element exists
        bool Contains(const Types::KeyString& key) const;
        bool Contains(const char* key) const { return Contains(Types::MakeKey(key)); };
        bool Contains(uint64_t key) const { return Contains(Types::MakeKey(key)); };
        /// find element
        virtual const CatKey* Find(const Types::KeyString& key) const;
        const CatKey* Find(const char* key) const { return Find(Types::MakeKey(key)); };
        const CatKey* Find(uint64_t key) const { return Find(Types::MakeKey(key)); };

        virtual CatKey* Find(const Types::KeyString& key) { return const_cast<CatKey*>(((const iCatTrie*)this)->Find(key)); }
        CatKey* Find(const char* key) { return Find(Types::MakeKey(key)); };
        CatKey* Find(uint64_t key) { return Find(Types::MakeKey(key)); };
        
        /// add element
        virtual CatKey* Add(const Types::KeyString& key, CatValEnums::CatValTypes type, CatVal val);
        CatKey* Add(const char* key, CatValEnums::CatValTypes type, CatVal val) { return Add(Types::MakeKey(key), type, val); };
        CatKey* Add(uint64_t key, CatValEnums::CatValTypes type, CatVal val) { return Add(Types::MakeKey(key), type, val); };
        /// erase element
        virtual void Erase(const Types::KeyString& key); //TODO::
        void Erase(const char* key) { return Erase(Types::MakeKey(key)); };
        void Erase(uint64_t key) { return Erase(Types::MakeKey(key)); };
    
    protected:
        virtual Allocator& Nodes() = 0;
        virtual const Allocator& Nodes() const = 0;
        static void InitializeNode(iBufferV& nodeBuffer);
    private:
        
        //TYPE& InsertNewKey(uint64_t nodeID, uint8_t index, const Types::KeyString& key, uint64_t tare);
        CatKey* InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, CatValEnums::CatValTypes type, CatVal val, uint64_t PostID = 0);
        void InsertEmptyKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, uint64_t PostID = 0);
        uint64_t PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen);
        static uint64_t GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index);       
        

    };

   
    inline bool iCatTrie::Contains(const Types::KeyString& key) const
    {
        return Find(key) != nullptr;
    };

    
    inline const CatKey* iCatTrie::Find(const Types::KeyString& key) const
    {
        const iBufferV* node = &Nodes()[0];
        uint8_t count = *node->Data();
        CatKey* nk = (CatKey*)node->Data(1);
        const uint8_t* keyItr = key.Data();
        uint64_t tare = 0;
        //for each key
        for (int16_t i = 0; i < count; i++) {
            const uint8_t* checkItr = nk->Key();

            if (*checkItr < *keyItr) {
                nk = nk->Next(); //get next nodekey
                /*if ((uint8_t*)nk - node->Data() > node->Size())
                    return nullptr;*/
                continue;
            }
            if (*checkItr == *keyItr) {//at least partial match
                //prefetch next node (Cache Hack)
                uint64_t* postIDptr = nk->PostID();
                if (postIDptr) {
                    //TODO:: this can be done in spawned thread
                    node = &Nodes()[*postIDptr];
                    count = *node->Data();
                }

                //compare the keys
                Types::KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size() - tare);

                if (c.Equal()) // found
                    return nk;
                if (!c.lhsPostfix) {//full match ck check posts
                    if (!node)//no posts exists
                        return nullptr;
                    keyItr += c.commonPrefix;
                    tare += c.commonPrefix;
                    nk = (CatKey*)node->Data(1);
                    i = -1;
                    continue; //restart loop on new Node;
                }
                else //not a match
                    return nullptr;
            }
            return nullptr;
        }
        return nullptr;
    };


inline CatKey* iCatTrie::Add(const Types::KeyString& key, CatValEnums::CatValTypes type, CatVal val)
{
    uint64_t NodeID = 0;
    iBufferV* node = &Nodes()[NodeID];
    uint8_t count = *node->Data();
    CatKey* nk = (CatKey*)node->Data(1);
    const uint8_t* keyItr = key.Data();

    //for each key
    for (uint8_t i = 0; i < count; i++) {
        const uint8_t* checkItr = nk->Key();

        if (*checkItr < *keyItr) {
            nk = nk->Next(); //get next nodekey
            continue;
        }
        if (*checkItr == *keyItr) {//at least partial match
            //prefetch next node (Cache Hack)
            uint64_t nextNodeID;
            iBufferV* nextNode = nullptr;
            uint64_t* postIDptr = nk->PostID();
            if (postIDptr) {
                //TODO:: this can be done in spawned thread
                nextNodeID = *postIDptr;
                nextNode = &Nodes()[nextNodeID];
                count = *(nextNode->Data());
            }

            //compare the keys
            Types::KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size());

            if (c.Equal()) { // found
                if (!nk->HasValue()) {
                    uint64_t offset = (uint8_t*)nk - node->Data();
                    nk->flags |= 0x01;
                    nk->Type(type);
                    uint64_t size = ((type == CatValEnums::stc) ? val.uint_64 : 0) + sizeof(CatVal);
                    *(CatVal*)node->AddInsert((uint8_t*)nk->Value()-node->Data(), size) = val;
                    nk = (CatKey*)node->Data(offset);
                }                    
                return nk;
            }  
            keyItr += c.commonPrefix;
            if (!c.lhsPostfix) {//full match ck check posts
                
                if (!nextNode) {//no posts, create them
                    uint64_t newNodeID = Nodes().Count();
                    nk->flags |= 0x02;
                    uint64_t offset = (uint8_t*)nk - node->Data();
                    node->AddInsert(((uint8_t*)nk->PostID() - node->Data()), sizeof(uint64_t));
                    nk = (CatKey*)node->Data(offset);
                    nk->PostID()[0] = newNodeID;
                    uint64_t size = ((type == CatValEnums::stc) ? val.uint_64 : 0) + sizeof(CatVal);
                    Nodes().New(1 + sizeof(CatKey) + key.Size()+ size - (keyItr - key.Data())); //might invalidates nk pointer
                    InitializeNode(Nodes()[newNodeID]);
                    return InsertKey(Nodes()[newNodeID], 0, key, keyItr - key.Data(), type, val);
                }
                NodeID = nextNodeID;
                node = nextNode;
                nk = (CatKey*)node->Data(1);
                i = -1;
                continue; //restart loop on new Node;
            }
            if (c.lhsPostfix) { // partial match
                //push down
                uint64_t offset = (uint8_t*)nk - node->Data();  
                uint64_t pushNodeID = PushDown(NodeID, i, c.commonPrefix);
                node = &Nodes()[NodeID];
                nk = (CatKey*)node->Data(offset);

                if (c.rhsPostfix) {//insert key in new node
                    if (nk->HasValue()) {//remove the value
                        node->Remove((uint8_t*)nk->Value() - node->Data(), nk->ValueSize());
                        nk->flags &= 0x02;
                    }
                    return InsertKey(Nodes()[pushNodeID], c.lhsLesser ? 1 : 0, key, keyItr - key.Data(),type,val);
                }
                else { //insert key in this nodekey
                    if (!nk->HasValue()) {
                        uint64_t offset = (uint8_t*)nk - node->Data();
                        nk->flags |= 0x01;
                        nk->Type(type);
                        uint64_t size = ((type == CatValEnums::stc) ? val.uint_64 : 0) + sizeof(CatVal);
                        *(CatVal*)node->AddInsert((uint8_t*)nk->Value() - node->Data(), size) = val;
                        nk = (CatKey*)node->Data(offset);
                        return nk;
                    }                       
                    //value already exists overrite it                    
                    if (type == CatValEnums::stc) {
                        if (nk->Type() != CatValEnums::stc) {
                            //add room for static array
                            nk->Type(type);
                            *nk->Value() = val;
                            node->AddInsert(((uint8_t*)nk->Value() + sizeof(CatVal)) - node->Data(), nk->ValueSize() - sizeof(CatVal));
                            //nk might have been invalidated
                            nk = (CatKey*)node->Data(offset);
                        }
                        else if (nk->ValueSize() < val.int_64 * Types::SizeOf(val.type)) {
                            //must add more space
                            uint64_t size = (val.int_64 * Types::SizeOf(val.type)) - nk->ValueSize();
                            uint64_t endoffset = (uint8_t*)nk->Next() - node->Data();
                            *nk->Value() = val;
                            node->AddInsert(endoffset, size);
                            //nk might have been invalidated
                            nk = (CatKey*)node->Data(offset);
                        }
                        else if (nk->ValueSize() > val.int_64 * Types::SizeOf(val.type)) {
                            //must remove space
                            uint64_t size = nk->ValueSize() - (val.int_64 * Types::SizeOf(val.type));
                            *nk->Value() = val;
                            uint64_t endoffset = (uint8_t*)nk->Next() - node->Data();                            
                            node->Remove(endoffset, size);
                            //nk might have been invalidated
                            nk = (CatKey*)node->Data(offset);
                        }        
                        else {//same size update value
                            *nk->Value() = val;
                        }
                    }
                    else {//update value
                        nk->Type(type);
                        *nk->Value() = val;
                    }
                    
                    return nk;
                }
            }
        }//key is greater insert here
        return InsertKey(*node, i, key, keyItr - key.Data(),type,val);
    } // no matches insert at end
    return InsertKey(*node, count, key, keyItr - key.Data(), type, val);
}

inline void iCatTrie::Erase(const Types::KeyString& key)
{
    iBufferV* node = &Nodes()[0];
    uint8_t count = *node->Data();
    CatKey* nk = (CatKey*)node->Data(1);
    const uint8_t* keyItr = key.Data();
    uint64_t tare = 0;
    //for each key
    for (int16_t i = 0; i < count; i++) {
        const uint8_t* checkItr = nk->Key();

        if (*checkItr < *keyItr) {
            nk = nk->Next(); //get next nodekey
            continue;
        }
        if (*checkItr == *keyItr) {//at least partial match
            
            //compare the keys
            //prefetch next node (Cache Hack)
            uint64_t* postIDptr = nk->PostID();

            Types::KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size() - tare);
            if (c.Equal()) { // found
                if (postIDptr) {
                    //has posts only delete Value;
                    if (nk->HasValue()) {//remove the value
                        node->Remove((uint8_t*)nk->Value() - node->Data(), nk->ValueSize());
                        nk->flags &= 0x02;
                    }
                    return;
                }
                //DELETE NK
                node->Remove((uint8_t*)nk - node->Data(), (uint8_t*)(nk->Next()) - (uint8_t*)nk);
                node->Data()[0]--;
                //TODO:: if node only has 1 key then recombine with parent.
                return;
            }
                                   
            
            if (postIDptr) {
                //TODO:: this can be done in spawned thread
                node = &Nodes()[*postIDptr];
                count = *node->Data();
            }

           
            if (!c.lhsPostfix) {//full match ck check posts
                if (!node)//no posts exists
                    return;
                keyItr += c.commonPrefix;
                tare += c.commonPrefix;
                nk = (CatKey*)node->Data(1);
                i = -1;
                continue; //restart loop on new Node;
            }
            else //not a match
                return;
        }
        return;
    }
    return;
}
;



inline void iCatTrie::InitializeNode(iBufferV& nodeBuffer)
{
    nodeBuffer.Clear();
    nodeBuffer.AddBack(1);
    *nodeBuffer.Data() = 0;
};



//inline TYPE& iCatTrie::InsertNewKey(uint64_t nodeID, uint8_t index, const Types::KeyString& key, uint64_t tare)
//{
//    return InsertKey(Nodes()[nodeID], index, key, tare, TYPE());
//};



inline CatKey* iCatTrie::InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, CatValEnums::CatValTypes type, CatVal val, uint64_t PostID)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    //find index of insert
    uint64_t size = ((type == CatValEnums::stc) ? val.uint_64 : 0) + sizeof(CatVal);
    uint8_t* newBuffer = nodeBuffer.AddInsert(GetKeyOffset(nodeBuffer, index), sizeof(CatKey) + key.Size() - tare + size + (PostID ? sizeof(uint64_t) : 0));
    new (newBuffer) CatKey(key, tare);
    ++* (nodeBuffer.Data()); //increment count

    CatKey* TK = (CatKey*)newBuffer;
    
    TK->flags |= 0x01;
    TK->Type(type);

    if (PostID) {
        TK->flags |= 0x02;
        TK->PostID()[0] = PostID;
    }   

    *(CatVal*)TK->Value() = val;

    return TK;
}

inline void iCatTrie::InsertEmptyKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, uint64_t PostID)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
//find index of insert
    uint8_t* newBuffer = nodeBuffer.AddInsert(GetKeyOffset(nodeBuffer, index), sizeof(CatKey) + key.Size() - tare + (PostID ? sizeof(uint64_t) : 0));
    new (newBuffer) CatKey(key, tare);
    ++* (nodeBuffer.Data()); //increment count

    CatKey* TK = (CatKey*)newBuffer;

    if (PostID) {
        TK->flags |= 0x02;
        TK->PostID()[0] = PostID;
    }
}
;


inline uint64_t iCatTrie::PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen) {
    uint64_t offset = GetKeyOffset(Nodes()[nodeID], index);
    CatKey* nk = (CatKey*)Nodes()[nodeID].Data(offset);
    offset += prefixLen + sizeof(CatKey);

    //copy the key
    uint64_t PostFixLen = nk->KeyLen - prefixLen;
    
    Types::KeyString key;
    key.CopyBack(nk->Key() + prefixLen, PostFixLen);
    
    uint64_t PostID = nk->HasPosts() ? nk->PostID()[0] : 0;

    //create new node
    if (!PostID) {
        uint64_t nOffset = (uint8_t*)nk - Nodes()[nodeID].Data();
        nk->flags |= 0x02;
        Nodes()[nodeID].AddInsert((uint8_t*)nk->PostID() - Nodes()[nodeID].Data(), sizeof(uint64_t));
        nk = (CatKey*)Nodes()[nodeID].Data(nOffset);
    }
    CatVal* Value = nk->Value();
    nk->PostID()[0] = Nodes().Count();
    uint64_t newNodeID = nk->PostID()[0];
    uint64_t size = (Value) ? ((nk->Type() == CatValEnums::stc) ? Value->uint_64 : 0) + sizeof(CatVal) : 0;
    iBufferV& newNode = Nodes()[Nodes().New(1 + PostFixLen + sizeof(CatKey) + size + (PostID ? sizeof(uint64_t) : 0))]; //might invalidates nk pointer
    InitializeNode(newNode);
    
    //insert key
    if (Value) {
        InsertKey(newNode, 0, key, 0, nk->Type(), *Value, PostID);
    }
    else
        InsertEmptyKey(newNode, 0, key, 0, PostID);

    //erase the postfix 
    nk->KeyLen = prefixLen;
    Nodes()[nodeID].Remove(offset, PostFixLen); //Might invalidate NK pointer

    return newNodeID;
};


inline uint64_t iCatTrie::GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    //uint64_t offset = 1;
    CatKey* itr = (CatKey*)nodeBuffer.Data(1);
    for (uint8_t i = 0; i < index; i++) {
        itr = itr->Next();
    }
    return (uint8_t*)itr - nodeBuffer.Data();
};



} // namespace Oryol
