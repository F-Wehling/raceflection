#include <Allocator/LinearAllocator.h>
#include <Utilities/PointerTools.h>
#include <Utilities/Assert.h>

BEGINNAMESPACE

LinearAllocator::LinearAllocator() : Allocator() , m_Current(nullptr)
{}

LinearAllocator::LinearAllocator(void * start, size_type size)
	: Allocator(start, size)
{
	initialize(start, size);
}

LinearAllocator::LinearAllocator(void * start, void * end)
	: Allocator(start, end)
{
	initialize(start, end);
}

void LinearAllocator::initialize(void * start, size_type size)
{
	Allocator::initialize(start, size);
	reset();
}

void LinearAllocator::initialize(void * start, void * end)
{
	Allocator::initialize(start, end);
	reset();
}

void* LinearAllocator::allocate(size_type size, uint8 alignment, uint8 offset)
{
	m_Current = PointerTools::Align(m_Current + offset, alignment) - offset;

	void* userPtr = (Byte*)m_Current;
	m_Current += size;
	
	if (m_Current >= m_Start + m_Size) {
		return nullptr;
	}

	return userPtr;
}

void LinearAllocator::free(void * ptr)
{
	ASSERT(std::distance(m_Start, static_cast<Byte*>(ptr)) >= 0 && std::distance(static_cast<Byte*>(ptr), m_Start + m_Size) > 0, "The pointer wasn't allocated by this allocator");
} //in release free does not do anything - in debug it just checks 

void LinearAllocator::reset() {
	m_Current = m_Start; //reset it
}

ENDNAMESPACE

