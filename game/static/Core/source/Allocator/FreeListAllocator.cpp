#include <Allocator/FreeListAllocator.h>
#include <Utilities/PointerTools.h>
#include <Utilities/Assert.h>

BEGINNAMESPACE

FreeList::FreeList() : Allocator(), m_FreeBlocks(nullptr)
{}

/*
FreeList::FreeList(void * start, size_type size) : Allocator(start, size){
	initialize(start, size);
}

FreeList::FreeList(void * start, void * end) : Allocator(start, end)
{
	initialize(start, end);
}
*/

FreeList::FreeList(size_type size) : Allocator(size), m_FreeBlocks(nullptr) {}

FreeList::FreeList(FreeList && freeList)
{
	*this = std::forward<FreeList>(freeList);
}

FreeList & FreeList::operator=(FreeList && rhs)
{
	if (this != &rhs) {
		Allocator::operator = (std::move(rhs));
		m_FreeBlocks = rhs.m_FreeBlocks;
		rhs.m_FreeBlocks = nullptr;
	}
	return *this;
}

void FreeList::initialize()
{
	ASSERT(m_Start != nullptr, "To call initialize(), the underlying memory must already be set");
	initialize(m_Start, m_Start + m_Size); //
}

void FreeList::initialize(size_type size)
{
	Allocator::initialize(size);
	initialize(m_Start, m_Start + m_Size);
}

void FreeList::initialize(void * start, size_type size)
{
	initialize(start, (Byte*)start + size);
}

void FreeList::initialize(void * start, void * end)
{
	Allocator::initialize(start, end);
	ASSERT(m_Size < std::numeric_limits<uint32>::max(), "A FreeList-Allocator can manage at most %d Byte of Memory.", std::numeric_limits<uint32>::max());
	ASSERT(m_Size > sizeof(FreeBlock), "The whole size (%d) must be greater than sizeof(FreeBlock)(%d)", m_Size, sizeof(FreeBlock));
	m_FreeBlocks = (FreeBlock*)m_Start;
	reset();
}

size_type FreeList::getSize(void * ptr) const
{
	AllocationHeader* header = (AllocationHeader*)((Byte*)ptr - sizeof(AllocationHeader));
	return header->size - header->adjustment;
}

void * FreeList::allocate(size_type size, uint8 alignment, uint8 offset)
{
	ASSERT(size != 0 && alignment != 0, "Neither size, nor alignment may be zero for allocation");

	FreeBlock* prev_free_block = nullptr;
	FreeBlock* free_block = m_FreeBlocks;

	size_type off = offset + sizeof(AllocationHeader);

	while (free_block != nullptr) {
		uint8 adjustment = (PointerTools::Align(((Byte*)free_block) + off, alignment) - off) - (Byte*)free_block;

		size_type totalSize = size + adjustment;

		if (free_block->size < totalSize) {
			prev_free_block = free_block;
			free_block = (FreeBlock*)(m_Start[free_block->next]);
			continue;
		}

		if (free_block->size - totalSize <= sizeof(AllocationHeader)) { //if the remaining memory for this block will be less than one minimal allocation
			totalSize = free_block->size;
			if (prev_free_block != nullptr) prev_free_block->next = free_block->next;
			else m_FreeBlocks = (FreeBlock*)(m_Start[free_block->next]);
		}
		else {
			FreeBlock* next = (FreeBlock*)(((Byte*)free_block) + totalSize);
			next->size = free_block->size - totalSize;
			next->next = free_block->next;

			if (prev_free_block != nullptr) prev_free_block->next = std::distance(m_Start, (Byte*)next);
			else m_FreeBlocks = next;
		}

		Byte* ptr = (Byte*)free_block + adjustment;

		AllocationHeader* header = (AllocationHeader*)(ptr - sizeof(AllocationHeader));
		header->size = totalSize;
		header->adjustment = adjustment;

		return (void*)ptr;
	}
	return nullptr;
}

void FreeList::free(void * ptr)
{
	ASSERT(ptr != nullptr, "A nullptr can't be freed");

	AllocationHeader* header = (AllocationHeader*)((Byte*)ptr - sizeof(AllocationHeader));
	Byte* block_start = (Byte*)ptr - header->adjustment;
	size_type block_size = header->size;
	Byte* block_end = block_start + block_size;

	FreeBlock* prev_free_block = nullptr;
	FreeBlock* free_block = m_FreeBlocks;
	while (free_block != nullptr) {
		if ((Byte*)free_block > block_end) break;

		prev_free_block = free_block;
		free_block = (FreeBlock*)(m_Start[free_block->next]);
	}

	if (prev_free_block == nullptr) {
		prev_free_block = (FreeBlock*)block_start;
		prev_free_block->size = block_size;
		prev_free_block->next = std::distance(m_Start, (Byte*)m_FreeBlocks);
		m_FreeBlocks = prev_free_block;
	}
	else if ((Byte*)prev_free_block + prev_free_block->size == block_start) {
		prev_free_block->size += block_size;
	}
	else {
		FreeBlock* tmp = (FreeBlock*)(block_start);
		tmp->size = block_size;
		tmp->next = prev_free_block->next;
		prev_free_block->next = std::distance(m_Start, (Byte*)tmp);
		prev_free_block = tmp;
	}
	if (free_block != nullptr && (Byte*)free_block == block_end) {
		prev_free_block->size += free_block->size;
		prev_free_block->next = free_block->next;
	}
}

void FreeList::reset()
{
	m_FreeBlocks->size = uint32(m_Size);
	m_FreeBlocks->next = 0;
}

ENDNAMESPACE

