#pragma once

#include <Allocator/Allocator.h>

BEGINNAMESPACE

class FreeList : public Allocator {
public:
	FreeList();
	FreeList(size_type size);

	//FreeList(void* start, size_type size);
	//FreeList(void* start, void* end);
	
	FreeList(FreeList&& freeList);
	FreeList& operator = (FreeList&& rhs);

	void initialize();
	void initialize(size_type size);
	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	size_type getSize(void* ptr) const;

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* ptr);
	void reset();

private:
	struct AllocationHeader {
		size_type size : 32;
		size_type adjustment : 32;
	};

	struct FreeBlock {
		size_type size : 32;
		size_type next : 32;
	};

	FreeBlock* m_FreeBlocks;
};

ENDNAMESPACE