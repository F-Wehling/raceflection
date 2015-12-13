#pragma once

#include <Allocator/Allocator.h>

BEGINNAMESPACE

class LinearAllocator : public Allocator {
public:
	LinearAllocator();
	LinearAllocator(size_type size);
	LinearAllocator(void* start, size_type size);
	LinearAllocator(void* start, void* end);

	LinearAllocator(LinearAllocator&& other);
	LinearAllocator& operator = (LinearAllocator&& rhs);

	void initialize();
	void initialize(size_type size);
	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* ptr);
	void reset();

private:
	Byte* m_Current;
};

ENDNAMESPACE