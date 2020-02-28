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

   /*class iCatalogueAllocator : public iCatalogue {

    };*/

   //class CatalogueFileAllocator : public iCatalogueAllocator {
   //public:
   //    CatalogueFileAllocator() {};


   //private:
   //    TypeBuffer<FileCatalogue> objects;

   //};

    class CatalogueAllocator : public iProfile {
    public:
        CatalogueAllocator() {};       
        ~CatalogueAllocator();

        virtual iCatalogue& operator[](uint64_t index) override {
            if (index < objects.Size())
                return *(objects[index]);
            return NULLOBJECT;
        };
        virtual const iCatalogue& operator[](uint64_t index) const  override {
            if (index < objects.Size())
                return *(objects[index]);
            return NULLOBJECT;
        };

        CatalogueBase* Get(uint64_t index) { return objects[index]; };
        const CatalogueBase* Get(uint64_t index) const { return objects[index]; };

        virtual uint64_t New() override;
        virtual bool Delete(uint64_t ID) override;
        /// get number of buffers
        virtual uint64_t Count() const { return objects.Size(); };

    private:
        void NextID();
        uint64_t nextid = 0;
        TypeBuffer<Catalogue*> objects;        
    };

    inline CatalogueAllocator::~CatalogueAllocator()
    {
        for (uint64_t i = 0; i < objects.Size(); i++)
            delete objects[i];
    }

    inline uint64_t ContainersPP::CatalogueAllocator::New()
    {
        uint64_t id = nextid;
        if (id == objects.Size())
            objects.PushBack(new Catalogue(this));
        else {
            objects[id] = new Catalogue(this);
            NextID();
        }
        return id;
    }

    inline bool CatalogueAllocator::Delete(uint64_t ID)
    {
        if (ID < objects.Size()) {
            delete objects[ID];
            objects[ID] = nullptr;
            if (ID < nextid)
                nextid = ID;
            return true;
        }
        return false;
    }

    inline void CatalogueAllocator::NextID()
    {
        if (nextid >= objects.Size())
            nextid = 0;

        for (nextid; nextid < objects.Size(); nextid++) {
            if (!objects[nextid])
                return;
        }
        nextid = std::numeric_limits<uint64_t>::max();
    }


} // namespace Oryol


#endif