#pragma once

BEGINNAMESPACE

//
/// implementation should be provided by exactly one lib-file (e.g WindowPlatform or LinuxPlatform)
class Malloc {
public:
	static void* Alloc(size_type size, size_type alignment, size_type offset);
	static void Free(void* ptr);
	static void* Realloc(void* ptr, size_type new_size, size_type alignment, size_type offset);
	static bool ValidateHeap();
};


ENDNAMESPACE