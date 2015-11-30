#pragma once

#include <Allocator/NamedAllocator.h>

BEGINNAMESPACE
namespace policy {
	class NoTagging {
	public:
		inline NoTagging(NamedAllocator*) {}
		inline void tag(void*, size_type) const {}
		inline void untag(void*, size_type) const {}
	};
}

ENDNAMESPACE