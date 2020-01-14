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
#include "ContainersPP/Types/KeyString.h"


namespace ContainersPP {
    struct NodeKey {
        NodeKey( const Types::KeyString& key, uint64_t tare = 0 , uint64_t valID = 0, uint64_t postID = 0);
        uint64_t PostID = 0;
        uint64_t ValID = 0;
        uint64_t KeyLen = 0;
        uint8_t* Key() { return (uint8_t*)(this + 1); }
        NodeKey* Next() { return (NodeKey*)(Key() + KeyLen); }
    };

    ContainersPP::NodeKey::NodeKey(const Types::KeyString& key, uint64_t tare, uint64_t valID, uint64_t postID) : ValID(valID), PostID(postID)
    {
        KeyLen = key.Size() - tare;
        Oryol::Memory::Copy(key.Data(tare), Key(), (int)KeyLen);
    }

    template<typename TYPE>
    struct TypeKey {
        TypeKey(const Types::KeyString& key, uint64_t tare = 0);
        uint8_t flags = 0;
        uint64_t KeyLen = 0;

        bool HasValue() { return flags & 0x01;  };
        bool HasPosts() { return flags & 0x02; };
        
        uint64_t* PostID() { return HasPosts() ? (*uint64_t)(Key() + KeyLen + (HasValue() ? sizeof(TYPE) : 0)) : nullptr; };
        TYPE* Value() { return (*TYPE)(HasValue() ? Key() + KeyLen : nullptr); }
        uint8_t* Key() { return (uint8_t*)(this + 1); }
        NodeKey* Next() { return (NodeKey*)(Key() + KeyLen + (HasValue() ? sizeof(TYPE) : 0) + (HasPosts() ? sizeof(uint64_t) : 0); }
    };
    template<typename TYPE>
    inline ContainersPP::TypeKey<TYPE>::TypeKey(const Types::KeyString& key, uint64_t tare) 
    {
        KeyLen = key.Size() - tare;
        Oryol::Memory::Copy(key.Data(tare), Key(), (int)KeyLen);
    }
    

    template<typename TYPE>
    class iTrie 
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
        virtual const TYPE* Find(const Types::KeyString& key) const;
        const TYPE* Find(const char* key) const { return Find(Types::MakeKey(key)); };
        const TYPE* Find(uint64_t key) const { return Find(Types::MakeKey(key)); };
        /// add element
        virtual TYPE& GetOrAdd(const Types::KeyString& key);
        TYPE& GetOrAdd(const char* key) { return GetOrAdd(Types::MakeKey(key)); };
        TYPE& GetOrAdd(uint64_t key) { return GetOrAdd(Types::MakeKey(key)); };
        /// erase element
        virtual void Erase(const Types::KeyString& key) {}; //TODO::
        void Erase(const char* key) { return Erase(Types::MakeKey(key)); };
        void Erase(uint64_t key) { return Erase(Types::MakeKey(key)); };
    
    protected:
        virtual Allocator& allocator();
        virtual const Allocator& allocator() const;
    private:
        static void InitializeNode(iBufferV& nodeBuffer);
        TYPE& InsertNewKey(uint64_t nodeID, uint8_t index, const Types::KeyString& key, uint64_t tare);
        TYPE& InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, TYPE&& Value, uint64_t PostID = 0);
        void InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, uint64_t PostID = 0);
        uint64_t PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen);
        static uint64_t GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index);
        //static TypeVector<uint8_t> MakeKey(const char* key);   

    };

//inline Trie::Trie()
//{
//    o_assert_dbg(allocator.Count() == 0);
//    InitializeNode(allocator[allocator.New(32)]);
//    o_assert_dbg(allocator.Count() == 1);
//}
    template<typename TYPE>
    inline bool iTrie<TYPE>::Contains(const Types::KeyString& key) const
    {
        return Find(key) != nullptr;
    }
    template<typename TYPE>
inline const TYPE* iTrie<TYPE>::Find(const Types::KeyString& key) const
{
    const iBufferV* node = &allocator[0];
    uint8_t count = *node->Data();
    TypeKey<TYPE>* nk = (NodeKey*)node->Data(1);
    const uint8_t* keyItr = key.Data();
    uint64_t tare = 0;
    //for each key
    for (int16_t i = 0; i < count; i++) {
        const uint8_t* checkItr = nk->Key();        

        if (*checkItr< *keyItr) {
            nk = nk->Next(); //get next nodekey
            continue; 
        }
        if (*checkItr == *keyItr) {//at least partial match
            //prefetch next node (Cache Hack)
            uint64_t* postIDptr = nk->PostID();
            if (postIDptr) {
                //TODO:: this can be done in spawned thread
                node = &allocator[*postIDptr];
                count = *node->Data();
            }            
           
            //compare the keys
            Types::KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size()-tare);
            
            if (c.Equal()) // found
                return nk->Value();
            if (!c.lhsPostfix) {//full match ck check posts
                if (!node)//no posts exists
                    return nullptr;
                keyItr += c.commonPrefix;
                tare += c.commonPrefix;
                nk = (NodeKey*)node->Data(1);
                i = -1;
                continue; //restart loop on new Node;
            }
            else //not a match
                return nullptr;
        }
        return nullptr;       
    }
    return nullptr;
}
template<typename TYPE>
inline TYPE& iTrie<TYPE>::GetOrAdd(const Types::KeyString& key)
{
    uint64_t NodeID = 0;
    iBufferV* node = &allocator[NodeID];
    uint8_t count = *node->Data();
    TypeKey<TYPE>* nk = (NodeKey*)node->Data(1);
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
                nextNode = &allocator[nextNodeID];
                count = *(nextNode->Data());
            }

            //compare the keys
            Types::KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size());

            if (c.Equal()) { // found
                if (!nk->HasValue()) {
                    uint64_t offset = nk - node->Data();
                    nk->flags |= 0x01;
                    new (node->AddInsert(nk->Value()-node->Data(), sizeof(TYPE))) TYPE();
                    nk = node->Data(offset);
                }                    
                return *nk->Value();
            }  
            keyItr += c.commonPrefix;
            if (!c.lhsPostfix) {//full match ck check posts
                
                if (!nextNode) {//no posts, create them
                    uint64_t newNodeID = allocator.Count();
                    nk->flags |= 0x02;
                    uint64_t offset = nk - node->Data();
                    node->AddInsert(nk->PostID() - node->Data(), sizeof(uint64_t));
                    nk = node->Data(offset);
                    nk->PostID()[0] = newNodeID;
                    allocator.New(1 + sizeof(TypeKey<TYPE>) + key.Size()+sizeof(TYPE) - (keyItr - key.Data())); //might invalidates nk pointer
                    InitializeNode(allocator[newNodeID]);
                    return InsertNewKey(newNodeID, 0, key, keyItr - key.Data());
                }
                NodeID = nextNodeID;
                node = nextNode;
                nk = (NodeKey*)node->Data(1);
                i = -1;
                continue; //restart loop on new Node;
            }
            if (c.lhsPostfix) { // partial match
                //push down
                uint64_t pushNodeID = PushDown(NodeID, i, c.commonPrefix);

                if (c.rhsPostfix) {//insert key in new node
                    if (nk->HasValue()) {//remove the value
                        node->Remove(nk->Value() - node->Data(), sizeof(TYPE));
                        nk->flags &= 0x02;
                    }
                    return InsertNewKey(pushNodeID, c.lhsLesser ? 1 : 0, key, keyItr - key.Data());
                }
                else { //insert key in in this nodekey
                    if (!nk->HasValue()) {
                        uint64_t offset = nk - node->Data();
                        nk->flags |= 0x01;
                        new (node->AddInsert(nk->Value() - node->Data(), sizeof(TYPE))) TYPE();
                        nk = node->Data(offset);
                    }
                    else
                        new (nk - Value()) TYPE();
                    
                    return *nk->Value();
                }
            }
        }//key is greater insert here
        return InsertNewKey(NodeID, i, key, keyItr - key.Data());
    } // no matches insert at end
    return InsertNewKey(NodeID, count, key, keyItr - key.Data());
}
template<typename TYPE>
inline void iTrie<TYPE>::InitializeNode(iBufferV& nodeBuffer)
{
    nodeBuffer.Clear();
    nodeBuffer.AddBack(1);
    *nodeBuffer.Data() = 0;
}
template<typename TYPE>
inline TYPE& iTrie<TYPE>::InsertNewKey(uint64_t nodeID, uint8_t index, const Types::KeyString& key, uint64_t tare)
{
    return InsertKey(allocator[nodeID], index, key, tare, TYPE());
}
template<typename TYPE>
inline TYPE& iTrie<TYPE>::InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, TYPE&& Value, uint64_t PostID)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    //find index of insert
    uint8_t* newBuffer = nodeBuffer.AddInsert(GetKeyOffset(nodeBuffer, index), sizeof(TypeKey<TYPE>) + key.Size() - tare + sizeof(TYPE) + (PostID ? sizeof(uint64_t):0)) );
    new (newBuffer) TypeKey<TYPE>(key, tare);
    ++* (nodeBuffer.Data()); //increment count

    TypeKey<TYPE>* TK = nodeBuffer;
    TK->flags |= 0x01;
    new (TK->Value()) TYPE(std::move(Value));

    if (PostID) {
        TK->flags |= 0x02;
        TK->PostID()[0] = PostID
    }
   
    return TK->Value()[0];
}

template<typename TYPE>
inline uint64_t iTrie<TYPE>::PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen)
{
    uint64_t offset = GetKeyOffset(allocator[nodeID], index);
    TypeKey<TYPE>* nk = (TypeKey<TYPE>*)allocator[nodeID].Data(offset);
    offset += prefixLen + sizeof(TypeKey<TYPE>);

    //copy the key
    uint64_t PostFixLen = nk->KeyLen - prefixLen;
    nk->KeyLen = prefixLen;
    Types::KeyString key;
    key.CopyBack(nk->Key() + prefixLen, PostFixLen);
    TYPE* Value = nk->Value();
    uint64_t PostID = nk->HasPosts() ? nk->PostID()[0] : 0;

    //create new node
    if (!PostID) {
        uint64_t nOffset = nk - allocator[nodeID].Data();
        nk->flags |= 0x02;
        allocator[nodeID].AddInsert(nk->PostID() - allocator[nodeID].Data(), sizeof(uint64_t));
        nk = allocator[nodeID].Data(nOffset);
    }
    nk->PostID()[0] = allocator.Count();
    uint64_t newNodeID = nk->PostID()[0];
    iBufferV& newNode = allocator[allocator.New(1 + PostFixLen + sizeof(TypeKey<TYPE>) + (Value?sizeof(TYPE):0)+(PostID?sizeof(uint64_t):0) )]; //might invalidates nk pointer
    InitializeNode(newNode);

    //insert key
    if (Value) {
        InsertKey(newNode, 0, key, 0, std::move(*Value), PostID);
    }
    else
        InsertKey(newNode, 0, key, 0, PostID);
    
    //erase the postfix   
    allocator[nodeID].Remove(offset, PostFixLen); //Might invalidate NK pointer

    return newNodeID;
}
template<typename TYPE>
inline uint64_t iTrie<TYPE>::GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    //uint64_t offset = 1;
    TypeKey<TYPE>* itr = (NodeKey*)nodeBuffer.Data(1);
    for (uint8_t i = 0; i < index; i++) {
        itr = itr->Next();
    }
    return itr - nodeBuffer.Data();
}

//inline TypeVector<uint8_t> Trie::MakeKey(const char* key)
//{
//    TypeVector<uint8_t> newKey;
//    newKey.CopyBack((const uint8_t*)key, std::strlen(key));
//    return newKey;
//}

} // namespace Oryol
