#pragma once

BEGINNAMESPACE

template<size_type IdxBitCount, size_type GenBitCount>
struct Handle {
	static const size_type HandleNumBits = NextPowerOf2<size_type, IdxBitCount + GenBitCount>::value;
	static const size_type Size = HandleNumBits / 8;
	typedef typename IntegerOfSize< Size >::u_type _Handle_type;
	
	static const size_type IndexBitCount = 
		HandleNumBits == (IdxBitCount + GenBitCount) ? IdxBitCount : ((IdxBitCount * HandleNumBits) / (IdxBitCount + GenBitCount) + (IdxBitCount < GenBitCount ? 1 : 0));
	static const size_type GenerationBitCount = 
		HandleNumBits == (IdxBitCount + GenBitCount) ? GenBitCount : ((GenBitCount * HandleNumBits) / (IdxBitCount + GenBitCount) + (GenBitCount <  IdxBitCount ? 1 : 0));

	static_assert(IndexBitCount + GenerationBitCount == HandleNumBits, "The used handle bits should sum up to the unterlying _Handle_type size");
	_Handle_type index : IndexBitCount;
	_Handle_type generation : GenerationBitCount;
};

ENDNAMESPACE