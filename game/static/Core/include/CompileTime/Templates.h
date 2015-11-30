#pragma once

#include <Utilities/Assert.h>

BEGINNAMESPACE

template<typename T, typename T1, typename T2>
constexpr T min(const T1& a, const T2& b) {
	return a < b ? static_cast<T>(a) : static_cast<T>(b);
}

template<typename T, typename T1, typename T2>
constexpr T max(const T1& a, const T2& b) {
	return a > b ? static_cast<T>(a) : static_cast<T>(b);
}

template<typename T>
constexpr T& min(T& a, T& b){
	return a < b ? a : b;
}

template<typename T>
constexpr const T& min(const T& a, const T& b) {
	return a < b ? a : b;
}

template<typename T>
constexpr T& max(T& a, T& b) {
	return a > b ? a : b;
}

template<typename T>
constexpr const T& max(const T& a, const T& b) {
	return a > b ? a : b;
}

template<typename From, typename To>
To union_cast(From from) {
	union unioncast {
		From from;
		To to;

		unioncast(From f) : from(f) {}

		To getTo() { return to; }
	};
	unioncast c(from);
	return c.getTo();
}

template<typename From, typename To>
To union_cast(From* from) {
	return union_cast<From, To>(*from);
}

/*
template<typename T1, typename T2>
T1* union_cast(T2* ptr) {
	union {
		T1* p1;
		T2* p2;
	} u;
	u.p2 = ptr;
	return u.p1;
}

template<typename T1, typename T2>
const T1* union_cast(const T2* ptr) {
	union {
		const T1* p1;
		const T2* p2;
	} u;
	u.p2 = ptr;
	return u.p1;
}
*/

template<typename T, T A, T B> struct Min { static const T min = A < B ? A : B; };

template<typename T, T A, T B> struct Max { static const T max = A > B ? A : B; };

template<size_type Start, size_type Count>
struct Repeat {
	//
	/// Copy a value from y to x
	template<typename T1, typename T2>
	static inline void copy(T1* x, const T2* y) {
		x[Start + Count - 1] = static_cast<T1>(y[Start + Count - 1]);
		Repeat<Start, Count - 1>::copy(x, y);
	}

	template<typename T1, typename T2>
	static inline void copy(T1* x, T2 y) {
		x[Start + Count - 1] = static_cast<T1>(y);
		Repeat<Start, Count - 1>::copy(x, y);
	}

	//
	/// compare values from y and x
	template<typename T1, typename T2>
	static inline void equal(const T1* x, const T2* y) {
		return x[Start + Count - 1] == y[Start + Count - 1] ? Repeat<Start, Count - 1>::equal(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline void equal(const T1* x, T2 y) {
		return x[Start + Count - 1] == y ? Repeat<Start, Count - 1>::equal(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline void notequal(const T1* x, const T2* y) {
		return x[Start + Count - 1] != y[Start + Count - 1] ? Repeat<Start, Count - 1>::notequal(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline void notequal(const T1* x, T2 y) {
		return x[Start + Count - 1] != y ? Repeat<Start, Count - 1>::notequal(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline bool all_greater(const T1* x, const T2* y) {
        return x[Start + Count - 1] > y[Start + Count - 1] ? Repeat<Start, Count - 1>::all_greater(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline bool all_greater(const T1* x, T2 y) {
		return x[Start + Count - 1] > y ? Repeat<Start, Count - 1>::all_greater(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline bool all_less(const T1* x, const T2* y) {
        return x[Start + Count - 1] < y[Start + Count - 1] ? Repeat<Start, Count - 1>::all_greater(x, y) : false;
	}

	template<typename T1, typename T2>
	static inline bool all_less(const T1* x, T2 y) {
        return x[Start + Count - 1] < y ? Repeat<Start, Count - 1>::all_greater(x, y) : false;
	}

	//
	/// calculate with values from X and Y
	template<typename T1, typename T2>
	static inline void add(T1* x, const T2* y) {
		x[Start + Count - 1] += y[Start + Count - 1];
		Repeat<Start, Count - 1>::add(x, y);
	}

	template<typename T1, typename T2>
	static inline void add(T1* x, T2 y) {
		x[Start + Count - 1] += y;
		Repeat<Start, Count - 1>::add(x, y);
	}

	template<typename T1, typename T2>
	static inline void sub(T1* x, const T2* y) {
		x[Start + Count - 1] -= y[Start + Count - 1];
		Repeat<Start, Count - 1>::sub(x, y);
	}

	template<typename T1, typename T2>
	static inline void sub(T1* x, T2 y) {
		x[Start + Count - 1] -= y;
		Repeat<Start, Count - 1>::sub(x, y);
	}

	template<typename T1, typename T2>
	static inline void mul(T1* x, const T2* y) {
		x[Start + Count - 1] *= y[Start + Count - 1];
		Repeat<Start, Count - 1>::mul(x, y);
	}

	template<typename T1, typename T2>
	static inline void mul(T1* x, T2 y) {
		x[Start + Count - 1] *= y;
		Repeat<Start, Count - 1>::mul(x, y);
	}

	template<typename T1, typename T2>
	static inline void div(T1* x, const T2* y) {
		ASSERT(y[Start] == T2(0), "Division by 0");
		x[Start + Count - 1] /= y[Start + Count - 1];
		Repeat<Start, Count - 1>::div(x, y);
	}

	template<typename T1, typename T2>
	static inline void div(T1* x, T2 y) {
		//DLOG_IF(y[Start] == T2(0), WARNING) << "Division by 0.";
		ASSERT(y[Start] == T2(0), "Devision by 0");
		x[Start + Count - 1] /= y;
		Repeat<Start, Count - 1>::div(x, y);
	}

};

template<size_type Start>
struct Repeat<Start, 0> {
	template<typename T1, typename T2>
	static inline void copy(T1* x, const T2* y) {
		x[Start] = static_cast<T1>(y[Start]);
	}

	template<typename T1, typename T2>
	static inline void copy(T1* x, T2 y) {
		x[Start] = static_cast<T1>(y);
	}

	template<typename T1, typename T2>
	static inline void equal(const T1* x, const T2* y) {
		return x[Start] == y[Start];
	}

	template<typename T1, typename T2>
	static inline void equal(const T1* x, T2 y) {
		return x[Start] == y;
	}

	template<typename T1, typename T2>
	static inline void notequal(const T1* x, const T2* y) {
		return x[Start] != y[Start];
	}

	template<typename T1, typename T2>
	static inline void notequal(const T1* x, T2 y) {
		return x[Start] != y;
	}

	template<typename T1, typename T2>
	static inline bool all_greater(const T1* x, const T2* y) {
		return x[Start] > y[Start];
	}

	template<typename T1, typename T2>
	static inline bool all_greater(const T1* x, T2 y) {
		return x[Start] > y;
	}

	template<typename T1, typename T2>
	static inline bool all_less(const T1* x, const T2* y) {
		return x[Start] < y[Start];
	}

	template<typename T1, typename T2>
	static inline bool all_less(const T1* x, T2 y) {
		return x[Start] < y;
	}

	template<typename T1, typename T2>
	static inline void add(T1* x, const T2* y) {
		x[Start] += y[Start];
	}

	template<typename T1, typename T2>
	static inline void add(T1* x, T2 y) {
		x[Start] += y;
	}

	template<typename T1, typename T2>
	static inline void sub(T1* x, const T2* y) {
		x[Start] -= y[Start];
	}

	template<typename T1, typename T2>
	static inline void sub(T1* x, T2 y) {
		x[Start] -= y;
	}

	template<typename T1, typename T2>
	static inline void mul(T1* x, const T2* y) {
		x[Start] *= y[Start];
	}

	template<typename T1, typename T2>
	static inline void mul(T1* x, T2 y) {
		x[Start] *= y;
	}

	template<typename T1, typename T2>
    static inline void div(T1* x, const T2* y) {
		x[Start] /= y[Start];
	}

	template<typename T1, typename T2>
    static inline void div(T1* x, T2 y) {
		x[Start] /= y;
	}
};

//
/// Check for equality of 2 types
template<typename T1, typename T2>
struct Is_SameType : public std::false_type {};

template<typename T1>
struct Is_SameType<T1,T1> : public std::true_type {};

//
/// Get Nth value from an argument package
namespace detail {
	template<size_type N, typename Arg1, typename ...Args>
	struct __NthType {
		typedef typename __NthType<N - 1, Args...>::type type;
	};

	template<typename Arg1, typename ...Args>
	struct __NthType<0, Arg1, Args...> {
        typedef Arg1 type;
	};
}

template<size_type N, typename ...Args>
struct NthType {
	static_assert(N < sizeof...(Args), "Args has less parameters than N.");
	typedef typename detail::__NthType<N, Args...>::type type;
};

template<>
struct NthType<0, void>{
	typedef void type;
};

//
/// some function traits
template<typename Signature>
struct Function_Traits {};

template<typename Result, typename ...Args>
struct Function_Traits<Result(Args...)> {
	typedef Result result_type;
	typedef Result(*Signature)(Args...);
	static const size_type arity = sizeof...(Args);

	template<size_type N>
	using ArgN = NthType<N, Args...>;
};

template<typename Result, typename ...Args>
struct Function_Traits<Result(*)(Args...)>{
	typedef Result result_type;
	typedef Result(*Signature)(Args...);
	static const size_type arity = sizeof...(Args);
	static const bool IsMemberFunction = false;

	template<size_type N>
	using ArgN = NthType<N,Args...>;
};

template<typename Result, typename Class, typename ...Args>
struct Function_Traits<Result(Class::*)(Args...)> {
	typedef Result result_type;
	typedef Class class_type;
	typedef Result(Class::*Signature)(Args...);
	static const size_type arity = sizeof...(Args);
	static const bool IsMemberFunction = true;

	template<size_type N>
	using ArgN = NthType<N, Args...>;
};

template<typename Result, typename Class, typename ...Args>
struct Function_Traits<Result(Class::*)(Args...) const> {
	typedef Result result_type;
	typedef Class class_type;
	typedef Result(Class::*Signature)(Args...) const;
	static const size_type arity = sizeof...(Args);
	static const bool IsMemberFunction = true;

	template<size_type N>
	using ArgN = NthType<N, Args...>;
};


template<typename Signature>
struct Result_Of {
	typedef typename Function_Traits<Signature>::result_type result_type;
};

template<bool Condition, typename Type, Type condTrue, Type condFalse>
struct If_Else {
	static const Type value = condFalse;
};

template<typename Type, Type condTrue, Type condFalse>
struct If_Else<true, Type, condTrue, condFalse> {
	static const Type value = condTrue;
};

class MemberFunction { size_type _() { return 0; } public: static const size_type MEMBERFUNC_SIZE = sizeof(&MemberFunction::_); };

template<typename T>
struct ArrayType {};

template<typename T, size_type N>
struct ArrayType<T[N]> {
	typedef T value_type;
	static const size_type SIZE = N;
};

template<typename T>
struct IsPOD {
	static const bool value = std::is_pod<T>::value;
};

template<bool Flag>
struct ToType {};

typedef ToType<true> TrueType;
typedef ToType<false> FalseType;

template<typename IntegerType, IntegerType M>
struct NextPowerOf2 {
	typedef IntegerType value_type;
private:
	static const value_type n = M - 1;
	static const value_type n1 = n | n >> 1;
	static const value_type n2 = n1 | n1 >> 2;
	static const value_type n4 = n2 | n2 >> 4;
	static const value_type n8 = n4 | n4 >> 8;
	static const value_type n16 = n8 | n8 >> 16;
	static const value_type n32 = n16 | n16 >> 32;
public:
	static const value_type value = n32 + 1;
};

template<typename Type>
struct NumBits {
	static const size_type value = sizeof(Type) * 8;
};

namespace detail {
	template<typename IntegerType, size_type N, bool>
	struct _MaxWithNBits {
	public:
		static const IntegerType value = ~(~0 << N);
	};

	template<typename IntegerType, size_type N>
	struct _MaxWithNBits<IntegerType, N, true> {
	public:
		static const IntegerType ONES = IntegerType(0xFFFFFFFFFFFFFFFF);
	public:
		static const IntegerType value = ONES;
	};
}

template<typename IntegerType, size_type N>
struct MaxUnsignedWithNBits : public detail::_MaxWithNBits<IntegerType, N, (N > sizeof(IntegerType) * 8)> {};


template<typename ValueType, size_type N>
struct CutToNBits {
private:
	static const size_type remaining = sizeof(ValueType) * 8 - N;
public:
	static const size_type size = N;
	union {
		ValueType  set;
		struct {
			ValueType _ : remaining;
			ValueType get : size;
		};
	};
};

ENDNAMESPACE
