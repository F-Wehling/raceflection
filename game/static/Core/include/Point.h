#pragma once

#include <CompileTime/Templates.h>

BEGINNAMESPACE

namespace detail {

	template<size_type N, typename Point_t>
	struct Point_Typedefs {
		typedef Point_t value_type;
		static const size_type Size = N;
	};

	template<size_type N, typename Point_t>
	struct Point_Name : public Point_Typedefs<N, Point_t> {
        typedef typename Point_Typedefs<N, Point_t>::value_type value_type;
        static const size_type Size = Point_Typedefs<N, Point_t>::Size;
		union {
			PlainArray<value_type, Size> data;
		};
	};

	template<typename Point_t>
	struct Point_Name<1, Point_t> : public Point_Typedefs<1, Point_t> {
        typedef typename Point_Typedefs<1, Point_t>::value_type value_type;
        static const size_type Size = Point_Typedefs<1, Point_t>::Size;
		Point_Name(value_type x = 0) { this->x = x; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type x; };
		};
	};

	template<typename Point_t>
	struct Point_Name<2, Point_t> : public Point_Typedefs<2, Point_t> {
        typedef typename Point_Typedefs<2, Point_t>::value_type value_type;
        static const size_type Size = Point_Typedefs<2, Point_t>::Size;
		Point_Name(value_type x = 0, value_type y = 0) { this->x = x; this->y = y; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type x, y; };
		};
	};

	template<typename Point_t>
	struct Point_Name<3, Point_t> : public Point_Typedefs<3, Point_t> {
        typedef typename Point_Typedefs<3, Point_t>::value_type value_type;
        static const size_type Size = Point_Typedefs<3, Point_t>::Size;
		Point_Name(value_type x = 0, size_type y = 0, size_type z = 0) { this->x = x; this->y = y; this->z = z; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type x, y, z; };
		};
	};

	template<typename Point_t>
	struct Point_Name<4, Point_t> : public Point_Typedefs<4, Point_t> {
        typedef typename Point_Typedefs<4, Point_t>::value_type value_type;
        static const size_type Size = Point_Typedefs<4, Point_t>::Size;
		Point_Name(value_type x = 0, size_type y = 0, size_type z = 0, size_type w = 0) { this->x = x; this->y = y; this->z = z; this->w = w; }
		union {
			PlainArray<value_type, Size> data;
			struct { value_type x, y, z, w; };
		};
	};
}

template<size_type N, typename Point_t = float32>
struct Point : public detail::Point_Name<N, Point_t> {
	typedef typename detail::Point_Typedefs<N, Point_t>::value_type value_type;

	//default constructor
	Point() : detail::Point_Name<N,Point_t>() {}

	//using Size_Name constructor
	using detail::Point_Name<N, Point_t>::Point_Name;

	//from raw pointer
	Point(const value_type* in) {
        Repeat<0, N>::copy(detail::Point_Name<N, Point_t>::data, in);
	}
	
	//from other Size
	template<size_type OtherN, typename OtherPoint_t>
	Point(const Point<OtherN, OtherPoint_t>& other) {
		*this = other;
	}

	//operator =
	template<size_type OtherN, typename OtherPoint_t>
	Point& operator = (const Point<OtherN, OtherPoint_t>& rhs) {
		if (this != &rhs) {
            Repeat<0, Min<value_type, N, OtherN>::min>::copy(detail::Point_Name<N, Point_t>::data, rhs.data);
		}
		return *this;
	}
};

typedef Point<1, float32> Point1f;
typedef Point<2, float32> Point2f;
typedef Point<3, float32> Point3f;
typedef Point<4, float32> Point4f;

typedef Point<1, int32> Point1i;
typedef Point<2, int32> Point2i;
typedef Point<3, int32> Point3i;
typedef Point<4, int32> Point4i;

typedef Point<1, uint32> Point1ui;
typedef Point<2, uint32> Point2ui;
typedef Point<3, uint32> Point3ui;
typedef Point<4, uint32> Point4ui;

ENDNAMESPACE
