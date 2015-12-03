#include <Allocator/PoolAllocator.h>
#include <Utilities/PointerTools.h>
#include <Utilities/Number.h>
#include <Utilities/Assert.h>

BEGINNAMESPACE

PoolAllocator::PoolAllocator() : Allocator()
{
}

/*
PoolAllocator::PoolAllocator(size_type objectSize, uint8 objectAlignment, void * start, size_type size, size_type offset) : Allocator(start,size)
{
	initialize(objectSize, objectAlignment, start, size, offset);
}

PoolAllocator::PoolAllocator(size_type objectSize, uint8 objectAlignment, void * start, void * end, size_type offset) : Allocator(start, end)
{
	initialize(objectSize, objectAlignment, start, end, offset);
}
*/

PoolAllocator::PoolAllocator(size_type size) : Allocator(size) {
}

void PoolAllocator::initialize(size_type objectSize, uint8 objectAlignment, uint8 offset)
{
	ASSERT(m_Start != nullptr, "To call initialize(), the underlying memrory must already be set.");
	initialize(objectSize, objectAlignment, m_Start, m_Start + m_Size, offset);
}

void PoolAllocator::initialize(size_type size, size_type objectSize, uint8 objectAlignment, uint8 offset)
{
	Allocator::initialize(size);
	initialize(objectSize, objectAlignment, m_Start, m_Start + m_Size, offset);
}

void PoolAllocator::initialize(size_type objectSize, uint8 objectAlignment, void * start, size_type size, uint8 offset)
{
	initialize(objectSize, objectAlignment, start, (Byte*)start + size, offset);
}

void PoolAllocator::initialize(size_type objectSize, uint8 objectAlignment, void * start, void * end, uint8 offset)
{
	Allocator::initialize(start, end);
	objectSize = Number::NextPowerOf2(objectSize + offset); //object size (objectSize + offset)...
	objectAlignment = Number::NextPowerOf2(objectAlignment); //... and alignment should be power of 2
	m_ObjectSize = max(objectSize, sizeof(void**)); //power of 2, but at least sizeof(void**)
	m_Alignment = objectAlignment;
	m_Offset = offset;
	reset();
}

void * PoolAllocator::allocate(size_type size, uint8 alignment, uint8 offset)
{
	ASSERT(m_Alignment % alignment == 0, "The pool can't provide alignment of %d. It is %d aligned.", alignment, m_Alignment);
	ASSERT(size <= m_ObjectSize, "The desired size of %d can't be provided by this pool (%d-elements)", size, m_ObjectSize);
	ASSERT(offset == m_Offset, "The pools offset (%d) is not the desired %d offset", m_Offset, offset);
	if (m_FreeList == nullptr) return nullptr;
	void** p_res = m_FreeList;
	void** p = m_FreeList;
	m_FreeList = (void**)(*m_FreeList);
	return p;
}

void PoolAllocator::free(void * p)
{
	ASSERT(m_Start <= p && p <= m_Start + m_Size, "The pointer wasn't allocated by this pool");
	*((void**)p) = m_FreeList;
	m_FreeList = (void**)p;
}

void PoolAllocator::reset()
{
	if (m_Start == nullptr) return;
	Byte* poolStart = PointerTools::Align(m_Start + m_Offset, m_Alignment); //align m_Start to m_Alignment with, but added offset. 
	size_type adjustment = poolStart - m_Start;

	m_FreeList = (void**)poolStart;

	m_NumElements = (m_Size - adjustment) / m_ObjectSize;
	
	void** p = m_FreeList;
	for (size_type i = 0; i < m_NumElements - 1; ++i) {
		*p = (void*)(((Byte*)p) + m_ObjectSize);
		p = (void**)*p;
	}
	*p = nullptr;
}


ENDNAMESPACE
