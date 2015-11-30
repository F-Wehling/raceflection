#pragma once

#include <stdint.h>
#include <float.h>
#include <string>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <chrono>
#include <functional>
#include <atomic>
#include <assert.h>
#include <cstring>

#define TO_SYMBOL(X) #X
#define TO_STRING(X) TO_SYMBOL(X)
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define BUILD_VERSION 1
#define VERSION_STRING TO_STRING(MAJOR_VERSION) ## "." ## TO_STRING(MINOR_VERSION) ## "." ## TO_STRING(BUILD_VERSION) 

#if defined _MSC_VER
#	define OS_WINDOWS 1
#	define IDE_MSVC 1
#elif defined __unix__
#	define OS_LINUX 1
#else
#	error "Either linux or windows to compile"
#endif

#if OS_WINDOWS == 1
#	define OS_LINUX 0
#elif OS_LINUX == 1
#	define OS_WINDOWS 0
#endif

#if OS_WINDOWS
#	if !defined(_MSC_VER)
#		error "This project depends on Visual Studio 2015 or greater."
#	endif

#	if (_MSC_VER < 1900)
#		error "Please use Microsoft Visual Studio 2015 or greater to build."
#	endif
#	pragma warning(disable : 4244 4267 4275)
#else
	//Check for special linux things that necessary for core
	
	//
#endif

#if defined (_DEBUG)
#	define DEBUG_BUILD 1
#	define RELEASE_BUILD 0
#else
#	define DEBUG_BUILD 0
#	define RELEASE_BUILD 1
#endif

#define NS_NAME racing	//Everything is embeded in this namespace

#if defined(NS_NAME)
#	define BEGINNAMESPACE namespace NS_NAME {
#	define ENDNAMESPACE }
#	define USING_NAMESPACE using namespace NS_NAME;
#else
#	define BEGINNAMESPACE
#	define ENDNAMESPACE
#	define USING_NAMESPACE
#endif

#if OS_WINDOWS
#	if DLL_EXPORT
#		define DLL_API __declspec(dllexport)
#	else
#		define DLL_API __declspec(dllimport)
#	endif
#	define COMPILER_BARRIER _Compiler_barrier
#	define MEMORY_BARRIER 
#	define _FUNCTION __FUNCTION__
#	define _LINE __LINE__
#	define _FILE __FILE__
#	define _debugbreak __debugbreak
#else 
#	define DLL_API
#	define COMPILER_BARRIER
#	define MEMORY_BARRIER
#	define _FUNCTION __FUNCTION__
#	define _LINE __LINE__
#	define _FILE __FILE__
#	define _debugbreak
#endif

#define CACHE_LINE_SIZE 64
#define DEFAULT_ALIGNMENT 16

#if UNICODE
#	define USE_WIDECHAR 1
#else
#	define USE_WIDECHAR 0
#endif

inline void noop(){}

#	define NO_OP noop

#	define FORCE_INLINE __forceinline

BEGINNAMESPACE

typedef std::int8_t  int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

typedef std::uint8_t  uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;

typedef float  float32;
typedef double float64;

//
/// String
typedef std::string Ansistring;
typedef std::wstring Widestring;
typedef Ansistring String;

//
/// Basic Stream
typedef std::istream IStream;
typedef std::ostream OStream;
typedef std::iostream IOStream;

//
/// StringStream
typedef std::stringstream  AnsistringStream;
typedef std::istringstream IAnsistringStream;
typedef std::ostringstream OAnsistringStream;
typedef std::wstringstream WidestringStream;
typedef std::wistringstream IWidestringStream;
typedef std::wostringstream OWidestringStream;

//
/// FileStream
typedef std::fstream AnsifileStream;
typedef std::ifstream IAnsifileStream;
typedef std::ofstream OAnsifileStream;
typedef std::wfstream WidefileStream;
typedef std::wifstream IWidefileStream;
typedef std::wofstream OWidefileStream;

#if USE_WIDECHAR
	typedef WidestringStream StringStream;
	typedef IWidestringStream IStringStream;
	typedef OWidestringStream OStringStream;
	typedef WidefileStream FileStream;
	typedef IWidefileStream IFileStream;
	typedef OWidefileStream OFileStream;

#	if defined TEXT
#		undef TEXT
#	endif
#	define TEXT(text) L ## txt
#else
	typedef AnsistringStream StringStream;
	typedef IAnsistringStream IStringStream;
	typedef OAnsistringStream OStringStream;
	typedef AnsifileStream FileStream;
	typedef IAnsifileStream IFileStream;
	typedef OAnsifileStream OFileStream;

#	if defined TXT
#		undef TXT
#	endif
#	define TXT(txt) txt
#endif


typedef String::value_type tchar;
typedef char ansichar;
typedef wchar_t widechar;

typedef uint64 size_type;

template<size_type n>
struct IntegerOfSize {
//	static_assert(false, "Invalid sized integer");
};

template<>
struct IntegerOfSize<1> {
	typedef int8 s_type;
	typedef uint8 u_type;
};

template<>
struct IntegerOfSize<2> {
	typedef int16 s_type;
	typedef uint16 u_type;
};

template<>
struct IntegerOfSize<4> {
	typedef int32 s_type;
	typedef uint32 u_type;
};

template<>
struct IntegerOfSize<8> {
	typedef int64 s_type;
	typedef uint64 u_type;
};

template<size_type n> struct FloatOfSizeExists : public std::false_type {};
template<> struct FloatOfSizeExists<4> : public std::true_type{};
template<> struct FloatOfSizeExists<8> : public std::true_type{};

template<size_type n> struct FloatOfSize {};
template<> struct FloatOfSize<4> { typedef float32 value_type; };
template<> struct FloatOfSize<8> { typedef float64 value_type; };

typedef IntegerOfSize<sizeof(void*)>::s_type IntOfPtrSize;
typedef IntegerOfSize<sizeof(void*)>::u_type UIntOfPtrSize;

typedef unsigned char Byte;

//
/// Array
template<typename Value, size_type size>
using PlainArray = Value[size];

//some final checks
static_assert(sizeof(IntOfPtrSize) > 4, "Please use a 64-compiler to compile this project");

ENDNAMESPACE

#define S_PTR(T) \
	class T; \
	typedef T* T ## Ptr; \
	typedef const T* T ## ConstPtr;

#include <Predefs.h>

//size and point type
#include <Size.h>
#include <Point.h>
