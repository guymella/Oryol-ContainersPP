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
#include "ContainersPP/Containers/InlineTable.h"
#include "ContainersPP/Types/Types.h"
#include "ContainersPP/Types/BitPointer.h"
#include "ContainersPP/Types/KeyString.h"


namespace ContainersPP {
   
    namespace PackKeyEnum {
        enum ValTypes : uint8_t {
            none,
            tiny,
            small,
            full
        };
    }



    struct PackKey {
        PackKey(const uint8_t* key, uint8_t size);
        uint8_t flags = 0;
        uint8_t KeyLen = 0;        
       
        PackKeyEnum::ValTypes ValueType() const;
        PackKeyEnum::ValTypes PostType()  const;
        uint8_t ValueSize() const;
        uint8_t PostSize() const;
        uint32_t PostID() const;
        const uint8_t* PostPtr() const { return Key() + KeyLen + ValueSize(); };  
        uint8_t* PostPtr() { return Key() + KeyLen + ValueSize(); };
        uint32_t Value() const;
        const uint8_t* ValuePtr() const { return Key() + KeyLen; }; 
        uint8_t* ValuePtr() { return Key() + KeyLen; };
        uint8_t* Key() { return (uint8_t*)(this + 1); }
        const uint8_t* Key() const { return (uint8_t*)(this + 1); }
        PackKey* Next() { return (PackKey*)(Key() + KeyLen + ValueSize() + PostSize()); }
        const PackKey* Next() const { return (PackKey*)(Key() + KeyLen + ValueSize() + PostSize()); };
        uint64_t Size() const { return (uint8_t*)Next() - (uint8_t*)this; };
    };

    struct PackKeyAdapter {
        PackKeyAdapter(iBlockD& Container, uint32_t StartOffset) : block(Container), startOffset(StartOffset) {};
        iBlockD& block;
        uint32_t startOffset;
        void Insert(const uint8_t* key, uint8_t size);

        PackKey& Key() { return *(PackKey*)block.Data(startOffset); };

        void Truncate(uint8_t Tare, uint32_t PostID, bool ClearValue = true);
        void SetPostID(uint32_t ID);
        bool ClearPostID();
        void SetValue(uint32_t val);
        bool ClearValue();
    };

    
    class iIndexTrieNode
    {
    public:
        virtual uint32_t ID() const = 0;
        const PackKey* Find(const uint8_t* key) const;
        PackKey* Find(const uint8_t* key);
        void Erase(PackKey* key);
        uint8_t Count() const { return *Block().Data(); };
        friend class iIndexTrie;
    protected:
        virtual iBlockD& Block() = 0;
        virtual const iBlockD& Block() const = 0;
    private:
        PackKeyAdapter InsertNewKey(const uint8_t* key, uint8_t Keylen,uint32_t val= std::numeric_limits<uint32_t>::max(), uint32_t PostID = 0);
        PackKeyAdapter InsertKey(const PackKey* CopyKey, uint8_t Tare = 0);
        PackKeyAdapter PushDown(PackKey* breakKey, uint8_t prefixLen, iIndexTrieNode& PushNode);
        uint64_t GetKeyOffset(uint64_t index);
        PackKey* GetKey(uint64_t index);
        uint8_t& Count() { return *Block().Data(); };
    };

    class Node_Adapter : public iIndexTrieNode
    {
    public:
        virtual uint32_t ID() const { return (uint32_t)block.ID(); };
        friend class iIndexTrie;
    protected:
        virtual iBlockD& Block() override { return block; };
        virtual const iBlockD& Block() const override { return block; };
    private:
        InlinePartition block;
    };


    class iIndexTrie
    {
    public:
        /// get number of elements in array
        uint64_t Size() const { return 0; };
        /// return true if empty
        bool Empty() const { return !Size(); };
        /// clear the array (deletes elements, keeps capacity)
        void Clear() {};
        /// test if an element exists
        bool Contains(const uint8_t* key, uint8_t KeyLen) const { return Find(key, KeyLen); };
        bool Contains(const Types::KeyString& key) const { return Contains(key.Data(), (uint8_t)key.Size()); };
        bool Contains(const char* key) const { return Contains(Types::MakeKey(key)); };
        bool Contains(uint64_t key) const { return Contains(Types::MakeKey(key)); };
        /// find element
        virtual const PackKey* Find(const uint8_t* key, uint8_t KeyLen) const;
        const PackKey* Find(const Types::KeyString& key) const { return Find(key.Data(), (uint8_t)key.Size()); };
        const PackKey* Find(const char* key) const { return Find(Types::MakeKey(key)); };
        const PackKey* Find(uint64_t key) const { return Find(Types::MakeKey(key)); };

        virtual PackKey* Find(const uint8_t* key, uint8_t KeyLen) { return const_cast<PackKey*>(((const iIndexTrie*)this)->Find(key, KeyLen)); }
        PackKey* Find(const Types::KeyString& key) { return Find(key.Data(), (uint8_t)key.Size()); };
        PackKey* Find(const char* key) { return Find(Types::MakeKey(key)); };
        PackKey* Find(uint64_t key) { return Find(Types::MakeKey(key)); };
        
        /// find or add element
        virtual PackKeyAdapter Get(const uint8_t* key, uint8_t KeyLen);
        PackKeyAdapter Get(const Types::KeyString& key) { return Get(key.Data(), (uint8_t)key.Size()); };
        PackKeyAdapter Get(const char* key) { return Get(Types::MakeKey(key)); };
        PackKeyAdapter Get(uint64_t key) { return Get(Types::MakeKey(key)); };
        /// erase element
        virtual void Erase(const uint8_t* key, uint8_t KeyLen);
        void Erase(const Types::KeyString& key) { return Erase(key.Data(), (uint8_t)key.Size()); };
        void Erase(const char* key) { return Erase(Types::MakeKey(key)); };
        void Erase(uint64_t key) { return Erase(Types::MakeKey(key)); };
    
    protected:
        virtual Node_Adapter Nodes(uint32_t Index) = 0;
        virtual const Node_Adapter Nodes(uint32_t Index) const = 0;
        virtual Node_Adapter NewNode() = 0;
        //static void InitializeNode(iBufferV& nodeBuffer);
    private:
        
        //TYPE& InsertNewKey(uint64_t nodeID, uint8_t index, const Types::KeyString& key, uint64_t tare);
        /*CatKey* InsertKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, CatValEnums::CatValTypes type, CatVal val, uint64_t PostID = 0);
        void InsertEmptyKey(iBufferV& nodeBuffer, uint8_t index, const Types::KeyString& key, uint64_t tare, uint64_t PostID = 0);
        uint64_t PushDown(uint64_t nodeID, uint64_t index, uint64_t prefixLen);
        static uint64_t GetKeyOffset(const iBufferV& nodeBuffer, uint64_t index);    */   
        

    };

   


} // namespace 
