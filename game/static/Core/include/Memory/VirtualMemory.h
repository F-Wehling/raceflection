#pragma once

BEGINNAMESPACE

class VirtualMemory {
public:
	typedef void* VirtualMemoryPtr;
	typedef void* PhysicalMemoryPtr;

	static VirtualMemoryPtr Reserve(size_type size);
	static PhysicalMemoryPtr Allocate(VirtualMemoryPtr start, size_type size);
	static void Free(PhysicalMemoryPtr ptr, size_type size);
	static void Unreserve(VirtualMemoryPtr ptr, size_type size);
	static size_type PageSize();
};

ENDNAMESPACE