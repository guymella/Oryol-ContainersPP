#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Catalogue
    @ingroup Core
    @brief growable memory buffer for raw data
*/


#ifndef Included_CATALOGUEALLOCATOR_H
#define Included_CATALOGUEALLOCATOR_H



//#include "ContainersPP/Types/TypeDescr.h"
#include "Catalogue.h"
//#include "TypeTrie.h"
//#include "Core/Assertion.h"
//#include "Core/Memory/Memory.h"
//#include "FileBlock.h"
//#include "TypeBuffer.h"
//#include "Allocator.h"

namespace ContainersPP {

   class iCatalogueAllocator : public iCatalogue {

    };

   class CatalogueFileAllocator : public iCatalogueAllocator {
   public:
       CatalogueFileAllocator() {};

   private:
       TypeBuffer<FileCatalogue> objects;

   };

    class CatalogueAllocator : public iCatalogueAllocator {
    public:
        CatalogueAllocator() {};

    private:
        TypeBuffer<Catalogue> objects;
        
    };




} // namespace Oryol


#endif