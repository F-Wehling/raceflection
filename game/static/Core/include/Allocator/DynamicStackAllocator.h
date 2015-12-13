#pragma once

BEGINNAMESPACE

class DynamicStackAllocator {
public:
	DynamicStackAllocator();
	DynamicStackAllocator(size_type maxSize, size_type growingSize);

	void initialize(size_type maxSize, size_type growingSize);

	void* allocate(size_type size, uint8 alignment, uint8 offset = 0);
	void free(void* ptr);

	void freeToOS();
private:
	Byte* m_VirtualStart;
	Byte* m_VirtualEnd;
	Byte* m_PhysicalCurrent;
	Byte* m_PhysicalEnd;
	size_type m_GrowingSize;
};

ENDNAMESPACE