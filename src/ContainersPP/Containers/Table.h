//------------------------------------------------------------------------------
//  Table.h
//------------------------------------------------------------------------------

#ifndef _TABLE_DEFINED
#define _TABLE_DEFINED

#include "ContainersPP/Interfaces/iTable.h"
#include "Buffer.h"

namespace ContainersPP {

	class Table : public iTable {
	public:
        Table() {};
        Table(const Table& rhs) : _index(rhs._index), buffer(rhs.buffer) {};
        Table(Table&& rhs) : _index(std::move(rhs._index)), buffer(std::move(rhs.buffer)) {};

    protected:
        virtual TypeBuffer<Partition>& Index() override { return _index; };
        virtual const TypeBuffer<Partition>& Index() const override { return _index; };
        virtual iBlockD& Buffer() override { return buffer; };
        virtual const iBlockD& Buffer() const override { return buffer; };
		
    private:
        TypeBuffer<Partition> _index;
        ContainersPP::Buffer buffer;
	};

    class RefTable : public iTable {
    public:
        RefTable(ContainersPP::Buffer* bufferPtr) : buffer(bufferPtr) {};
        RefTable(const RefTable& rhs) : _index(rhs._index), buffer(rhs.buffer) {};
        RefTable(RefTable&& rhs) : _index(std::move(rhs._index)), buffer(rhs.buffer) { rhs.buffer = nullptr; };

    protected:
        virtual TypeBuffer<Partition>& Index() override { return _index; };
        virtual const TypeBuffer<Partition>& Index() const override { return _index; };
        virtual iBlockD& Buffer() override { return *buffer; };
        virtual const iBlockD& Buffer() const override { return *buffer; };

    private:
        TypeBuffer<Partition> _index;
        ContainersPP::Buffer* buffer;
    };

    class AllocatedTable : public iTable {
    public:
        AllocatedTable(Allocator* alocatorPtr, uint64_t blockID) : allocator(alocatorPtr), BlockID(blockID) {};
        AllocatedTable(const AllocatedTable& rhs) : _index(rhs._index), allocator(rhs.allocator), BlockID(rhs.BlockID) {};
        AllocatedTable(AllocatedTable&& rhs) : _index(std::move(rhs._index)), allocator(rhs.allocator), BlockID(rhs.BlockID) { rhs.allocator = nullptr; rhs.BlockID = 0; };
    protected:
        virtual TypeBuffer<Partition>& Index() override { return _index; };
        virtual const TypeBuffer<Partition>& Index() const override { return _index; };
        virtual iBlockD& Buffer() override { return (*allocator)[BlockID]; };
        virtual const iBlockD& Buffer() const override { return (*allocator)[BlockID]; };

    private:
        TypeBuffer<Partition> _index;
        Allocator* allocator;
        uint64_t BlockID;
    };
          
}//contanersPP

#endif //defined