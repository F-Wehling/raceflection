#pragma once

#include <Allocator/NamedAllocator.h>

BEGINNAMESPACE

namespace policy {
	class NoTracking {
	public:
		inline NoTracking(NamedAllocator* alloc){}
		inline void onAllocation(void*,size_type, size_type) const {}
		inline void onFree(void*, size_type) const {}
	};

	class IncrementTracking {
	public:
		inline IncrementTracking(NamedAllocator* alloc) : m_AllocationCount(0), m_FreeCount(0), m_Alloc(alloc) {}
		inline void onAllocation(void*, size_type, size_type) { ++m_AllocationCount; }
		inline void onFree(void*, size_type) { ++m_FreeCount; }

		~IncrementTracking() {
			ASSERT(m_AllocationCount == m_FreeCount, "Allcator: %s - Allocate-calls: %d , free-calls: %d", m_Alloc->getName(), (uint32)m_AllocationCount, (uint32)m_FreeCount);
		}
	private:
		NamedAllocator* m_Alloc;
	private:
		std::atomic_uint m_AllocationCount;
		std::atomic_uint m_FreeCount;
	};

	class SizeTracking {
	public:
		inline SizeTracking(NamedAllocator* alloc) : m_Allocated(0), m_Freed(0), m_Alloc(alloc) {}
		inline void onAllocation(void*, size_type size, size_type) { m_Allocated += size; }
		inline void onFree(void*, size_type size) { m_Freed += size; }

		~SizeTracking() {
			ASSERT(m_Allocated == m_Freed, "Allocator %s - Allocated: %d bytes, Freed: %d bytes", m_Alloc->getName(), (uint64)m_Allocated, (uint64)m_Freed);
		}
	private:
		NamedAllocator* m_Alloc;
	private:
#       if OS_WINDOWS
        typedef std::atomic_uint atomic;
#       else
        typedef std::atomic<uint64> atomic;
#       endif

        atomic m_Allocated;
        atomic m_Freed;
	};
}

ENDNAMESPACE
