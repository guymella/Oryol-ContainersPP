#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::Block
    @ingroup Core
    @brief growable memory Block for raw data
*/

#include "Core/Memory/Memory.h"
//#include "BlockStatic.h"
#include "Block.h"
#include <stdio.h>

namespace ContainersPP {

    class FileBlock : public Block {
    public:
        /// default constructor
        FileBlock() {};
        FileBlock(const char* FileName) { 
            CopyFileName(FileName);
            Read();
        };
        FileBlock(const char* foldername, uint64_t FileID) { 
            uint64_t len = std::strlen(foldername);
            filename = (char*)Oryol::Memory::Alloc((int)len +9);
            Oryol::Memory::Copy(foldername, filename, (int)len);
            Types::n2hexstr(FileID, filename+len);
            Read();
        };
        /// Copy construct
        //FileBlock(const FileBlock& rhs);
        /// Copy construct
        //FileBlock(const iBlockS& rhs);
        /// move constructor
        FileBlock(FileBlock&& rhs);
        /// destructor
        ~FileBlock();

        /// move-assignment
        void operator=(FileBlock&& rhs);
        /// Copy-assignment
        //void operator=(const FileBlock& rhs);

        /// Copy-assignment
        //void operator=(const iBlockS& rhs);
        
        bool Save();
        uint64_t Read();
        bool Delete();
    protected:
        void CopyFileName(const char* FileName);
       
    private:
        char * filename = 0;
    };

    inline FileBlock::FileBlock(FileBlock&& rhs)
    {         
         if (rhs.filename) {
             CopyFileName(rhs.filename);
             Block::move(std::move(rhs));
             Oryol::Memory::Free(rhs.filename);
             rhs.filename = nullptr;
         }
    }

    inline FileBlock::~FileBlock()
    {
        if (filename) {
            Save();
            Oryol::Memory::Free(filename);
            filename = nullptr;
        }
        Block::~Block();       
    }

    inline void FileBlock::operator=(FileBlock&& rhs)
    {
        if (rhs.filename) {
            CopyFileName(rhs.filename);
            Block::move(std::move(rhs));
            Oryol::Memory::Free(rhs.filename);
            rhs.filename = nullptr;
        }
    }

    inline bool ContainersPP::FileBlock::Save()
    {
        if (!filename)
            return false;

        if (Empty()) {
            remove(filename);
            return false;
        }
        FILE* pFile;
        #pragma warning(suppress : 4996) //TODO:: make windows def
        pFile = fopen(filename, "wb");
        if (pFile != NULL)
        {
            uint64_t size = Size();
            fwrite(&size, sizeof(uint64_t), 1, pFile);
            if (size) {                
                fwrite(Data(), sizeof(uint8_t), Size(), pFile);
            }
            fclose(pFile);
            return true;
        }
        return false;
    }

    inline uint64_t FileBlock::Read()
    {
        Clear();
        FILE* pFile;
        #pragma warning(suppress : 4996)
        pFile = fopen(filename, "rb");
        if (pFile != NULL)
        {
            uint64_t size = 0;
            if (fread(&size, sizeof(uint64_t), 1, pFile) && size)
                fread(AddBack(size), sizeof(uint8_t), size, pFile);
            fclose(pFile);
            return size;
        }
        return 0;
    }

    inline bool FileBlock::Delete()
    {
        Clear();
        //remove(filename);
        return !remove(filename);
    }

    inline void FileBlock::CopyFileName(const char* FileName)
    {
        if (filename)
            Oryol::Memory::Free(filename);
        uint64_t strSize = std::strlen(FileName) + 1;
        filename = (char*)Oryol::Memory::Alloc((int)strSize);
        Oryol::Memory::Copy(FileName, filename, (int)strSize);
    }

} // namespace Oryol
