#pragma once

#include <CompileTime/Templates.h>

BEGINNAMESPACE

namespace detail {

	template<size_type N, typename Size_t>
	struct Size_Typedefs {
		typedef Size_t value_type;
		static const size_type Size = N;
	};

	template<size_type N, typename Size_t>
	struct Size_Name : public Size_Typedefs<N, Size_t> {
        typedef typename Size_Typedefs<N, Size_t>::value_type value_type;
        static const size_type Size = Size_Typedefs<N, Size_t>::Size;
		union {
            PlainArray<value_type, Size> data;
		};
	};

	template<typename Size_t>
	struct Size_Name<1, Size_t> : public Size_Typedefs<1,Size_t> {
        typedef typename Size_Typedefs<2, Size_t>::value_type value_type;
        static const size_type Size = Size_Typedefs<2, Size_t>::Size;

        Size_Name(value_type width = 0) { this->width = width; }
		union {
            PlainArray<value_type, Size> data;
			struct { value_type width; };
		};
	};

	template<typename Size_t>
	struct Size_Name<2, Size_t> : public Size_Typedefs<2, Size_t> {
        typedef typename Size_Typedefs<2, Size_t>::value_type value_type;
        static const size_type Size = Size_Typedefs<2, Size_t>::Size;
		Size_Name(value_type width = 0, value_type height = 0) { this->width = width; this->height = height; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type width, height; };
		};
	};

	template<typename Size_t>
	struct Size_Name<3, Size_t> : public Size_Typedefs<3, Size_t> {
        typedef typename Size_Typedefs<3, Size_t>::value_type value_type;
        static const size_type Size = Size_Typedefs<3, Size_t>::Size;
		Size_Name(value_type width = 0, value_type height = 0, value_type depth = 0) { this->width = width; this->height = height; this->depth = depth; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type width, height, depth; };
		};
	};

	template<typename Size_t>
	struct Size_Name<4, Size_t> : public Size_Typedefs<4, Size_t> {
        typedef typename Size_Typedefs<4, Size_t>::value_type value_type;
        static const size_type Size = Size_Typedefs<4, Size_t>::Size;
		Size_Name(value_type width = 0, value_type height = 0, value_type depth = 0, value_type layer = 0) { this->width = width; this->height = height; this->depth = depth; this->layer = layer; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type width, height, depth, layer; };
		};
	};
}

template<size_type N, typename Size_t = size_type>
struct Size : public detail::Size_Name<N,Size_t> {

    typedef typename detail::Size_Name<N,Size_t>::value_type value_type;
	//default constructor
	Size() : detail::Size_Name<N,Size_t>(){}

	//using Size_Name constructor
	using detail::Size_Name<N, Size_t>::Size_Name;

	//from raw pointer
	Size(const value_type* in) {
        Repeat<0, N>::copy(detail::Size_Name<N,Size_t>::data, in);
	}

	//from other Size
	template<size_type OtherN, typename OtherSize_t>
	Size(const Size<OtherN, OtherSize_t>& other) {
		*this = other;
	}

	//operator =
	template<size_type OtherN, typename OtherSize_t>
	Size& operator = (const Size<OtherN, OtherSize_t>& rhs) {
        Repeat<0, Min<value_type, N, OtherN>::min>::copy(detail::Size_Name<N,Size_t>::data, rhs.data);
		return *this;
	}

	template<size_type OtherN, typename OtherSize_t>
	inline bool operator > (const Size<OtherN, OtherSize_t>& val) const {
        return Repeat<0, Min<value_type, N, OtherN>::min>::all_greater(detail::Size_Name<N,Size_t>::data, val.data);
	}

	template<typename OtherSize_t>
	inline bool operator > (OtherSize_t val) const {
        return Repeat<0, N>::all_greater(detail::Size_Name<N,Size_t>::data, val);
	}

	template<size_type OtherN, typename OtherSize_t>
	inline bool operator < (const Size<OtherN, OtherSize_t>& val) const {
        return Repeat<0, Min<value_type, N, OtherN>::min>::all_less(detail::Size_Name<N,Size_t>::data, val.data);
	}

	template<typename OtherSize_t>
	inline bool operator < (OtherSize_t val) const {
        return Repeat<0, N>::all_less(detail::Size_Name<N,Size_t>::data, val);
	}
};

typedef Size<1, size_type> Size1;
typedef Size<2, size_type> Size2;
typedef Size<3, size_type> Size3;
typedef Size<4, size_type> Size4;

typedef Size<1, int32> Size1i;
typedef Size<2, int32> Size2i;
typedef Size<3, int32> Size3i;
typedef Size<4, int32> Size4i;

typedef Size<1, uint32> Size1u;
typedef Size<2, uint32> Size2u;
typedef Size<3, uint32> Size3u;
typedef Size<4, uint32> Size4u;

typedef Size<1, float32> Size1f;
typedef Size<2, float32> Size2f;
typedef Size<3, float32> Size3f;
typedef Size<4, float32> Size4f;

ENDNAMESPACE
