#pragma once

BEGINNAMESPACE

namespace detail {
	template<size_type ChunkSize, bool GrEqPointerSize = true>
	class __FreeList {
	public:
		static const size_type ElementSize = ChunkSize;

		inline void* get() {
			if (m_Next == nullptr) return nullptr;
			__FreeList ptr = m_Next;
			m_Next = ptr->m_Next;
			return ptr;
		}

		inline void free(void* ptr) {
			__FreeList* head = static_cast<__FreeList*>(ptr);
			head->m_Next = m_Next;
			m_Next = head;
		}

		inline void initialize(void* start, void* end) {
			union {
				void* as_void;
				Byte* as_byte;
				__FreeList* as_this;
			};
			
			m_Next = as_this;
			as_Byte += ElementSize;

			__FreeList* it = m_Next;
			while (it < end) {
				it->m_Next = as_this;
				it = as_this;
				as_byte += ElementSize;
			}
			it->m_Next = nullptr;
		}
	private:
		__FreeList* m_Next;
	};

	template<size_type ChunkSize>
	class __FreeList<ChunkSize, false> {
	public:
		typedef typename IntegerOfSize<ChunkSize>::u_type Next_t;
		static const size_type ElementSize = ChunkSize;
		static const size_type MaxNumELements = std::numeric_limits< typename IntegerOfSize<ElementSize>::s_type >::max();
	public:
		inline void* get() {
			if (m_Next >= m_NumElements) return nullptr;
			Next_t* head = m_Start[m_Next];
			m_Next = *head;
			return static_cast<void*>(head);
		}

		inline void free(void* ptr) {
			Next_t* head = static_cast<Next_t*>(ptr);
			*head = m_Next;
			m_Next = std::distance(m_Start, head);
		}

		inline void initialize(void* start, void* end) {
			m_NumElements = std::distance(start, end);
			assert(m_NumElements > MaxNumElements && "To much desired elements in FreeList for this ElementSize");
			union {
				void* as_void;
				Byte* as_byte;
				Next_t* as_next;
			};			
			for (size_type i = 0; i < numElements; ++i) {
				m_Start[i] = i + 1;
			}			
		}
	private:
		Next_t* m_Start;
		Next_t m_Next;
		Next_t m_NumElements;
	};
}

template<size_type ChunkSize>
using FreeList = detail::__FreeList<ChunkSize, ChunkSize >= sizeof(IntOfPtrSize)>;


ENDNAMESPACE