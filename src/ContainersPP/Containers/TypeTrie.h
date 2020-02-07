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
//#include "ContainersPP/Containers/Allocator.h"
//#include "ContainersPP/Types/Types.h"
//#include "ContainersPP/Types/KeyString.h"

#include "ContainersPP/Interfaces/iTrie.h"
#include "ContainersPP/Interfaces/iCatalogueTrie.h"
#include "ContainersPP/Interfaces/iIndexTrie.h"

namespace ContainersPP {

template<typename TYPE>
class TypeTrie : public iTrie<TYPE>
{
public:
    TypeTrie();
protected:
    virtual Allocator& Nodes() override { return allocator; };
    virtual const Allocator& Nodes() const override { return allocator; };
    
private:
    Allocator allocator;

};

template<typename TYPE>
inline TypeTrie<TYPE>::TypeTrie()
{
    o_assert_dbg(allocator.Count() == 0);
    InitializeNode(Nodes()[Nodes().New(32)]);
    o_assert_dbg(allocator.Count() == 1);
}

template<typename TYPE>
class TypeTrieRef : public iTrie<TYPE>
{
public:
    TypeTrieRef(Allocator* Allocator) : allocator(Allocator) { InitializeNode(Nodes()[Nodes().New(32)]); };
    TypeTrieRef(Allocator* Allocator, uint64_t HeadBlockID) : allocator(Allocator),headBlockID(HeadBlockID) {};
protected:
    virtual Allocator& Nodes() override { return *allocator; };
    virtual const Allocator& Nodes() const override { return *allocator; };

private:
    Allocator* allocator;
    uint64_t headBlockID;


};




//////////////////




class CatTrie : public iCatTrie
{
public:
    CatTrie();
protected:
    virtual Allocator& Nodes() override { return allocator; };
    virtual const Allocator& Nodes() const override { return allocator; };

private:
    Allocator allocator;

};


inline CatTrie::CatTrie()
{
    o_assert_dbg(allocator.Count() == 0);
    InitializeNode(Nodes()[Nodes().New(32)]);
    o_assert_dbg(allocator.Count() == 1);
}


class CatTrieRef : public iCatTrie
{
public:
    CatTrieRef(Allocator* Allocator) : allocator(Allocator) { InitializeNode(Nodes()[Nodes().New(32)]); };
    CatTrieRef(Allocator* Allocator, uint64_t HeadBlockID) : allocator(Allocator), headBlockID(HeadBlockID) {};
protected:
    virtual Allocator& Nodes() override { return *allocator; };
    virtual const Allocator& Nodes() const override { return *allocator; };

private:
    Allocator* allocator;
    uint64_t headBlockID;


};



//////////////////




class IndexTrie : public iIndexTrie
{
public:
    IndexTrie() { NewNode(); };
protected:
    virtual Node_Adapter Nodes(uint32_t Index) override { return Node_Adapter(table[Index]); };
    virtual const Node_Adapter Nodes(uint32_t Index) const override { return Node_Adapter(table[Index]); };
    virtual Node_Adapter NewNode() {
        InlinePartition p = table[table.New(1)];
        p.Data()[0] = 0;
        return Node_Adapter(p);
    }
private:
    InlineTable table;
};

class SubIndexTrie : public iIndexTrie
{
public:
    SubIndexTrie(iInlineTable* Table, uint64_t BlockID) : table(Table,BlockID) { NewNode(); };
protected:
    virtual Node_Adapter Nodes(uint32_t Index) override { return Node_Adapter(table[Index]); };
    virtual const Node_Adapter Nodes(uint32_t Index) const override { return Node_Adapter(table[Index]); };
    virtual Node_Adapter NewNode() {
        InlinePartition p = table[table.New(1)];
        p.Data()[0] = 0;
        return Node_Adapter(p);
    }
private:
    InlineSubTable table;
};

} // namespace Oryol
