#include "Memory/Malloc.h"

#if OS_WINDOWS
#	include <Windows.h>
#elif OS_LINUX
#   include <stdlib.h>
#endif

BEGINNAMESPACE

void * Malloc::Alloc(size_type size, size_type alignment, size_type offset)
{
#	if OS_WINDOWS
	return _aligned_offset_malloc(size, alignment, offset);
#   elif OS_LINUX
    return malloc(size);
    return aligned_alloc(size, alignment);
#	else
#	pragma error "Implement Malloc::Alloc for this operating system"
	return nullptr;
#	endif
}

void Malloc::Free(void * ptr)
{
#	if OS_WINDOWS
	return _aligned_free(ptr);
#   elif OS_LINUX
    return free(ptr);
#	else
#	pragma error "Implement Malloc::Free for this operating system"
#	endif
}

void * Malloc::Realloc(void * ptr, size_type new_size, size_type alignment, size_type offset)
{
#	if OS_WINDOWS
	return _aligned_offset_realloc(ptr, new_size, alignment, offset);
#   elif OS_LINUX
    Free(ptr);
    return Alloc(new_size, alignment, offset);
#	else
#	pragma error "Implement Malloc::Realloc for this operating system"
	return nullptr;
#	endif
}

bool Malloc::ValidateHeap() {
#	if OS_WINDOWS
	int32 res = _heapchk();
	ASSERT(res != _HEAPBADBEGIN, "Bad heap begin");
	ASSERT(res != _HEAPBADNODE, "Bad heap node");
	ASSERT(res != _HEAPBADPTR, "Bad heap ptr");
	ASSERT(res != _HEAPEMPTY, "Bad heap empty");
	ASSERT(res == _HEAPOK, "Heap not okay");
	return true;
#   elif OS_LINUX
    return true;
#	else
#	pragma error "Implement Malloc::ValidateHeap for this operating system (or just return true)"
#	endif
}


ENDNAMESPACE
