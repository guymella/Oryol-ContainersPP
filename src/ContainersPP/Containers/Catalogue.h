#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Catalogue
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Types/Types.h"
//#include "Core/Assertion.h"
//#include "Core/Memory/Memory.h"
#include "FileBlock.h"
#include "TypeBuffer.h"
#include "Allocator.h"

namespace ContainersPP {

class Catalogue  {
public:
    Catalogue(const char* SystemRoot);

protected:
    //iTrie<Catalogue> ClusterMap();
    TypeVector<iAllocator*>& SystemAllocators();

private:
    TypeVector<iAllocator*> systemAllocators;
    FileBlock root;
};

ContainersPP::Catalogue::Catalogue(const char* SystemRoot) : root(SystemRoot)
{
    SystemAllocators().PushBack(new Allocator());
    uint8_t* end = root.Data(root.Size());

    for (uint8_t* itr = root.Data(); itr < end; itr++)
        if (*itr == 0)
            SystemAllocators().PushBack(new FileAllocator((const char *)(++itr)));

}



} // namespace Oryol
