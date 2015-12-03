#include <Allocator/StackAllocator.h>
#include <Utilities/PointerTools.h>

BEGINNAMESPACE

StackAllocator::StackAllocator() : Allocator(), m_Current(nullptr)
{}

StackAllocator::StackAllocator(size_type size) : Allocator(size), m_Current(nullptr)
{
}

/*
StackAllocator::StackAllocator(void * start, size_type size) : Allocator(start,size)
{
	initialize(start, size);
}

StackAllocator::StackAllocator(void * start, void * end) : Allocator(start,end)
{
	initialize(start, end);
}
*/

void StackAllocator::initialize()
{
	ASSERT(m_Start != nullptr, "To call initialize(), the underlying memrory must already be set.");
	StackAllocator(m_Start, m_Start + m_Size);
}

void StackAllocator::initialize(size_type size)
{
	Allocator::initialize(size);
	reset();
}

void StackAllocator::initialize(void * start, size_type size)
{
	Allocator::initialize(start, size);
	reset();
}

void StackAllocator::initialize(void * start, void * end)
{
	Allocator::initialize(start, end);
	reset();
}

void* StackAllocator::allocate(size_type size, uint8 alignment, uint8 offset)
{
	size += sizeof(AllocatorOffset) 
#	if DEBUG_BUILD
		+ sizeof(AllocationIndex)
#	endif
	;
	offset += sizeof(AllocatorOffset)
#	if DEBUG_BUILD
		+ sizeof(AllocationIndex)
#	endif
	;

	uint32 allocationOffset = static_cast<uint32>(m_Current - m_Start);

	m_Current = PointerTools::Align(m_Current + offset, alignment) - offset;

	if (m_Current + size > m_Start + m_Size) {
		return nullptr;
	}
	
	union {
		void* as_void;
		Byte* as_byte;
		uint32* as_uint32;
	};

	as_byte = m_Current;

	//store the allocation offset
	*as_uint32 = allocationOffset;
	as_byte += sizeof(AllocatorOffset);
	
#	if DEBUG_BUILD
	//store the allocation index
	*as_uint32 = m_CurrentIdx++;
	as_byte += sizeof(AllocationIndex);
#	endif

	void* userPtr = as_void;
	m_Current += size;
	return userPtr;
}

void StackAllocator::free(void * ptr)
{
	ASSERT(std::distance(m_Start, static_cast<Byte*>(ptr)) > 0 && std::distance(static_cast<Byte*>(ptr), m_Start + m_Size) > 0, "The pointer wasn't allocated by this allocator");
	union {
		void* as_void;
		Byte* as_byte;
		uint32* as_uint32;
	};
	as_void = ptr;
	as_byte -= sizeof(AllocatorOffset) 
#	if DEBUG_BUILD
		- sizeof(AllocationIndex)
#	endif
		; //to user pointer

	uint32 offset = *as_uint32;

#	if DEBUG_BUILD
	as_byte += sizeof(AllocatorOffset);
	uint32 allocationIndex = *as_uint32;
	ASSERT(allocationIndex == m_CurrentIdx - 1, "StackAllocator - LIFO order violated.");
#	endif
	m_Current = m_Start + offset;
}

void StackAllocator::reset() {
	m_Current = m_Start; //reset it
}


ENDNAMESPACE

