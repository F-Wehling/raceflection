#pragma once

#include <Delegates/Delegate.h>
#include <CompileTime/Templates.h>
#include <Allocator/PoolAllocator.h>

#include <Delegates/Combiner.h>

BEGINNAMESPACE

template<typename Signature, size_type Size = 5, typename Combiner = combiner::LastResult< typename Result_Of<Signature>::result_type>> class DelegatePool {
	void invalid(){}
};

template<typename R, typename ...Args, size_type Size, typename Combiner>
class DelegatePool< R(Args...), Size, Combiner > {
public:
	typedef Delegate<R(Args...)> Delegate_t;
	typedef typename Delegate_t::result_type result_type;
	typedef Combiner combiner_type;
private:
	//
	/// private combiner class
	/// just to combe up with void returning callbacks
	template<typename ResultType>
	struct combiner {
		inline combiner() {}
		template<typename ...Args>
		inline void operator()(const Delegate_t& f, Args&&... args) {
			m_combiner(f.invoke(std::forward<Args>(args)...));
		}
		inline ResultType get() const {
			return m_combiner.get();
		}
		combiner_type m_combiner;
	};
	template<>
	struct combiner<void> {
		inline combiner() {}
		template<typename ...Args>
		inline void operator()(const Delegate_t& f, Args&&... args) {
			f(std::forward<Args>(args)...);
		}
		inline void get() const {}
	};
public:
	DelegatePool() {
		m_MemoryManager.initialize(sizeof(Delegate_t), alignof(Delegate_t), m_Delegates, m_Delegates + Size);
	}
	
	void attach(const Delegate<R(Args...)>& del) {
		Delegate_t* delegator = ((Delegate_t*)m_MemoryManager.allocate(sizeof(Delegate_t), alignof(Delegate_t), 0));
		ASSERT(delegator != nullptr, "Unable to connect another delegate to this DelegatePool");
		*delegator = del;
	}

	template< R(*Functor)(Args...) >
	void attach() {
		Delegate_t* delegator = ((Delegate_t*)m_MemoryManager.allocate(sizeof(Delegate_t), alignof(Delegate_t), 0));
		ASSERT(delegator != nullptr, "Unable to connect another delegate to this DelegatePool");
		delegator->bind<Functor>();
	}

	template< typename C, R(C::*Functor)(Args...) >
	void attach() {
		Delegate_t* delegator = ((Delegate_t*)m_MemoryManager.allocate(sizeof(Delegate_t), alignof(Delegate_t), 0));
		ASSERT(delegator != nullptr, "Unable to connect another delegate to this DelegatePool");
		delegator->bind<C, Functor>();
	}

	template< typename C, R(C::*Functor)(Args...) >
	void attach(C* instance) {
		Delegate_t* delegator = ((Delegate_t*)m_MemoryManager.allocate(sizeof(Delegate_t), alignof(Delegate_t), 0));
		ASSERT(delegator != nullptr, "Unable to connect another delegate to this DelegatePool");
		delegator->bind<C, Functor>(instance);
	}

	void detach(const Delegate<R(Args...)>& del) {
		m_MemoryManager.free(del);
	}

	result_type signal(Args... args) const {
		combiner<result_type> combiner;
		for (size_type i = 0; i < Size; ++i) {
			if (m_Delegates[i]) {
				combiner(m_Delegates[i],std::forward<Args>(args)...);
			}
		}
		return combiner.get();
	}

private:
	PoolAllocator m_MemoryManager;
	Delegate_t m_Delegates[Size];
};

#define DECLARE_EVENT( name, ... ) \
	struct name ## Event { \
		DECLARE_MEM_EXCLUSIVE_FLAGS(name ## Event, __VA_ARGS__); \
		name ## Event ## Flags type; \
		const void* data; \
		\
		static name ## Event Generate(name ## Event ## Flags type) { \
			name ## Event e = {type, nullptr };	\
			return e; \
		} \
		template<typename T> \
		static name ## Event Generate(name ## Event ## Flags type, const T* data) { \
			name ## Event e = {type, (const void*)data };	\
			return e; \
		} \
		template<typename T> \
		static name ## Event Generate(name ## Event ## Flags type, const T& data) { \
			name ## Event e = {type, (const void*)&data };	\
			return e; \
		} \
	}


#define DECLARE_QUERY( name, ... ) \
	struct name ## Query { \
		DECLARE_MEM_EXCLUSIVE_FLAGS(name ## Query, __VA_ARGS__); \
		name ## Query ## Flags type; \
		const void* data; \
		\
		static name ## Query Generate(name ## Query ## Flags type) { \
			name ## Query q = {type, nullptr };	\
			return q; \
		} \
		template<typename T> \
		static name ## Query Generate(name ## Query ## Flags type, const T* data) { \
			name ## Query q = {type, (const void*)data };	\
			return q; \
		} \
		template<typename T> \
		static name ## Query Generate(name ## Query ## Flags type, const T& data = T()) { \
			name ## Query q = {type, (const void*)&data };	\
			return q; \
		} \
	}


ENDNAMESPACE