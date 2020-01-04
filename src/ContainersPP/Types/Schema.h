//------------------------------------------------------------------------------
//  Schema.h
//------------------------------------------------------------------------------

#include "TypeDescr.h"

//#include "Structures/Containers/SparseArray.h"

namespace ContainersPP {
	namespace Types {

		class Schema {
		public:
			Schema() { init(); };
			size_t SizeOfFixed();
			size_t SizeOfFixed() const;
			size_t SizeOfBools();
			size_t SizeOfBools() const;
			size_t GetElmSize(size_t index) const;
			size_t BlockCount(uint8_t BlockIndex) const; //returns number of elements in the sequence block
			size_t BlockCount(Types::TypeSequence start, uint8_t end) const; //returns number of elements in the sequence block
			size_t FindIndex(const KeyString& name) const;
			size_t FindIndex(const char* name) const;
			bool AddAttribute(const KeyString& name, Types::TypeDescr type);
			bool AddAttribute(const char* name, Types::baseTypes type);
		//private:
			size_t SequenceStart(TypeSequence seq) const;
			void init();
			size_t fixedSize = 0;
			bool extendable = false;
			TypeVector<size_t> Sequence;
			TypeVector<KeyString> labels;
			TypeVector<Types::TypeDescr> types;
			//SparseArray<Darivation> Darivations
			//TypeVector<Types::Constraint> Constraints;
			//SparseArray<Schema*> SubSchemas;
		};





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
			for (size_t i = Sequence[TypeSequence::Nullable]; i < secEnd; i++)//null cols
				Size += GetElmSize(i);

			return Size;
		}

		inline size_t Schema::SizeOfBools()
		{
			size_t bools = BlockCount(0);
			size_t size = bools;
			for (size_t i = 0; i < bools; i++) {
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
			size_t bools = BlockCount(0);
			size_t size = bools;
			for (size_t i = 0; i < bools; i++) {
				auto argp = types[i].constraints.Contains(MakeKey("FixedSize"));
				if (argp) {
					size--;
					size += *((size_t*)(argp->begin()));
				}				
			}
			return size;
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
				const uint8_t* p = c.begin();
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

			return true;
		}

		inline bool Schema::AddAttribute(const char* name, Types::baseTypes type)
		{
			Types::TypeDescr t;
			t.type = type;
			return AddAttribute(Types::MakeKey(name), t);
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
