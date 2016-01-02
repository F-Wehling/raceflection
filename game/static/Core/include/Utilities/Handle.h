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

#define INVALID_HANDLE_DECL(handle_type) extern handle_type Invalid ## handle_type;
#define INVALID_HANDLE_IMPL(handle_type) handle_type Invalid ## handle_type = { \
                                            handle_type::_Handle_type(-1), \
                                            handle_type::_Handle_type(-1) \
                                        }

template<size_type IdxBitCount, size_type GenBitCount>
inline bool operator == (const Handle<IdxBitCount, GenBitCount>& lhs, const Handle<IdxBitCount, GenBitCount>& rhs) {
	return lhs.index == rhs.index && lhs.generation == rhs.generation;
}

template<size_type IdxBitCount, size_type GenBitCount>
inline bool operator != (const Handle<IdxBitCount, GenBitCount>& lhs, const Handle<IdxBitCount, GenBitCount>& rhs) {
	return lhs.index != rhs.index && lhs.generation != rhs.generation;
}

ENDNAMESPACE
