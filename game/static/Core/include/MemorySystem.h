#pragma once

#include <CompileTime/Preprocessor.h>
#include <CompileTime/Templates.h>

#include <Allocator/DefaultAllocator.h>
#include <Allocator/DynamicStackAllocator.h>
#include <Allocator/LinearAllocator.h>
#include <Allocator/PoolAllocator.h>
#include <Allocator/FreeListAllocator.h>
#include <Allocator/StackAllocator.h>
#include <Allocator/ProxyAllocator.h>

#include <Utilities/SourceInfo.h>

BEGINNAMESPACE

template<typename Allocator, typename SyncPolicy = policy::NoSync, typename BoundsCheckingPolicy = policy::NoBoundsChecking, typename MemoryTrackingPolicy = policy::NoTracking, typename MemoryTaggingPolicy = policy::NoTagging>
using PassTroughProxyAllocator = ProxyAllocator<DefaultAllocator, SyncPolicy, BoundsCheckingPolicy, MemoryTrackingPolicy, MemoryTaggingPolicy>;

#if DEBUG_BUILD
typedef ProxyAllocator<DefaultAllocator, policy::NoSync, policy::PatternBoundsChecking, policy::IncrementTracking, policy::NoTagging> DefaultMemoryAllocator; //> forward to OS malloc/free, boundcheck + memory tracker
#else
typedef PassTroughProxyAllocator<DefaultAllocator> DefaultMemoryAllocator; //just default
#endif

extern DefaultMemoryAllocator g_DefaultAllocator;

template<typename T, typename Allocator>
inline void Delete(T* object, Allocator& region) {
	if (!object) return;
	object->~T(); //destructor
	region.free(object);
}

typedef TrueType PODType;
typedef FalseType NonPODType;

template<typename T, typename Allocator>
T* NewArray(Allocator& allocator, size_type N, const SourceInfo& info, uint8 alignment = std::alignment_of<T>::value) {
	return NewArray<T>(allocator, N, info, alignment, ToType<IsPOD<T>::value>());
}

template<typename T, typename Allocator>
T* NewArray(Allocator& allocator, size_type N, const SourceInfo& info, uint8 alignment , NonPODType) {
	union {
		void* as_void;
		uint32* as_size;
		T* as_T;
	};

	as_void = allocator.allocate(sizeof(T)*N + sizeof(uint32), alignment);
	*as_size++ = uint32(N);
	const T* end = as_T + N;
	while (as_T < end) new(as_T++) T;

	return (as_T - N);
}

template<typename T, typename Allocator>
T* NewArray(Allocator& allocator, size_type N, const SourceInfo& info, uint8 alignment, PODType) {
	return static_cast<T*>(allocator.allocate(sizeof(T)*N, alignment));
}
template<typename T, typename Allocator>
void DeleteArray(T* ptr, Allocator& allocator) {
	return DeleteArray(ptr, allocator, ToType<IsPOD<T>::value>());
}

template<typename T, typename Allocator>
void DeleteArray(T* ptr, Allocator& allocator, NonPODType) {
	if (!ptr) return;
	union {
		uint32* as_size;
		T* as_T;
	};
	as_T = ptr;

	uint32 size = as_size[-1];

	for (uint32 i = size; i > 0; --i) 
		as_T[i - 1].~T(); //call destructor
	allocator.free(ptr);
}

template<typename T, typename Allocator>
void DeleteArray(T* ptr, Allocator& allocator, PODType) {
	allocator.free(ptr);
}


#	define eng_new_1(type) new (g_DefaultAllocator.allocate(sizeof(type), alignof(type))) type
#	define eng_new_2(type, allocator) new (allocator.allocate(sizeof(type), alignof(type))) type
#	define eng_new_3(type, allocator, alignment) new (allocator.allocate(sizeof(type), alignment)) type

#	define eng_new_array_1(type) NewArray< typename ArrayType<type>::value_type >(g_DefaultAllocator, ArrayType<type>::SIZE, SOURCE_INFO)
#	define eng_new_array_2(type, allocator) NewArray< typename ArrayType<type>::value_type >(allocator, ArrayType<type>::SIZE, SOURCE_INFO)
#	define eng_new_array_3(type, allocator, Alignment) NewArray< typename ArrayType<type>::value_type >(allocator, ArrayType<type>::SIZE, SOURCE_INFO, Alignment)

#	define eng_delete_1(object) Delete(object, g_DefaultAllocator); object = nullptr
#	define eng_delete_2(object, allocator) Delete(object, allocator); object = nullptr

#	define eng_delete_array_1(object) DeleteArray(object, g_DefaultAllocator); object = nullptr
#	define eng_delete_array_2(object, allocator) DeleteArray(object, allocator); object = nullptr

#if OS_WINDOWS

#	define eng_new(...) PP_JOIN(eng_new_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)
#	define eng_delete(...) PP_JOIN(eng_delete_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)

#	define eng_new_array(...) PP_JOIN(eng_new_array_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)
#	define eng_delete_array(...) PP_JOIN(eng_delete_array_ , PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__)

#elif OS_LINUX

#	define eng_new(...) PP_IND_1(PP_JOIN(eng_new_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__))
#	define eng_delete(...) PP_IND_1(PP_JOIN(eng_delete_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__))

#	define eng_new_array(...) PP_IND_1(PP_JOIN(eng_new_array_, PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__))
#	define eng_delete_array(...) PP_IND_1(PP_JOIN(eng_delete_array_ , PP_NUM_ARGS(__VA_ARGS__))PP_PASS_VA(__VA_ARGS__))

#endif

#	define BYTE(n) n;
#	define KILOBYTE(n) n * 1024
#	define MEGABYTE(n) KILOBYTE(n) * 1024
#	define GIGABYTE(n) MEGABYTE(n) * 1024

ENDNAMESPACE
