#pragma once

#include <Allocator/Allocator.h>
#include <Utilities/Number.h>

BEGINNAMESPACE

class PoolAllocator : public Allocator {
public:
	PoolAllocator();
	PoolAllocator(size_type size);

	//PoolAllocator(size_type objectSize, uint8 objectAlignment, void* start, size_type size, size_type offset = 0);
	//PoolAllocator(size_type objectSize, uint8 objectAlignment, void* start, void* end, size_type offsetof = 0);

	void initialize(size_type objectSize, uint8 objectAlignment, uint8 offset = 0);
	void initialize(size_type size, size_type objectSize, uint8 objectAlignment, uint8 offset = 0);
	void initialize(size_type objectSize, uint8 objectAlignment, void* start, size_type size, uint8 offset = 0);
	void initialize(size_type objectSize, uint8 objectAlignment, void* start, void* end, uint8 offset = 0);

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* p);
	void reset();
private:
	size_type m_ObjectSize;
	size_type m_NumElements;
	size_type m_Offset;
	uint8 m_Alignment;

	void** m_FreeList;
};

template<typename ElementType, typename PoolAllocator>
ElementType* getNthElement(int32 n, PoolAllocator& alloc) {
	uint32 size = Number::NextPowerOf2(sizeof(ElementType));
	return (ElementType*)((Byte*)alloc.getStart() + (n * size));
}

template<typename ElementType, typename PoolAllocator>
uint32 getElementIndex(ElementType* element, PoolAllocator& alloc) {
	uint32 size = Number::NextPowerOf2(sizeof(ElementType));
	return std::distance((Byte*)alloc.getStart(), (Byte*)element) / size;
}


ENDNAMESPACE