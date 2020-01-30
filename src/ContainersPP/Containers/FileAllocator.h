#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Allocator
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
#include "Core/Assertion.h"
#include "Core/Memory/Memory.h"
#include "ContainersPP/Containers/TypeTrie.h"
#include "ContainersPP/Containers/FileBlock.h"
#include "ContainersPP/Interfaces/iAllocator.h"



namespace ContainersPP {

class FileAllocator : public iAllocator {
public:
    /// default constructor
    FileAllocator(const char* AllocationFolder);
    /// Copy constructor
    //Allocator(const Allocator& rhs) { buffers = rhs.buffers; };
    /// move constructor
    //Allocator(Allocator&& rhs) { buffers = std::move(rhs.buffers); };
    /// destructor
    ~FileAllocator() { Oryol::Memory::Free(folderName); };

    virtual iBlockD& operator[](uint64_t index) override { 
        Load(index);
        return buffers.GetOrAdd(index);
    };
    virtual const iBlockD& operator[](uint64_t index) const override {
        //Load(index);
        const iBlockD* block = buffers.Find(index);
        //if (block) 
            return *block;
    };

    virtual uint64_t New() override;
    virtual uint64_t New(uint64_t newSize) override;

    bool Load(uint64_t BlockID);
    bool Save(uint64_t BlockID);
    bool UnLoad(uint64_t BlockID);
    bool Delete(uint64_t BlockID);

    /// get number of buffers
    virtual uint64_t Count() const override { return buffers.Size(); };
   /* /// return true if empty
    bool Empty() const { return !Count(); };*/
private:
    char* folderName = 0;
    uint64_t NextID(uint64_t lastID = 0) const;
    TypeTrie<FileBlock> buffers;
};

inline FileAllocator::FileAllocator(const char* AllocationFolder)
{
    uint64_t len = std::strlen(AllocationFolder);
    folderName = (char*)Oryol::Memory::Alloc((int)len + 9);
    Oryol::Memory::Copy(AllocationFolder, folderName, (int)len);  
    folderName[len] = 0;
}

uint64_t ContainersPP::FileAllocator::New()
{
    uint64_t id = NextID();
    FileBlock* blockPtr = &buffers.GetOrAdd(id);
    new (blockPtr) FileBlock(folderName,id);
    return id;
}

inline uint64_t FileAllocator::New(uint64_t newSize)
{
    return New();
}

inline bool FileAllocator::Load(uint64_t BlockID)
{
    if (buffers.Contains(BlockID)) 
        return true;

    FileBlock* blockPtr = &buffers.GetOrAdd(BlockID);    
    new (blockPtr) FileBlock(folderName, BlockID);

    return blockPtr->Size();
}

inline bool FileAllocator::Save(uint64_t BlockID)
{
    if (buffers.Contains(BlockID))
        return buffers.GetOrAdd(BlockID).Save();

    return false;
}

inline bool FileAllocator::UnLoad(uint64_t BlockID)
{
    if (buffers.Contains(BlockID))
        buffers.Erase(BlockID);
    else
        return false;

    return true;
}

inline bool FileAllocator::Delete(uint64_t BlockID)
{
    if (buffers.Contains(BlockID)) {
        buffers.GetOrAdd(BlockID).Delete();
        buffers.Erase(BlockID);
        return true;
    }
    
    //if file exists
    FileBlock f(folderName, BlockID);
    if (f.Size()){
        f.Delete();
        return true;
    }
    return false;
}

inline uint64_t FileAllocator::NextID(uint64_t lastID) const
{
    uint64_t len = std::strlen(folderName);
    char* fname = (char* )Oryol::Memory::Alloc((int)len + 9);
    Oryol::Memory::Copy(folderName, fname, (int)len);
    FILE* pFile = nullptr;
    bool exists;
    do {
        exists = false;
        if (buffers.Contains(++lastID)) {
            exists = true;
            continue;
        }
        //check folder
        Types::n2hexstr(lastID, fname + len);
        #pragma warning(suppress : 4996)
        pFile = fopen(fname, "rb");
    } while (exists || (pFile && fclose(pFile)));
        
    //fclose(pFile);   
    
    return lastID;
}






//struct BufferPtr {
//    BufferPtr(Allocator* _allocator, uint64_t bufferId) : allocator(_allocator), BufferId(bufferId) {};
//    Allocator* allocator;
//    uint64_t BufferId;
//    Buffer& get() { return (*allocator)[BufferId]; };
//    const Buffer& get() const { return (*allocator)[BufferId]; };
//};


//
//
//class CoAllocator : public iAllocator {
//public:
//    /// default constructor
//    CoAllocator() {};
//    CoAllocator(Allocator* ParentAllocator) : allocator(ParentAllocator) {};
//    /// Copy constructor
//    CoAllocator(const CoAllocator& rhs) {//TODO:: Copy Buffers, make new index with copies
//        _index = rhs._index;
//        allocator = rhs.allocator;
//    }
//    /// move constructor
//    CoAllocator(CoAllocator&& rhs) { 
//        _index = std::move(rhs._index);
//        allocator = rhs.allocator;
//        rhs.allocator = nullptr;
//    };
//    /// destructor
//    //~Allocator();
//    void operator=(CoAllocator&& rhs) {
//        _index = std::move(rhs._index);
//        allocator = rhs.allocator;
//        rhs.allocator = nullptr;
//    };
//
//    void operator=(const CoAllocator& rhs) {
//        _index = rhs._index;
//        allocator = rhs.allocator;
//    };
//
//    virtual Buffer& operator[](uint64_t index) override { return (*allocator)[_index[index]]; };
//    virtual const Buffer& operator[](uint64_t index) const override { return (*allocator)[_index[index]]; };
//
//    virtual uint64_t New() override ;
//    virtual uint64_t New(uint64_t newSize) override;
//
//    /// get number of buffers
//    virtual uint64_t Count() const override { return _index.Size(); };
//    
//private:
//    Allocator* allocator = 0;
//    TypeVector<uint64_t> _index;
//};
//
//uint64_t ContainersPP::CoAllocator::New()
//{
//    uint64_t newID = Count();
//    _index.PushBack(allocator->New());
//    return newID;
//}
//
//inline uint64_t CoAllocator::New(uint64_t newSize)
//{
//    uint64_t newID = Count();
//    _index.PushBack(allocator->New(newSize));
//    return newID;
//}
//


} // namespace Oryol
