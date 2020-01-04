//------------------------------------------------------------------------------
//  KeyString.h
//------------------------------------------------------------------------------

#ifndef Included_Keys_H
#define Included_Keys_H



#include "Types.h"
#include "ContainersPP/Containers/TypeBuffer.h"
#include <cstring>

namespace ContainersPP {
	namespace Types {


		typedef TypeVector<uint8_t> KeyString;

		KeyString MakeKey(const char* key)
		{
			KeyString newKey;
			newKey.CopyBack((const uint8_t*)key, std::strlen(key));
			return newKey;
		}

		bool operator==(const KeyString& lhs, const KeyString& rhs) {
			if (lhs.Size() != rhs.Size())
				return false;
			return !std::memcmp(lhs.begin(), rhs.begin(), lhs.Size());
		}

		bool operator<(const KeyString& lhs, const KeyString& rhs) {
			if (lhs.Size() != rhs.Size())
				return false;
			return std::memcmp(lhs.begin(), rhs.begin(), lhs.Size()) < 0;
		}

		struct KeyCompare {
			KeyCompare(const KeyString& lhs, const KeyString& rhs);
			KeyCompare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize);
			void Compare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize);
			size_t commonPrefix = 0;
			size_t lhsPostfix = 0;
			size_t rhsPostfix = 0;
			bool lhsLesser = false;
			bool Equal() { return !(lhsPostfix || rhsPostfix); }

		};
		inline KeyCompare::KeyCompare(const KeyString& lhs, const KeyString& rhs)
		{
			Compare(lhs.begin(), rhs.begin(), lhs.Size(), rhs.Size());
		}
		KeyCompare::KeyCompare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize)
		{
			Compare(lhs, rhs, lhsSize, rhsSize);
		}

		inline void KeyCompare::Compare(const uint8_t* lhs, const uint8_t* rhs, size_t lhsSize, size_t rhsSize)
		{
			size_t compSize = (lhsSize < rhsSize) ? lhsSize : rhsSize;

			while (*lhs == *rhs && commonPrefix < compSize)
				commonPrefix++, lhs++, rhs++;

			lhsPostfix = lhsSize - commonPrefix;
			rhsPostfix = rhsSize - commonPrefix;
			lhsLesser = *(lhs) < *(rhs);
		}

	};

};




//
//class iKeyString {
//public:
//	virtual size_t Size() const { return StringData().Size() - Tare(); };
//	virtual bool operator==(const iKeyString& rhs) const;
//	virtual bool operator<(const iKeyString& rhs) const;
//	virtual bool operator>(const iKeyString& rhs) const;
//	virtual bool operator<=(const iKeyString& rhs) const;
//	virtual bool operator>=(const iKeyString& rhs) const;
//	virtual keyCompare CompareTo(const iKeyString& rhs) const;
//	virtual size_t& Tare(size_t tareLen = 0) = 0;
//	virtual size_t Tare() const=0;
//	friend class KeyString;
//	virtual iArray<uint8_t>& StringData() = 0;
//	virtual const iArray<uint8_t>& StringData() const = 0;
//protected:
//
//};
//
//class iKeyRange{
//	virtual bool operator==(const iKeyString& rhs) const = 0;
//	virtual keyCompare CompareTo(const iKeyString& rhs) const = 0;
//	virtual iKeyRange& Union(iKeyRange& rhs) = 0;
//	virtual iKeyRange& Intersect(iKeyRange& rhs) = 0;
//	//virtual iKeyRange& Subtract(iKeyRange& rhs) = 0;
//};
//
//
//
//class KeyString : public iKeyString {	
//public:
//	KeyString() {};
//	KeyString(const iKeyString& rhs);
//	KeyString(const char* str);
//	virtual size_t& Tare(size_t tareLen = 0) override;
//	virtual size_t Tare() const override { return tare; };
//	virtual size_t Truncate(size_t NewLen);
//	virtual iArray<uint8_t>& StringData() override { return key; };
//	virtual const iArray<uint8_t>& StringData() const override { return key; };
//
//private:
//	
//	Array<uint8_t> key;
//	size_t tare =0;
//};
//
//class KeyStringRef : public iKeyString {
//public:
//	virtual iArray<uint8_t>& StringData() override {return key;};
//	virtual const iArray<uint8_t>& StringData() const override { return key; };
//private:
//	Slice<uint8_t> key;
//};
//
//bool iKeyString::operator==(const iKeyString& rhs) const
//{
//	keyCompare c = CompareTo(rhs);
//	return !(c.Postfix0 || c.Postfix1);
//}
//
//inline bool iKeyString::operator<(const iKeyString& rhs) const
//{
//	return CompareTo(rhs).LessThan;
//}
//
//inline bool iKeyString::operator>(const iKeyString& rhs) const
//{
//	return !((*this)<=rhs);
//}
//
//inline bool iKeyString::operator<=(const iKeyString& rhs) const
//{
//	keyCompare c = CompareTo(rhs);
//	return c.LessThan || c.Equal();
//}
//
//inline bool iKeyString::operator>=(const iKeyString& rhs) const
//{
//	return !((*this) < rhs);
//}
//
//inline keyCompare iKeyString::CompareTo(const iKeyString& rhs) const
//{
//	keyCompare c;
//	const uint8_t* l = StringData().begin(Tare());
//	const uint8_t* r = rhs.StringData().begin(rhs.Tare());
//	size_t s = (rhs.Size() < Size()) ? rhs.Size() : Size();
//	if (s)
//		while (l[c.commonPrefix] == r[c.commonPrefix])
//			if (++c.commonPrefix == s) break;
//	c.Postfix0 = Size() - c.commonPrefix;
//	c.Postfix1 = rhs.Size() - c.commonPrefix;
//	c.LessThan = (l[c.commonPrefix] < r[c.commonPrefix]);
//	return c;
//}
//
//inline KeyString::KeyString(const iKeyString& rhs)
//{
//	const uint8_t* b = rhs.StringData().begin().Ptr();
//
//	//TODO:: optimize
//	for (size_t i = rhs.Tare(); i < rhs.StringData().Size(); i++)
//		key.PushBack(b[i]);
//}
//
//inline KeyString::KeyString(const char* str)
//{
//	key.Clear();
//	size_t len = std::strlen(str);
//	if (len) {
//		key.insertBlank(0, len);
//		std::memcpy(key.begin().Ptr(), str, len);
//	}
//}
//
//size_t& KeyString::Tare(size_t tareLen)
//{
//	tare += tareLen;
//	return tare;
//}
//
//inline size_t KeyString::Truncate(size_t NewLen)
//{
//	key.EraseRange(NewLen);
//	return key.Size();
//}

#endif //Keys