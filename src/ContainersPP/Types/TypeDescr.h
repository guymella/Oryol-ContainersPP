//------------------------------------------------------------------------------
//  TypeDescr.h
//------------------------------------------------------------------------------

#include "Types.h"
#include "KeyString.h"

#ifndef Included_TypeDescr_H
#define Included_TypeDescr_H

namespace ContainersPP {
	namespace Types {
		enum TypeSequence : uint8_t {
			Nullable,
			CachedNullable,
			NullableFixedMulti,
			CachedNullableFixedMulti,

			NullableStruct,
			CachedNullableStruct,
			NullableFixedMultiStruct,
			CachedNullableFixedMultiStruct,

			Sparse,
			CachedSparse,
			SparseFixedMulti,
			CachedSparseFixedMulti,

			SparseStruct,
			CachedSparseStruct,
			SparseFixedMultiStruct,
			CachedSparseFixedMultiStruct,

			Bool,
			CachedBool,
			FixedMultiBool,
			CachedFixedMultiBool,

			Fixed,
			Cached,
			FixedMulti,
			CachedFixedMulti,

			StructInline,
			CachedStruct,
			FixedMultiStruct,
			CachedFixedMultiStruct,

			Columnar,
			CachedColumnar,

			Multi,
			CachedMulti,
			MultiStruct,
			CachedMultiStruct,

			Var,
			CachedVar,
			NullableVar,
			CachedNullableVar,

			MultiVar,
			CachedMultiVar,
			NullableMultiVar,
			CachedNullableMultiVar,

			Uncached,
			UncachedMulti,
			UncachedStruct,
			UncachedMultiStruct,
			UcachedVar,
			UncachedMultVar,

			UncachedNullable,
			UncachedNullableMulti,
			UncachedNullableSchema,
			UncachedNullableMultiSchema,
			UncachedNullableVar,
			UncachedNullableMultiVar,

			END
		};

		struct Constraint {
			KeyString name;
			TypeVector<uint8_t> args;
		};

		class Constraints {
		public:
			bool AddConstraint(Constraint&& constraint);
			iTypeVector<uint8_t>* Contains(const KeyString& key) const;
			bool operator==(const Constraint& rhs) const { return true; };
		private:
			TypeVector<Constraint> constraints;
		};

		inline bool Constraints::AddConstraint(Constraint&& constraint)
		{
			if (Contains(constraint.name))
				return false;

			constraints.PushBack(std::move(constraint));
			return true;
		}

		iTypeVector<uint8_t>* Constraints::Contains(const KeyString& key) const
		{
			for (size_t i = 0; i < constraints.Size(); i++)
				if (constraints[i].name == key)
					return (iTypeVector<uint8_t>*)&(constraints[i].args);
			return nullptr;
		}

		struct TypeDescr {
			baseTypes type = baseTypes::Void;
			uint8_t flags = 0;
			Constraints constraints;
			bool operator==(const TypeDescr& rhs) const;
			bool Nullable() const { return (flags) & 1U; };
			bool Multiple() const { return (flags >> 1) & 1U; };
			bool Sparse() const { return (flags >> 2) & 1U; };
			bool Columnar() const { return (flags >> 3) & 1U; };
			bool Constrained() const { return (flags >> 4) & 1U; };
			bool Derived() const { return (flags >> 5) & 1U; };
			bool Cached() const { return (flags >> 6) & 1U; };
			bool Subscribable() const { return (flags >> 7) & 1U; };
			void Nullable(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL); };
			void Multiple(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 1); };
			void Sparse(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 2); };
			void Columnar(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 3); };
			void Constrained(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 4); };
			void Derived(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 5); };
			void Cached(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 6); };
			void Subscribable(bool flag) { flags ^= (-(uint8_t)flag ^ flags) & (1UL << 7); };

			static uint8_t setFLags(bool nullable, bool Multiple, bool Sparse, bool Constrained, bool Derived, bool Cached, bool Subscribable);
			TypeSequence getTypeSequence() const;
		};

		inline bool TypeDescr::operator==(const TypeDescr& rhs) const
		{
			return (type == rhs.type && flags == rhs.flags);
		}

		uint8_t TypeDescr::setFLags(bool nullable, bool Multiple, bool Sparse, bool Constrained, bool Derived, bool Cached, bool Subscribable)
		{
			return ((uint8_t)nullable)
				| ((uint8_t)Multiple << 1)
				| ((uint8_t)Sparse << 2)
				| ((uint8_t)Constrained << 4)
				| ((uint8_t)Derived << 5)
				| ((uint8_t)Cached << 6)
				| ((uint8_t)Multiple << 7);
		}

		inline TypeSequence TypeDescr::getTypeSequence() const
		{
			if (!Derived() || Cached()) {
				if ((!Multiple() && (type <= baseTypes::Struct)) || constraints.Contains(MakeKey("FixedSize"))) {
					if (!Nullable() && !Sparse()) {
						if ((type == baseTypes::Struct || type == baseTypes::VarStruct)) {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedStruct : TypeSequence::StructInline;
							else
								return Cached() ? TypeSequence::CachedFixedMultiStruct : TypeSequence::FixedMultiStruct;
						}
						else if (type == baseTypes::boolean) {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedBool : TypeSequence::Bool;
							else
								return Cached() ? TypeSequence::CachedFixedMultiBool : TypeSequence::FixedMultiBool;
						}
						else { //struct
							if (!Multiple())
								return Cached() ? TypeSequence::Cached : TypeSequence::Fixed;
							else
								return Cached() ? TypeSequence::CachedFixedMulti : TypeSequence::FixedMulti;

						}
					}
					else if (Nullable() && !Sparse()) {
						if (!(type == baseTypes::Struct || type == baseTypes::VarStruct)) {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedNullable : TypeSequence::Nullable;
							else
								return Cached() ? TypeSequence::CachedNullableFixedMulti : TypeSequence::NullableFixedMulti;
						}
						else { //struct
							if (!Multiple())
								return Cached() ? TypeSequence::CachedNullableStruct : TypeSequence::NullableStruct;
							else
								return Cached() ? TypeSequence::CachedNullableFixedMultiStruct : TypeSequence::NullableFixedMultiStruct;
						}
					}
					else { //Sparce
						if (!(type == baseTypes::Struct || type == baseTypes::VarStruct)) {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedSparse : TypeSequence::Sparse;
							else
								return Cached() ? TypeSequence::CachedSparseFixedMulti : TypeSequence::SparseFixedMulti;
						}
						else { //struct
							if (!Multiple())
								return Cached() ? TypeSequence::CachedSparseStruct : TypeSequence::SparseStruct;
							else
								return Cached() ? TypeSequence::CachedSparseFixedMultiStruct : TypeSequence::SparseFixedMultiStruct;
						}
					}
				}
				else { //Not Fixed Size
					if (type > baseTypes::Struct || type <= baseTypes::LRef) { //var
						if (!Nullable()) {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedVar : TypeSequence::Var;
							else
								return Cached() ? TypeSequence::CachedMultiVar : TypeSequence::MultiVar;
						}
						else {
							if (!Multiple())
								return Cached() ? TypeSequence::CachedNullableVar : TypeSequence::NullableVar;
							else
								return Cached() ? TypeSequence::CachedNullableMultiVar : TypeSequence::NullableMultiVar;
						}
					}
					else {
						if (!(type == baseTypes::Struct || type == baseTypes::VarStruct))
							return Cached() ? TypeSequence::CachedMulti : TypeSequence::Multi;
						else
							return Cached() ? TypeSequence::CachedMultiStruct : TypeSequence::MultiStruct;
					}
				}
			}
			else {//unCached
				if (!Nullable()) {
					if (type == Struct || type == VarStruct)
						return Multiple() ? TypeSequence::UncachedMultiStruct : TypeSequence::UncachedStruct;
					else if (type > baseTypes::Struct || type <= baseTypes::LRef)
						return Multiple() ? TypeSequence::UncachedMultVar : TypeSequence::UcachedVar;
					else
						return Multiple() ? TypeSequence::UncachedMulti : TypeSequence::Uncached;
				}
				else {
					if (type == Struct || type == VarStruct)
						return Multiple() ? TypeSequence::UncachedNullableMultiSchema : TypeSequence::UncachedNullableSchema;
					else if (type > baseTypes::Struct || type <= baseTypes::LRef)
						return Multiple() ? TypeSequence::UncachedNullableMultiVar : TypeSequence::UncachedNullableVar;
					else
						return Multiple() ? TypeSequence::UncachedNullableMulti : TypeSequence::UncachedNullable;
				}
			}
			return TypeSequence::Uncached;
		}
	};
};
#endif 