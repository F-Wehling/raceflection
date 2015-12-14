#pragma once

BEGINNAMESPACE

template<typename T, typename Allocator>
class StdAllocator {
	template<typename U, typename All>	friend class StdAllocator;
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
    typedef const value_type& const_reference;
	typedef IntOfPtrSize difference_type;
	typedef Allocator underlying_allocator;

public:
	template<typename U>
	struct rebind {
		typedef StdAllocator<U, underlying_allocator> other;
	};

public:
	inline explicit StdAllocator() : m_MemAlloc() {}
	inline explicit StdAllocator(size_type size, uint8 alignment = alignof(value_type), uint8 offset = 0) : m_MemAlloc() {
		m_MemAlloc.initialize(size, alignment, offset);
	}
	inline ~StdAllocator() {}
	inline explicit StdAllocator(StdAllocator const& other) {
		m_MemAlloc = other.m_MemAlloc;		
	}

	template<typename U>
	inline explicit StdAllocator(StdAllocator<U, underlying_allocator>&& other) {
		*this = std::forward<StdAllocator<U, underlying_allocator>>(other);
	}

	template<typename U>
	inline StdAllocator& operator = (StdAllocator<U, underlying_allocator>&& rhs) {
		if (this != &rhs) {
			m_MemAlloc = std::move(rhs.m_MemAlloc);
		}
		return *this;
	}

	inline underlying_allocator& getAllocator() { return m_MemAlloc; }

	inline pointer address(reference r) { return &r; }
	inline const_pointer address(const_reference r) { return &r; }

	inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
		ASSERT(m_MemAlloc.getSize() > 0, "Initialize allocator before using it.");
		return reinterpret_cast<pointer>(m_MemAlloc.allocate(cnt * sizeof(value_type), alignof(value_type)));
	}
	inline void deallocate(pointer p, size_type n) {
		ASSERT(m_MemAlloc.getSize() > 0, "Initialize allocator before using it.");
		m_MemAlloc.free((void*)p);
	}

	inline size_type max_size() const {
		ASSERT(m_MemAlloc.getSize() > 0, "Initialize allocator before using it.");
		return m_MemAlloc.getSize() / sizeof(value_type);
	}

	inline void construct(pointer p, const T& t) { new(p) T(t); }
	inline void destroy(pointer p) { p->~T(); }

	inline bool operator==(Allocator const&) { return true; }
	inline bool operator!=(Allocator const& a) { return !operator==(a); }

private:
	underlying_allocator m_MemAlloc;
};

template<typename T>
class StdAllocator<T, class PoolAllocator> {
	template<typename U, typename All>	friend class StdAllocator;
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
    typedef const value_type& const_reference;
	typedef IntOfPtrSize difference_type;
	typedef PoolAllocator underlying_allocator;

public:
	template<typename U>
	struct rebind {
		typedef StdAllocator<U, underlying_allocator> other;
	};

public:
	inline explicit StdAllocator() : m_MemAlloc() {}
	inline explicit StdAllocator(size_type numObjects, size_type objectSize, uint8 alignment = alignof(T), uint8 offset = 0) {
		m_MemAlloc.initialize(numObjects, objectSize, alignment, offset);
	}
	inline ~StdAllocator() {}
	
	template<typename U>
	inline explicit StdAllocator(StdAllocator<U, underlying_allocator>&& other) {
		*this = std::forward<StdAllocator<U,underlying_allocator>>(other);
	}

	template<typename U>
	inline StdAllocator& operator = (StdAllocator<U,underlying_allocator>&& rhs) {
		if (this != &rhs) {
			m_MemAlloc = std::move(rhs.m_MemAlloc);
		}
		return *this;
	}

	/*
	template<typename U>
	inline explicit StdAllocator(StdAllocator<U, underlying_allocator>& other) {
		*this = other;
	}

	template<typename U>
	inline StdAllocator& operator = (StdAllocator<U, underlying_allocator>& rhs) {
		m_MemAlloc = std::move(rhs.m_MemAlloc);
		return *this;
	}
	//*/

	inline underlying_allocator& getAllocator() { return m_MemAlloc; }

	inline pointer address(reference r) { return &r; }
	inline const_pointer address(const_reference r) { return &r; }

	inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
		ASSERT(m_MemAlloc.getSize() > 0, "Initialize allocator before using it.");
		return reinterpret_cast<pointer>(m_MemAlloc.allocate(cnt * sizeof(value_type), alignof(value_type)));
	}
	inline void deallocate(pointer p, size_type n) {
		ASSERT(m_MemAlloc.getSize() > 0, "Initialize allocator before using it.");
		m_MemAlloc.free((void*)p);
	}

	inline size_type max_size() const {
		ASSERT(m_MemAlloc.getSize(), "Initialize allocator before using it.");
		return m_MemAlloc.getSize() / sizeof(value_type);
	}

	inline void construct(pointer p, const T& t) { new(p) T(t); }
	inline void destroy(pointer p) { p->~T(); }

	inline bool operator==(Allocator const&) { return true; }
	inline bool operator!=(Allocator const& a) { return !operator==(a); }

private:
	underlying_allocator m_MemAlloc;
};

ENDNAMESPACE
