#pragma once

#include <Allocator/NamedAllocator.h>

BEGINNAMESPACE

namespace policy {
	class NoBoundsChecking {
	public:
		inline NoBoundsChecking(NamedAllocator*) {}
		inline static constexpr const int32 SizeFront() { return 0; }
		inline static constexpr const int32 SizeBack() { return 0; }

		inline void front(void*, size_type) const {}
		inline void back (void*) const {}

		inline size_type check(const void*) const { return 0; }
	};

	class PatternBoundsChecking {
	public:
		inline PatternBoundsChecking(NamedAllocator* alloc) : m_Alloc(alloc) {}
		inline static constexpr const int32 SizeFront() { return sizeof(uint32) + sizeof(size_type); }
		inline static constexpr const int32 SizeBack() { return sizeof(size_type); }

		inline void front(void* ptr, size_type size) const {
			union {
				void* as_void;
				Byte* as_byte;
				uint32 * as_uint32;
				size_type* as_size;
			};
			as_void = ptr;
			*as_uint32 = PATTERN_FRONT;
			as_byte += sizeof(uint32);
			*as_size = size;
		}
		inline void back(void* ptr) const {
			union {
				void* as_void;
				uint32 * as_uint32;
			};
			as_void = ptr;
			*as_uint32 = PATTERN_BACK;
		}

		inline size_type check(const void* ptr) const {
			union {
				const void* as_void;
				const Byte* as_byte;
				const uint32* as_uint32;
				const size_type* as_size;
			};
			as_void = ptr;
			ASSERT(*as_uint32 == PATTERN_FRONT, "Allocator %s - Front-pattern mismatch during boundary check. (at %p)", m_Alloc->getName(), ptr);
			as_byte += sizeof(uint32);
			size_type mem_size = *as_size;
			as_byte += sizeof(size_type) + mem_size;
			ASSERT(*as_uint32 == PATTERN_BACK, "Allocator %s - Back-pattern mismatch during boundary check. (at %p)", m_Alloc->getName(), ptr);
			return mem_size;
		}
	private:
		NamedAllocator* m_Alloc;
	private:
		static const uint32 PATTERN_FRONT = uint32(0xD0D0D0D0);
		static const uint32 PATTERN_BACK = uint32(0x0E0E0E0E);
	};
};

ENDNAMESPACE