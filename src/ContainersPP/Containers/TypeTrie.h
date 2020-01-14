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

} // namespace Oryol
