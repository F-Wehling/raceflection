#pragma once

#include <Allocator/Allocator.h>

BEGINNAMESPACE

class StackAllocator : public Allocator {
	typedef uint32 AllocatorOffset;
	typedef uint32 AllocationIndex;
public:
	StackAllocator();
	StackAllocator(size_type size);
	StackAllocator(void* start, size_type size);
	StackAllocator(void* start, void* end);

	StackAllocator(StackAllocator&& other);
	StackAllocator& operator = (StackAllocator&& rhs);

	void initialize();
	void initialize(size_type size);
	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* ptr);
	void reset();
private:
	Byte* m_Current;
#	if DEBUG_BUILD
	AllocationIndex m_CurrentIdx;
#	endif
};

ENDNAMESPACE