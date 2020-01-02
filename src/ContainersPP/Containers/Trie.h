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



namespace ContainersPP {
    struct NodeKey {
        NodeKey( const TypeVector<uint8_t>& key, uint64_t tare = 0 , uint64_t valID = 0, uint64_t postID = 0);
        uint64_t PostID = 0;
        uint64_t ValID = 0;
        uint64_t KeyLen = 0;
        uint8_t* Key() { return (uint8_t*)(this + 1); }
        NodeKey* Next() { return (NodeKey*)(Key() + KeyLen); }
    };

    ContainersPP::NodeKey::NodeKey(const TypeVector<uint8_t>& key, uint64_t tare, uint64_t valID, uint64_t postID) : ValID(valID), PostID(postID)
    {
        KeyLen = key.Size() - tare;
        Oryol::Memory::Copy(key.Data(tare), Key(), (int)KeyLen);
    }

    struct KeyCompare {
        KeyCompare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize);
        size_t commonPrefix = 0;
        size_t lhsPostfix = 0;
        size_t rhsPostfix = 0;
        bool lhsLesser = false;
        bool Equal() { return !(lhsPostfix || rhsPostfix); }

    };
    KeyCompare::KeyCompare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize)
    {
        size_t compSize = (lhsSize < rhsSize) ? lhsSize : rhsSize;

        while (*lhs == *rhs && commonPrefix < compSize)
            commonPrefix++, lhs++, rhs++;

        lhsPostfix = lhsSize - commonPrefix;
        rhsPostfix = rhsSize - commonPrefix;
        lhsLesser = *(lhs) < *(rhs);
    }



//template<class VALUE> 
class Trie 
{
public:
    /// default constructor
    Trie();
    /// copy constructor (truncates to actual size)
    //Trie(const Trie& rhs);
    /// move constructor (same capacity and size)
    //Trie(Trie&& rhs)
    /// copy-assignment operator (truncates to actual size)
    //void operator=(const Trie& rhs);
    /// move-assignment operator (same capacity and size)
    //void operator=(Trie&& rhs);
    
    /// get number of elements in array
    uint64_t Size() const { return 0; };
    /// return true if empty
    bool Empty() const { return !Size(); };
    /// clear the array (deletes elements, keeps capacity)
    void Clear() {};
    /// test if an element exists
    bool Contains(const TypeVector<uint8_t>& key) const;
    bool Contains(const char* key) const { return Contains(MakeKey(key)); };
    /// find element
    const iBufferV* Find(const TypeVector<uint8_t>& key) const;
    const iBufferV* Find(const char* key) const { return Find(MakeKey(key)); };
    /// add element
    iBufferV& GetOrAdd(const TypeVector<uint8_t>& key);
    iBufferV& GetOrAdd(const char* key) { return GetOrAdd(MakeKey(key)); };
    /// erase element
    void Erase(const TypeVector<uint8_t>& key) {}; //TODO::
    void Erase(const char* key) { return Erase(MakeKey(key)); };
    
private:
    static void InitializeNode(iBufferV& nodeBuffer);
    iBufferV& InsertNewKey(uint64_t nodeID, uint8_t index, const TypeVector<uint8_t>& key, uint64_t tare);
    iBufferV& InsertKey(iBufferV& nodeBuffer, uint8_t index, const TypeVector<uint8_t>& key, uint64_t tare = 0,uint64_t ValID = 0, uint64_t PostID = 0);
    uint64_t PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen);
    static uint64_t GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index);
    static TypeVector<uint8_t> MakeKey(const char* key);

    Allocator allocator;

};

inline Trie::Trie()
{
    o_assert_dbg(allocator.Count() == 0);
    InitializeNode(allocator[allocator.New(32)]);
    o_assert_dbg(allocator.Count() == 1);
}

inline bool Trie::Contains(const TypeVector<uint8_t>& key) const
{
    return Find(key) != nullptr;
}

inline const iBufferV* Trie::Find(const TypeVector<uint8_t>& key) const
{
    const iBufferV* node = &allocator[0];
    uint8_t count = *node->Data();
    NodeKey* nk = (NodeKey*)node->Data(1);
    const uint8_t* keyItr = key.begin();
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
            if (nk->PostID) {
                //TODO:: this can be done in spawned thread
                node = &allocator[nk->PostID];
                count = *node->Data();
            }            
            //prefetch Value pointer (Cache Hack)
            const iBufferV* valPtr = (nk->ValID) ? &allocator[nk->ValID] : nullptr;

            //compare the keys
            KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size()-tare);
            
            if (c.Equal()) // found
                return valPtr;
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

inline iBufferV& Trie::GetOrAdd(const TypeVector<uint8_t>& key)
{
    uint64_t NodeID = 0;
    iBufferV* node = &allocator[NodeID];
    uint8_t count = *node->Data();
    NodeKey* nk = (NodeKey*)node->Data(1);
    const uint8_t* keyItr = key.begin();

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
            if (nk->PostID) {
                //TODO:: this can be done in spawned thread
                nextNodeID = nk->PostID;
                nextNode = &allocator[nk->PostID];
                count = *(nextNode->Data());
            }

            //compare the keys
            KeyCompare c(checkItr, keyItr, nk->KeyLen, key.Size());

            if (c.Equal()) { // found
                uint64_t ValID = nk->ValID;
                if (!ValID) {
                    ValID = allocator.Count();
                    nk->ValID = ValID;
                    allocator.New();// might invalidate nk pointer
                }                    
                return allocator[ValID];
            }  
            keyItr += c.commonPrefix;
            if (!c.lhsPostfix) {//full match ck check posts
                
                if (!nextNode) {//no posts, create them
                    uint64_t newNodeID = allocator.Count();
                    nk->PostID = newNodeID;
                    allocator.New(1 + sizeof(NodeKey) + key.Size() - (keyItr - key.Data())); //might invalidates nk pointer
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
                    return InsertNewKey(pushNodeID, c.lhsLesser ? 1 : 0, key, keyItr - key.Data());
                }
                else { //insert key in in this nodekey
                    uint64_t newValID = allocator.Count();
                    nk->ValID = newValID;
                    allocator.New(); // might invalidate nk pointer
                    return allocator[newValID];
                }
            }
        }//key is greater insert here
        return InsertNewKey(NodeID, i, key, keyItr - key.Data());
    } // no matches insert at end
    return InsertNewKey(NodeID, count, key, keyItr - key.Data());
}

inline void Trie::InitializeNode(iBufferV& nodeBuffer)
{
    nodeBuffer.Clear();
    nodeBuffer.AddBack(1);
    *nodeBuffer.Data() = 0;
}

inline iBufferV& Trie::InsertNewKey(uint64_t nodeID, uint8_t index, const TypeVector<uint8_t>& key, uint64_t tare)
{
    return InsertKey(allocator[nodeID], index, key, tare, allocator.New());
}

inline iBufferV& Trie::InsertKey(iBufferV& nodeBuffer, uint8_t index, const TypeVector<uint8_t>& key, uint64_t tare, uint64_t ValID, uint64_t PostID)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    //find index of insert
    uint8_t* newBuffer = nodeBuffer.AddInsert(GetKeyOffset(nodeBuffer, index), sizeof(NodeKey) + key.Size() - tare);
    new (newBuffer) NodeKey(key, tare, ValID, PostID);
    ++*(nodeBuffer.Data()); //increment count
    return allocator[ValID];
}

inline uint64_t Trie::PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen)
{
    uint64_t offset = GetKeyOffset(allocator[nodeID], index);
    NodeKey* nk = (NodeKey*)allocator[nodeID].Data(offset);
    offset += prefixLen + sizeof(NodeKey);

    //copy the key
    uint64_t PostFixLen = nk->KeyLen - prefixLen;
    nk->KeyLen = prefixLen;
    TypeVector<uint8_t> key;
    key.CopyBack(nk->Key() + prefixLen, PostFixLen);
    uint64_t ValID = nk->ValID;
    uint64_t PostID = nk->PostID;

    //create new node
    nk->PostID = allocator.Count();
    uint64_t newNodeID = nk->PostID;
    iBufferV& newNode = allocator[allocator.New(1 + PostFixLen + sizeof(NodeKey))]; //might invalidates nk pointer
    InitializeNode(newNode);

    //insert key
    InsertKey(newNode, 0, key, 0,ValID,PostID);
    
    //erase the postfix   
    allocator[nodeID].Remove(offset, PostFixLen); //Might invalidate NK pointer

    return newNodeID;
}

inline uint64_t Trie::GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index)
{
    o_assert_dbg(*(nodeBuffer.Data()) >= index); //index is less than count
    uint64_t offset = 1;
    NodeKey* itr = (NodeKey*)nodeBuffer.Data(1);
    for (uint8_t i = 0; i < index; i++) {
        offset += sizeof(NodeKey) + itr->KeyLen;
        itr = itr->Next();
    }
    return offset;
}

inline TypeVector<uint8_t> Trie::MakeKey(const char* key)
{
    TypeVector<uint8_t> newKey;
    newKey.CopyBack((const uint8_t*)key, std::strlen(key));
    return newKey;
}

} // namespace Oryol
