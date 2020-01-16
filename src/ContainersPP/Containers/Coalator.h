//------------------------------------------------------------------------------
//  Coalator.h
//------------------------------------------------------------------------------

#ifndef _COALATOR_DEFINED
#define _COALATOR_DEFINED

#include "ContainersPP/Containers/Table.h"
#include "ContainersPP/Containers/TypeTrie.h"
//#include "Buffer.h"

namespace ContainersPP {

	class Coalator : public Table {
	public:
        Coalator() {};
        Coalator(const Coalator& rhs)  {};
        Coalator(Coalator&& rhs) {};

        virtual iBlockD& operator[](uint64_t ID) override;
        virtual const iBlockD& operator[](uint64_t ID) const override;       

        iBlockD& Assign(uint64_t ID, uint64_t NewSize);
        iBlockD& Assign(uint64_t ID, uint64_t NewSize, uint64_t StoreAfterID);
    protected:
        virtual void IncrementPartitions(uint64_t index) override {}; //lookup manages partionids
        virtual void DecrementPartitions(uint64_t index)override {}; //lookup manages partionids
        virtual void UpdateFollowingOffsets(uint64_t index, int64_t offsetDelta) override;

        TypeTrie<uint64_t>& Lookup() { return lookup; };
        const TypeTrie<uint64_t>& Lookup() const { return lookup; };
    private:
        TypeTrie<uint64_t> lookup;
	};
    
    inline iBlockD& Coalator::operator[](uint64_t ID)
    {
        o_assert_dbg(Lookup().Contains(ID));
        return Index()[Lookup().Find(ID)[0]];
    }

    inline const iBlockD& Coalator::operator[](uint64_t ID) const
    {
        o_assert_dbg(Lookup().Contains(ID));
        return Index()[Lookup().Find(ID)[0]];
    }

    inline iBlockD& ContainersPP::Coalator::Assign(uint64_t ID, uint64_t NewSize)
    {
        uint64_t newIndex = New(NewSize);
        Index()[newIndex].PartitionID = ID;
        Lookup().GetOrAdd(ID) = newIndex;
        return Index()[newIndex];
    }

    inline iBlockD& Coalator::Assign(uint64_t ID, uint64_t NewSize, uint64_t StoreAfterID)
    {
        o_assert_dbg(Lookup().Contains(StoreAfterID));

        uint64_t insertIndex = Lookup().GetOrAdd(StoreAfterID)+1;
        Insert(insertIndex, NewSize);
        Index()[insertIndex].PartitionID = ID;
        Lookup().GetOrAdd(ID) = insertIndex;
        //update lookup indexes
        for (uint64_t i = insertIndex + 1; i < Index().Size(); ++i)
            ++(Lookup().GetOrAdd(Index()[i].PartitionID));

        return Index()[insertIndex];
    }

    inline void Coalator::UpdateFollowingOffsets(uint64_t ID, int64_t offsetDelta)
    {
        o_assert_dbg(Lookup().Contains(ID));
        uint64_t index = Lookup().GetOrAdd(ID);
        iTable::UpdateFollowingOffsets(index, offsetDelta);
    }

}//contanersPP

#endif //defined