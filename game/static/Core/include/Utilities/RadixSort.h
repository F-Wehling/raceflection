#pragma once

BEGINNAMESPACE

template<typename IntegralType, IntegralType shift = 24>
class Radix {
	typedef IntegralType Type;
public:
	static void Sort(Type* begin, Type* end) {

	}
	static void Sort(Type* begin, Type* end, Type* begin1) {
		_Sort_msb(begin, end, begin1, shift);
	}
private:
	static void _Sort_msb(Type* begin, Type* end, Type* begin1, Type shift) {
		Type *end1 = begin1 + (end - begin);
		size_t count[0x100] = {};

		for (Type *p = begin; p != end; p++)
			count[(*p >> shift) & 0xFF]++;

		Type *bucket[0x100], *obucket[0x100], *q = begin1;

		for (int32 i = 0; i < 0x100; q += count[i++])
			obucket[i] = bucket[i] = q;

		for (Type *p = begin; p != end; p++)
			*bucket[(*p >> shift) & 0xFF]++ = *p;

		for (int32 i = 0; i < 0x100; ++i)
			_Sort_lsb(obucket[i], bucket[i], begin + (obucket[i] - begin1), shift - 8);
	}

	static void _Sort_lsb(Type* begin, Type* end, Type* begin1, Type maxshift) {
		Type *end1 = begin1 + (end - begin);
		
		for (Type shift = 0; shift <= maxshift; shift += 8){			
			size_t count[0x100] = {};

			for (Type *p = begin; p != end; p++)
				count[(*p >> shift) & 0xFF]++;

			Type *bucket[0x100], *q = begin1;

			for (int32 i = 0; i < 0x100; q += count[i++])
				bucket[i] = q;

			for (Type *p = begin; p != end; p++)
				*bucket[(*p >> shift) & 0xFF]++ = *p;

			std::swap(begin, begin1);
			std::swap(end, end1);
		}
	}
};

ENDNAMESPACE