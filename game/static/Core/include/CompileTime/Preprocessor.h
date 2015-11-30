#pragma once

#define PP_STRINGIZE_HELPER(token)    #token
#define PP_STRINGIZE(str)             PP_STRINGIZE_HELPER(str)

#define PP_JOIN(x, y)                    PP_JOIN_HELPER(x, y)
#define PP_JOIN_HELPER(x, y)             PP_JOIN_HELPER_HELPER(x, y)
#define PP_JOIN_HELPER_HELPER(x, y)      x##y

#define PP_IND_1(X) X
#define PP_INDIRECT(X,Y) X(Y)

#if _MSC_VER >= 1400
#	define PP_LEFT_PARENTHESIS (
#	define PP_RIGHT_PARENTHESIS )
#else
#   define PP_LP(...) __VA_ARGS__
#endif

#if _MSC_VER >= 1400
#    define __PP_NUM_VARARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,  N, ...)    N
#    define __PP_NUM_VARARGS_REVERSE_SEQUENCE 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#    define PP_NUM_ARGS(...) __PP_NUM_VARARGS_HELPER PP_LEFT_PARENTHESIS __VA_ARGS__, __PP_NUM_VARARGS_REVERSE_SEQUENCE PP_RIGHT_PARENTHESIS
#else
#    define __PP_NUM_VARARGS_REVERSE_SEQUENCE 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#    define __PP_NUM_VARARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N
#    define PP_NUM_ARGS(...) __PP_NUM_VARARGS_HELPER ( __VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 )
#endif

#if _MSC_VER >= 1400
#    define PP_PASS_VA(...)                            PP_LEFT_PARENTHESIS __VA_ARGS__ PP_RIGHT_PARENTHESIS
#else
#   define PP_PASS_VA(...) (__VA_ARGS__)
#endif

#define PP_DECLARE_FLAGS_ENUM(name, n)                    name = (1u << n),
#define PP_DECLARE_FLAGS_BITS(name, n)                    uint32 name : 1;
#define PP_DECLARE_FLAGS_TO_STRING(name, n)               case name: return PP_STRINGIZE(name);

#define __PP_EXPAND_ARGS_1(op) op(0)
#define __PP_EXPAND_ARGS_2(op, a1) op(a1, 0)
#define __PP_EXPAND_ARGS_3(op, a1, a2) op(a1, 0) op(a2, 1)
#define __PP_EXPAND_ARGS_4(op, a1, a2, a3) op(a1,0) op(a2, 1) op(a3, 2)
#define __PP_EXPAND_ARGS_5(op, a1, a2, a3, a4) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3)
#define __PP_EXPAND_ARGS_6(op, a1, a2, a3, a4, a5) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4)
#define __PP_EXPAND_ARGS_7(op, a1, a2, a3, a4, a5, a6) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5)
#define __PP_EXPAND_ARGS_8(op, a1, a2, a3, a4, a5, a6, a7) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6)
#define __PP_EXPAND_ARGS_9(op, a1, a2, a3, a4, a5, a6, a7, a8) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7)
#define __PP_EXPAND_ARGS_10(op, a1, a2, a3, a4, a5, a6, a7, a8, a9) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8)
#define __PP_EXPAND_ARGS_11(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) op(a1,0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9)
#define __PP_EXPAND_ARGS_12(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
		 __PP_EXPAND_ARGS_11(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) op(a11, 10)
#define __PP_EXPAND_ARGS_13(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
		 __PP_EXPAND_ARGS_12(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) op(a12, 11)
#define __PP_EXPAND_ARGS_14(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
		 __PP_EXPAND_ARGS_13(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) op(a13, 12)
#define __PP_EXPAND_ARGS_15(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
		 __PP_EXPAND_ARGS_14(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) op(a14, 13)
#define __PP_EXPAND_ARGS_16(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
		 __PP_EXPAND_ARGS_15(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) op(a15, 14)
#define __PP_EXPAND_ARGS_17(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) \
		 __PP_EXPAND_ARGS_16(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) op(a16, 15)
#define __PP_EXPAND_ARGS_18(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17) \
		 __PP_EXPAND_ARGS_17(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) op(a17, 16)
#define __PP_EXPAND_ARGS_19(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18) \
		 __PP_EXPAND_ARGS_18(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17) op(a18, 17)
#define __PP_EXPAND_ARGS_20(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19) \
		 __PP_EXPAND_ARGS_19(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18) op(a19, 18)
#define __PP_EXPAND_ARGS_21(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20) \
		 __PP_EXPAND_ARGS_20(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19) op(a20, 19)
#define __PP_EXPAND_ARGS_22(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21) \
		 __PP_EXPAND_ARGS_21(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20) op(a21, 20)
#define __PP_EXPAND_ARGS_23(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22) \
		 __PP_EXPAND_ARGS_22(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21) op(a22, 21)
#define __PP_EXPAND_ARGS_24(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23) \
		 __PP_EXPAND_ARGS_23(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22) op(a23, 22)
#define __PP_EXPAND_ARGS_25(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24) \
		 __PP_EXPAND_ARGS_24(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23) op(a24, 23)
#define __PP_EXPAND_ARGS_26(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25) \
		 __PP_EXPAND_ARGS_25(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24) op(a25, 24)
#define __PP_EXPAND_ARGS_27(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26) \
		 __PP_EXPAND_ARGS_26(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25) op(a26, 25)
#define __PP_EXPAND_ARGS_28(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27) \
		 __PP_EXPAND_ARGS_27(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26) op(a27, 26)
#define __PP_EXPAND_ARGS_29(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28) \
		 __PP_EXPAND_ARGS_28(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27) op(a28, 27)
#define __PP_EXPAND_ARGS_30(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29) \
		 __PP_EXPAND_ARGS_29(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28) op(a29, 28)
#define __PP_EXPAND_ARGS_31(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30) \
		 __PP_EXPAND_ARGS_30(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29) op(a30, 29)
#define __PP_EXPAND_ARGS_32(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31) \
		 __PP_EXPAND_ARGS_31(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30) op(a31, 30)
#define __PP_EXPAND_ARGS_33(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32) \
		 __PP_EXPAND_ARGS_32(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31) op(a32, 31)

#define PP_EXPAND_ARGS(...) PP_JOIN(__PP_EXPAND_ARGS_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)
//#define PP_EXPAND_ARGS(...) PP_JOIN(__PP_EXPAND_ARGS_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)
