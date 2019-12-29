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
#include "ContainersPP/Containers/ElementBuffer.h"
#include "ContainersPP/Types/Types.h"

namespace ContainersPP {

template<class VALUE> 
class Trie 
{
public:
    /// default constructor
    Trie();
    /// copy constructor (truncates to actual size)
    Trie(const Trie& rhs);
    /// move constructor (same capacity and size)
    Trie(Trie&& rhs)
    /// copy-assignment operator (truncates to actual size)
    void operator=(const Trie& rhs);
    /// move-assignment operator (same capacity and size)
    void operator=(Trie&& rhs);
    
    /// get number of elements in array
    int Size() const;
    /// return true if empty
    bool Empty() const;
    /// clear the array (deletes elements, keeps capacity)
    void Clear();
    /// test if an element exists
    bool Contains(const Array<uint8>& val) const;
    /// find element
    const VALUE* Find(const Array<uint8>& val) const;
    /// add element
    VALUE* GetOrAdd(const Array<uint8>& val);
    /// erase element
    void Erase(const Array<uint8>& val);
    /// get value at index
    //const VALUE& ValueAtIndex(int index) const;
    
    /// C++ conform begin
    //VALUE* begin();
    /// C++ conform begin
    //const VALUE* begin() const;
    /// C++ conform end
    //VALUE* end();
    /// C++ conform end
    //const VALUE* end() const;
    
private:
    _priv::elementBuffer<uint8_t> keys;

};

} // namespace Oryol
