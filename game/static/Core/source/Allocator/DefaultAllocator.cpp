#include <Allocator/DefaultAllocator.h>
#include <Memory/Malloc.h>

BEGINNAMESPACE

DefaultAllocator::DefaultAllocator() : Allocator()
{}

/*
DefaultAllocator::DefaultAllocator(void * start, size_type size) : Allocator(start, size)
{}

DefaultAllocator::DefaultAllocator(void * start, void * end) : Allocator(start, end)
{}
*/

DefaultAllocator::DefaultAllocator(size_type size) : Allocator()
{}

void DefaultAllocator::initialize()
{
}

void DefaultAllocator::initialize(size_type size)
{}

void DefaultAllocator::initialize(void * start, size_type size)
{}

void DefaultAllocator::initialize(void * start, void * end)
{}

void * DefaultAllocator::allocate(size_type size, uint8 alignment, uint8 offset)
{
	return Malloc::Alloc(size, alignment, offset);
}

void DefaultAllocator::free(void * ptr)
{
	Malloc::Free(ptr);
}

void DefaultAllocator::reset()
{
}

ENDNAMESPACE