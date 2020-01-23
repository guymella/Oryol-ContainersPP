//------------------------------------------------------------------------------
//  Schema.h
//------------------------------------------------------------------------------

#ifndef _SCHEMA_DEF
#define _SCHEMA_DEF




#include "TypeDescr.h"
#include "BitPointer.h"

//#include "Structures/Containers/SparseArray.h"

namespace ContainersPP {
	namespace Types {

		class Schema {
		public:
			Schema() { init(); };

			void WriteDefaults(iBlockD& MainBlock) const;

			size_t SizeOfFixed();
			size_t SizeOfFixed() const;
			size_t SizeOfBools();
			size_t SizeOfBools() const;
			uint64_t SeperatedColumnCount() const;
			size_t GetElmSize(size_t index) const;
			size_t BlockCount(uint8_t BlockIndex) const; //returns number of elements in the sequence block
			size_t BlockCount(Types::TypeSequence start, uint8_t end) const; //returns number of elements in the sequence block
			size_t FindIndex(const KeyString& name) const;
			size_t FindIndex(const char* name) const;
			bool AddAttribute(const KeyString& name, Types::TypeDescr type);
			bool AddAttribute(const char* name, Types::baseTypes type);			

			const TypeDescr& GetTypeDescr(uint64_t columnIndex) const { return types[columnIndex]; };
			uint8_t* GetValuePointer(uint64_t columnIndex, iBlockD& MainBlock) const;
			BitItr<uint8_t,1> GetBoolPointer(uint64_t columnIndex, iBlockD& MainBlock) const;
			uint64_t GetOffset(uint64_t columnIndex) const { return offsets[columnIndex]; };

			uint8_t* SetColumnValue(uint64_t columnIndex, iBlockD& MainBlock) const;
			void SetColumnNull(uint64_t columnIndex, iBlockD& MainBlock) const;

		//private:
			void RebuildOffsets();
			size_t SequenceStart(TypeSequence seq) const;
			void init();
			size_t fixedSize = 0;
			bool extendable = false;
			TypeVector<size_t> Sequence;
			TypeVector<KeyString> labels;
			TypeVector<Types::TypeDescr> types;
			TypeVector<uint64_t> offsets;
			//SparseArray<Darivation> Darivations
			//TypeVector<Types::Constraint> Constraints;
			//SparseArray<Schema*> SubSchemas;
		};





		inline void Schema::WriteDefaults(iBlockD& MainBlock) const
		{
			MainBlock.Clear();
			MainBlock.AddBack(SizeOfFixed());

			uint64_t index = 0;
			//Write Default Null FLags
			uint64_t Count = BlockCount(0);
			BitItr<uint8_t,1> bi(MainBlock.Data());
			for (index; index < Count; ++index) {
					bi[index] = (bool)(types[index].constraints.Contains(MakeKey("DEFAULT")));
					if (bi[index]) {//has non null default
						DataRange df = types[index].DefaultValue();
						MainBlock.CopyOver(offsets[index], df.size, df.data, df.size); //write the default value
					}
			}
			//Write Default sparse FLags
			Count += BlockCount(1);
			for (index; index < Count; ++index) {
				bi[index] = false; // alwais starts as false
			}
			//Write Default bool FLags
			bi += Count;
			Count += BlockCount(2);
			for (index; index < Count; ++index) {
				auto argp = types[index].constraints.Contains(MakeKey("FixedSize"));
				uint64_t bits = 1;
				
				if (argp) //get bit count
					bits = *((uint64_t*)(argp->begin()));
				
				if (types[index].constraints.Contains(MakeKey("DEFAULT"))) { //copy default bits
					DataRange df = types[index].DefaultValue();
					BitItr<uint8_t, 1> bd(df.data);
					for (uint64_t i = 0; i < bits; ++i)
						bi[i] = bd[i];
				}
				else {
					for (uint64_t i = 0; i < bits; ++i)
						bi[i] = false;
				}

				bi += bits;
			}

			Count += BlockCount(3);
			for (index; index < Count; ++index) {
				DataRange df = types[index].DefaultValue();
				MainBlock.CopyOver(offsets[index], df.size, df.data, df.size); //write the default value
			}

		}

		inline size_t Schema::SizeOfFixed()
		{
			if (fixedSize < std::numeric_limits<size_t>::max())
				return fixedSize;

			fixedSize = std::as_const(*this).SizeOfFixed();

			return fixedSize;
		}


		inline size_t Schema::SizeOfFixed() const
		{
			if (fixedSize < std::numeric_limits<size_t>::max())
				return fixedSize;
			size_t nul = BlockCount(0);
			size_t sps = BlockCount(1);
			size_t Size = SizeOfBools() + nul + sps;
			Size = (Size / 8) + ((Size % 8) ? 1 : 0); //all flags

			size_t fix = BlockCount(3);
			size_t secEnd = SequenceStart(TypeSequence::Columnar);
			for (size_t i = SequenceStart(TypeSequence::Fixed); i < secEnd; i++)// fixed cols
				Size += GetElmSize(i);

			secEnd = SequenceStart(TypeSequence::Sparse);
			for (size_t i = SequenceStart(TypeSequence::Nullable); i < secEnd; i++)//null cols
				Size += GetElmSize(i);

			return Size;
		}

		inline size_t Schema::SizeOfBools()
		{
			size_t bools = BlockCount(2);
			size_t size = bools;
			for (size_t i = BlockCount(0)+ BlockCount(1); i < bools; i++) {
				auto argp = types[i].constraints.Contains(MakeKey("FixedSize"));
				if (argp) {
					size--;
					size += *((size_t*)(argp->begin()));
				}				
			}
			return size;
		}

		inline size_t Schema::SizeOfBools() const
		{
			size_t bools = BlockCount(2);
			size_t size = bools;
			for (size_t i = BlockCount(0) + BlockCount(1); i < bools; i++) {
				auto argp = types[i].constraints.Contains(MakeKey("FixedSize"));
				if (argp) {
					size--;
					size += *((size_t*)(argp->begin()));
				}				
			}
			return size;
		}

		inline uint64_t Schema::SeperatedColumnCount() const
		{
			return BlockCount(4) + BlockCount(5) + BlockCount(6);
		}

		inline size_t Schema::GetElmSize(size_t index) const
		{
			if (types[index].getTypeSequence() >= Types::TypeSequence::Multi)
				return std::numeric_limits<size_t>::max(); //size unknown

			size_t size = Types::SizeOf(types[index].type);
			if (!size)
				return 0;//size is null

			if (types[index].Columnar())//fixed size is adjacency index
				return sizeof(size_t);

			//get constraints
			//const Types::Constraint* cst = Constraints.Exists(index);

			if (types[index].type == Types::baseTypes::Struct) {
				auto sch = types[index].constraints.Contains(Types::MakeKey("Schema"));
				if (sch)
					size = ((Schema*)sch->begin())->SizeOfFixed();
				
			}


			if (size < std::numeric_limits<size_t>::max()) { //size is fixed
				auto dims = types[index].constraints.Contains(Types::MakeKey("dim"));
				if (dims) {
					size_t dimCount = dims->Size() / sizeof(size_t);
					for (size_t i = 0; i < dimCount; i++)
						size *= ((size_t*)dims->begin())[i];
				}
				auto fs = types[index].constraints.Contains(Types::MakeKey("FixedSize"));
				if (fs) {
					size *= *((size_t*)fs->begin());
				}
				
			}
			else {
				if (types[index].type == Types::baseTypes::Struct) {
					auto sch = types[index].constraints.Contains(Types::MakeKey("Schema"));
					if (sch) {
						size = ((Schema*)sch->begin())->SizeOfFixed();

						auto dims = types[index].constraints.Contains(Types::MakeKey("dim"));
						if (dims) {
							size_t dimCount = dims->Size() / sizeof(size_t);
							for (size_t i = 0; i < dimCount; i++)
								size *= ((size_t*)dims->begin())[i];
						}
						auto fs = types[index].constraints.Contains(Types::MakeKey("FixedSize"));
						if (fs) {
							size *= *((size_t*)fs->begin());
						}
					}
				}
			}
			return size;
		}


		inline size_t Schema::BlockCount(uint8_t BlockIndex) const
		{
			switch (BlockIndex)
			{
			case 0: return BlockCount(Types::TypeSequence::Nullable, Types::TypeSequence::Sparse);
			case 1: return BlockCount(Types::TypeSequence::Sparse, Types::TypeSequence::Bool);
			case 2: return BlockCount(Types::TypeSequence::Bool, Types::TypeSequence::Fixed);
			case 3: return BlockCount(Types::TypeSequence::Fixed, Types::TypeSequence::Columnar);	
			case 4: return BlockCount(Types::TypeSequence::Columnar, Types::TypeSequence::Multi);
			case 5: return BlockCount(Types::TypeSequence::Multi, Types::TypeSequence::Var);
			case 6: return BlockCount(Types::TypeSequence::Var, Types::TypeSequence::MultiVar);
			case 7: return BlockCount(Types::TypeSequence::MultiVar, Types::TypeSequence::Uncached);
			case 8: return BlockCount(Types::TypeSequence::Uncached, (uint8_t)Sequence.Size());
			default:
				return 0;
			}
		}

		inline size_t Schema::BlockCount(Types::TypeSequence start, uint8_t end) const
		{
			size_t count = 0;
			for (size_t i = start; i < end; i++) {
				count += Sequence[i];
			}
			return count;
		}

		inline size_t Schema::FindIndex(const KeyString& name) const
		{
			//check name
			for (size_t i = 0; i < labels.Size(); i++) {
				const KeyString& c = labels[i];
				const uint8_t* p = c.Data();
				if (c == name)
					return i;
			}
			return std::numeric_limits<size_t>::max();
		}

		inline size_t Schema::FindIndex(const char* name) const
		{
			return FindIndex(MakeKey(name));
		}

		inline bool Schema::AddAttribute(const KeyString& name, Types::TypeDescr type)
		{
			//check name
			if (FindIndex(name) < labels.Size())
				return false;//attribute name already used

		//insert Sort by Seq Number
			Types::TypeSequence seq = type.getTypeSequence();
			if (seq < Types::TypeSequence::Multi) //recalculate size
				fixedSize = std::numeric_limits<size_t>::max();

			size_t seqStart = SequenceStart(seq);
			size_t seqEnd = seqStart + Sequence[seq];
			//sort by name
			for (uint64_t i = seqStart; i < seqEnd; i++)
				if (labels[seqStart] < name)
					seqStart++;
				else
					break;

			//insert
			labels.Insert(seqStart, name);
			types.Insert(seqStart, type);
			Sequence[seq]++;

			if (seq < Types::TypeSequence::Multi) //offsets changed
				RebuildOffsets();

			return true;
		}

		inline bool Schema::AddAttribute(const char* name, Types::baseTypes type)
		{
			Types::TypeDescr t;
			t.type = type;
			return AddAttribute(Types::MakeKey(name), t);
		}

		inline uint8_t* Schema::GetValuePointer(uint64_t columnIndex, iBlockD& MainBlock) const
		{
			Types::TypeDescr td = GetTypeDescr(columnIndex);
			Types::TypeSequence ts = td.getTypeSequence();
			if (ts < Types::TypeSequence::Columnar) { // is in mainblock	
				if (ts < Types::TypeSequence::Sparse) {//nullable
					//check null flags
					BitItr<uint8_t, 1> itr(MainBlock.Data(), 0);
					if (!itr[columnIndex]) 
						return nullptr;
					return MainBlock.Data(offsets[columnIndex]);
				}
				if (ts < Types::TypeSequence::Bool) {//Sparse
					//check sparse flag
					BitItr<uint8_t, 1> itr(MainBlock.Data(), 0);
					if (!itr[columnIndex]) return nullptr;
					//calculate sparse offset					
					uint64_t offset = SizeOfFixed();//start at end of fixed
					for (uint64_t i = BlockCount(0); i < columnIndex; i++) //for each sparse 
						if (itr[i]) //if exists
							offset += offsets[i]; //add its offset
					return MainBlock.Data(offset);					
				}
				if (ts < Types::TypeSequence::Fixed) {//TODO::Bool
					return nullptr; //use get BitPointer
				}
				if (ts < Types::TypeSequence::Columnar) { //fixed
					return MainBlock.Data(offsets[columnIndex]);
				}
			}
			return nullptr;
		}

		inline BitItr<uint8_t, 1> Schema::GetBoolPointer(uint64_t columnIndex, iBlockD& MainBlock) const
		{
			o_assert_dbg(types[columnIndex].type == baseTypes::boolean);

			BitItr<uint8_t, 1> bit(MainBlock.Data());
			bit += GetOffset(columnIndex);

			return bit;
		}

		inline uint8_t* Schema::SetColumnValue(uint64_t columnIndex, iBlockD& MainBlock) const
		{
			o_assert_dbg(GetTypeDescr(columnIndex).Nullable() || GetTypeDescr(columnIndex).Sparse());

			Types::BitItr<uint8_t, 1> flag(MainBlock.Data());
			flag += columnIndex;
			if (GetTypeDescr(columnIndex).Sparse() && !flag[0]) {//need to insert the value into buffer
				flag[0] = true;
				BitItr<uint8_t, 1> itr(MainBlock.Data(), 0);
				uint64_t offset = SizeOfFixed();//start at end of fixed
				for (uint64_t i = BlockCount(0); i < columnIndex; i++) //for each sparse 
					if (itr[i]) //if exists
						offset += offsets[i]; //add its offset

				//insert space for value
				MainBlock.AddInsert(offset, offsets[columnIndex]); //invalidates bitItrs
			} else
				flag[0] = true;
					   
			return GetValuePointer(columnIndex, MainBlock);
		}

		inline void Schema::SetColumnNull(uint64_t columnIndex, iBlockD& MainBlock) const
		{
			o_assert_dbg(GetTypeDescr(columnIndex).Nullable() || GetTypeDescr(columnIndex).Sparse());

			Types::BitItr<uint8_t, 1> flag(MainBlock.Data());
			flag += columnIndex;
			if (GetTypeDescr(columnIndex).Sparse() && flag[0]) {//need to remove the value from buffer
				flag[0] = false;
				BitItr<uint8_t, 1> itr(MainBlock.Data(), 0);
				uint64_t offset = SizeOfFixed();//start at end of fixed
				for (uint64_t i = BlockCount(0); i < columnIndex; i++) //for each sparse 
					if (itr[i]) //if exists
						offset += offsets[i]; //add its offset

				//insert space for value
				MainBlock.Remove(offset, offsets[columnIndex]); //invalidates bitItrs
			}
			else
				flag[0] = false;			
		}

		inline void Schema::RebuildOffsets()
		{
			offsets.Clear();			
			offsets.AddBack(SequenceStart(TypeSequence::Uncached));

			uint64_t offset = BlockCount(0) + BlockCount(1) + SizeOfBools();
			offset = offset / 8 + (offset % 8 ? 1 : 0);

			uint64_t index = 0;
			//Write Null Offsets
			uint64_t Count = BlockCount(0);
			for (index; index < Count; ++index) {
				offsets[index] = offset;
				offset += GetElmSize(index);
			}
			//Write Default sparse FLags
			Count += BlockCount(1);
			for (index; index < Count; ++index) {
				offsets[index] = GetElmSize(index);
			}
			//Write Default bool FLags
			Count += BlockCount(2);
			uint64_t boffset = BlockCount(0) + BlockCount(1);
			for (index; index < Count; ++index) {
				auto argp = types[index].constraints.Contains(MakeKey("FixedSize"));
				uint64_t bits = 1;

				if (argp) //get bit count
					bits = *((uint64_t*)(argp->begin()));

				offsets[index] = boffset;
				boffset += bits;
			}

			Count += BlockCount(3);
			for (index; index < Count; ++index) {
				offsets[index] = offset;
				offset += GetElmSize(index);
			}

			//colums, 
			offset = 0;
			uint64_t multivars = SequenceStart(TypeSequence::MultiVar);
			for (index; index < multivars; ++index) {
				offsets[index] = ++offset; //offsets start at 1, 0 reserved for mainbuffer
			}

			//multiVar Offsets, 
			offset = 0;			
			for (index; index < offsets.Size(); ++index) {
				offsets[index] = offset++;
			}

		}

		inline size_t Schema::SequenceStart(TypeSequence seq) const
		{
			size_t seqStart = 0;
			for (size_t i = 0; i < seq; i++) {
				seqStart += Sequence[i];
			}
			return seqStart;
		}

		inline void Schema::init()
		{
			Sequence.ReserveBack(Types::TypeSequence::END);
			for (uint8_t i = 0; i < Types::TypeSequence::END;i++)
				Sequence.PushBack(0);
		}

	};// namspace Types
}; //Namespace ContainersPP

#endif // _SCHEMA_DEF