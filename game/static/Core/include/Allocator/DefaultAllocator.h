#pragma once

#include <Allocator/Allocator.h>

BEGINNAMESPACE

class DefaultAllocator : public Allocator {
public:
	DefaultAllocator();
	DefaultAllocator(size_type size);

	inline DefaultAllocator(DefaultAllocator&&){}
	inline DefaultAllocator& operator = (DefaultAllocator&&){}
	
	//DefaultAllocator(void* start, size_type size);
	//DefaultAllocator(void* start, void* end);

	void initialize();
	void initialize(size_type size);
	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* ptr);
	void reset();
private:
};

ENDNAMESPACE