#include <Allocator/DynamicStackAllocator.h>
#include <Memory/VirtualMemory.h>
#include <Utilities/PointerTools.h>
#include <Utilities/Number.h>

BEGINNAMESPACE

DynamicStackAllocator::DynamicStackAllocator() : m_GrowingSize(0), m_VirtualStart(nullptr), m_VirtualEnd(nullptr), m_PhysicalCurrent(nullptr), m_PhysicalEnd(nullptr) 
{}

DynamicStackAllocator::DynamicStackAllocator(size_type maxSize, size_type growingSize)
	: m_GrowingSize(growingSize)
{
	initialize(maxSize, growingSize);
}

void DynamicStackAllocator::initialize(size_type maxSize, size_type growingSize)
{
	m_VirtualStart = static_cast<Byte*>(VirtualMemory::Reserve(maxSize));
	m_VirtualEnd = m_VirtualStart + maxSize;
	m_PhysicalCurrent = m_VirtualStart;
	m_PhysicalEnd = m_VirtualStart;
}

void * DynamicStackAllocator::allocate(size_type size, uint8 alignment, uint8 offset)
{
	void* ptr = nullptr;

	m_PhysicalCurrent = PointerTools::Align(m_PhysicalCurrent + offset, alignment) - offset;

	if (m_PhysicalCurrent + size > m_PhysicalEnd) {
		size_type needPhysicalMem = Number::RoundUpToPowerOf2(size, m_GrowingSize);
		if (m_PhysicalEnd + needPhysicalMem > m_VirtualEnd) {
			return nullptr; //out of memory
		}
		VirtualMemory::Allocate(m_PhysicalEnd, needPhysicalMem);
		m_PhysicalEnd += needPhysicalMem;
	}

	return m_PhysicalCurrent;
}

void DynamicStackAllocator::free(void * ptr)
{
	//assume freeing is in correct order (to check this we could store some meta-information like in the Stack-Allocator)
	size_type bytesToFree = std::distance(static_cast<Byte*>(ptr), m_PhysicalCurrent);
	VirtualMemory::Free(ptr, bytesToFree);
	m_PhysicalCurrent = static_cast<Byte*>(ptr);
}

void DynamicStackAllocator::freeToOS()
{
	Byte* freeFrom = PointerTools::Align(m_PhysicalCurrent, m_GrowingSize);
	size_type bytesToFree = std::distance(m_PhysicalEnd, freeFrom);
	VirtualMemory::Unreserve(static_cast<void*>(freeFrom), bytesToFree);
	m_PhysicalEnd = freeFrom;
}

ENDNAMESPACE
