#pragma once
//------------------------------------------------------------------------------
/**
    @class ContainersPP::Catalogue
    @ingroup Core
    @brief growable memory buffer for raw data
*/
//#include "ContainersPP/Types/TypeDescr.h"
#include "ContainersPP/Interfaces/iObject.h"
//#include "Core/Assertion.h"
//#include "Core/Memory/Memory.h"
//#include "FileBlock.h"
//#include "TypeBuffer.h"
//#include "Allocator.h"

namespace ContainersPP {

    class Catalogue : public iCatalogue {
    public:
        virtual Types::baseTypes Type() const override { return Types::baseTypes::Struct; };
        virtual void* Ptr() override { return nullptr; };
        virtual const void* Ptr() const override { return nullptr; };
        virtual void* Begin(uint64_t Index = 0) override { return nullptr; };
        virtual const void* Begin(uint64_t Index = 0) const override { return nullptr; };
        virtual bool Remove(uint32_t Index) override { return false; };
        virtual uint64_t Count() const override { return 0; };
        virtual iCatalogue& GetRef(const Types::KeyString& key) override;
        virtual iCatalogue& GetRef(const Types::KeyChain& keys, uint32_t keyindex = 0) override;
        virtual iCatalogue& GetRef(uint64_t Index) override;
        virtual iCatalogue& CDRRef(uint64_t BaseIndex) override;
        virtual Object Get(const Types::KeyString& key) override;
        virtual Object Get(const Types::KeyChain& keys, uint32_t keyindex = 0) override;
        virtual Object Get(uint64_t Index) override;
        virtual Object CDR(uint64_t BaseIndex) override;
    protected:
        virtual bool pushBack() override { return false; };
        virtual bool pushFront() override { return false; };
        virtual bool insert(uint32_t Index) override { return false; };
    private:
        iBlockD& cdr_Block();
        iBlockD& Instances_Block();

    };




} // namespace Oryol
