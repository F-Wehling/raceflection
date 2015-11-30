#pragma once

#include <Allocator/NamedAllocator.h>

BEGINNAMESPACE

namespace policy {
	class NoSync {
	public:
		inline NoSync(NamedAllocator*) {}
		inline void lock() const {}
		inline void unlock() const {}
	};
}

ENDNAMESPACE