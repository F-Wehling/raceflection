#pragma once

#include <winkSignal.h>
#include <Container/Array.h>
#include <Delegates/Combiner.h>

BEGINNAMESPACE



//
/// Delegates
/*
namespace delegates { //own delegate implementation
	
	template<typename Signature> class DelegateSignature{};
	template<typename Result, typename... Args> 
	class DelegateSignature<Result(Args...)>{
		typedef uint8 MemSignature[MemberFunction::MEMBERFUNC_SIZE];
	public:
		typedef Result result_type;
		typedef result_type(*Signature)(Args...);
		typedef void class_type;
	public:
		DelegateSignature(Signature func) m_Function(func){}

		template<typename class_t>
		DelegateSignature(result_type(class_t::* func)(Args...)) {
			m_MemFunction = reinterpret_cast<MemSignature>(func);
		}

	public:
		DelegateSignature():m_HasMemFunc(false){}
		template<typename... Args> inline result_type operator()(Args&&... args) { 
			return m_HasMemFunc ? executeMem<typename NthType<0, Args...>::type>(std::forward<Args>(args)...) : executeStat(std::forward<Args>(args)...);
		}
	private:
		template<typename ...Args> inline result_type executeStat(Args&&... args) {
			return (*m_Function)(args);
		}
		template<typename class_type,typename... Args> inline result_type executeMem(class_type* obj, Args&&... args) { 
			typedef result_type(class_type::*MemSignature)(Args...);
			return (obj->*reinterpret_cast<MemSignature>(m_MemFunction))(std::forward<Args>(args)...);
		}
	private:
		union {
			Signature m_Function;
			MemSignature m_MemFunction;
		};
		bool m_HasMemFunc;
	};
	template<typename Result, typename...Args>	
	class DelegateSignature<Result(*)(Args...)> : public DelegateSignature<Result(Args...)> {};

	template<typename Result, typename Class_t, typename...Args> 
	class DelegateSignature<Result(Class_t::*)(Args...)>{
	public:
		typedef Result result_type;
		typedef Class_t class_type;
		typedef result_type(class_type::*Signature)(Args...);
	public:
		DelegateSignature() :m_HasMemFunc(true) {}
		template<typename... Args> inline result_type operator()(Args&&... args) { return execute(std::forward<Args>(args)...); }
	private:
		template<typename... Args> inline result_type execute(class_type* obj, Args&... args) { return (obj->*m_Function)(args); }
	private:
		Signature m_Function;
		bool m_HasMemFunc;
	};
	template<typename Result, typename Class_t, typename...Args> 
	class DelegateSignature<Result(Class_t::*)(Args...)const>{
		typedef Result result_type;
		typedef Class_t class_type;
		typedef result_type(class_type::*Signature)(Args...) const;
	public:
		template<typename... Args> inline result_type operator()(Args&&... args) { return execute(std::forward<Args>(args)...); }
	private:
		template<typename... Args> inline result_type execute(class_type* obj, Args&... args) { return (obj->*m_Function)(args); }
	private:
		Signature m_Function;
		bool m_HasMemFunc;
	};


	template<typename Signature>
	class Delegate : public DelegateSignature<Signature>{
		typedef DelegateSignature<Signature> Parent;
	public:
		Delegate(Signature func) : m_Function(func) {
			m_HasMemFunc = If_Else<std::is_void<class_type>::value, bool, true, false>;
			m_Function = func;
		};

		template<uint32 type>
		struct FunctionCreator {};

		template<>
		struct FunctionCreator<0> {
			//it is a free function

		};

		template<>
		struct FunctionCreator<1> {
			//it is a member function
		};

		template<typename F>
		Delegate(F func) {
			static_assert(std::is_void<class_type>::value, "A memberfunction has to be bound to this delegate");
			FunctionCreator< Function_Traits<F>::IsMemberFunction ? 1 : 0> creator(m_Function, m_MemFunction);
		}
	};
}
*/
///
//

//
/// signal slot
namespace signal_slot {

	template<typename F>
	using Slot = std::function<F>;

	/*
	template <typename Signature>
	class Slot  {
	private:
		typedef typename Function_Traits<Signature>::result_type result_type;
		typedef Slot<Signature> this_type;
		typedef std::function<Signature> impl_delegate;
	public:		
		/// Construct a slot with no call-back
		Slot(){}

		/// Construct a slot with a static/global function call-back
		/// \param fn The static/global function
		Slot(Signature fn) : _delegate(fn){}
		
		template<typename F> Slot(F func) {
			_delegate = std::bind(func);
		}
		
		/// Copy constructor
		Slot(const this_type& slot)
			: _delegate(slot._delegate)
		{}

		/// Destructor
		~Slot() {}

		/// Assignment operator
		/// \param slot The slot you wish to assign to
		/// \return *this
		this_type& operator=(const this_type& slot)
		{
			_delegate = slot._delegate;
			return *this;
		}

		/// Calls the slot
		/// \param args Any arguments you want to pass to the slot
		template <class ...Args>
		typename result_type operator()(Args&&... args) const
		{
			return _delegate(std::forward<Args>(args)...);
		}


		// comparision operators for sorting and comparing
		bool operator==(const this_type& slot) const {	return _delegate == slot._delegate; }
		bool operator!=(const this_type& slot) const { return !operator==(slot); }
		bool operator<(const this_type& slot) const	{ return _delegate < slot._delegate; }
		bool operator>(const this_type& slot) const { return slot._delegate > _delegate; }
		bool operator<=(const this_type& slot) const { return !operator>(slot);	}
		bool operator>=(const this_type& slot) const { return !operator<(slot); }
	private:
		impl_delegate _delegate;
	};
	*/

	template <class Signature, typename Combiner = combiner::LastResult<typename Result_Of<Signature>::result_type>>
	class Signal
	{
	private:
		typedef Slot<Signature> slot_type;
		typedef Signal<slot_type, Combiner> this_type;
		typedef Combiner combiner_type;
		typedef typename combiner_type::result_type result_type;

		/// defines an array of slots
		typedef DynArray<slot_type> slot_array;

		//
		/// private combiner class
		/// just to combe up with void returning callbacks
		template<typename ResultType>
		struct combiner {
			inline combiner() {}
			template<typename ...Args>
			inline void operator()(typename slot_array::const_iterator i, Args&&... args) {
				m_combiner((*i)(std::forward<Args>(args)...));
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
			inline void operator()(typename slot_array::const_iterator i, Args&&... args) {
				(*i)(std::forward<Args>(args)...);
			}
			inline void get() const {}
		};

	public:
		/// Connects a slot to the signal
		void connect(const slot_type& slot) { _slots.push_back(slot); }
				
		/// Disconnects a slot from the signal
		/// \param args The arguments you wish to construct a slot with
		template <typename... Args>
		void disconnect(Args&&... args) { _slots.erase(std::find(_slots.begin(), _slots.end(), slot_type(std::forward<Args>(args)...))); }

		/// Emits the events you wish to send to the call-backs
		/// \param args The arguments to emit to the slots connected to the signal
		template <class ...Args>
		result_type emit(Args&&... args) const
		{
			combiner<result_type> combiner;
			for (typename slot_array::const_iterator i = _slots.begin(); i != _slots.end(); ++i)
			{
				combiner(i, std::forward<Args>(args)...);
			}
			return combiner.get();
		}

		/// Emits events you wish to send to call-backs
		/// \param args The arguments to emit to the slots connected to the signal
		/// \note
		/// This is equvialent to emit.
		template <class ...Args>
		result_type operator()(Args&&... args) const { return emit(std::forward<Args>(args)...); }

		// comparision operators for sorting and comparing
		bool operator==(const this_type& signal) const { return _slots == signal._slots; }
		bool operator!=(const this_type& signal) const { return !operator==(signal); }

	private:
		/// The slots connected to the signal
		slot_array _slots;
	};

}

template<typename Signature, typename Combiner = combiner::LastResult < Result_Of<Signature>::result_type >>
using Signal =  signal_slot::Signal<Signature, Combiner>;

template<typename Signature>
using Slot = signal_slot::Slot<Signature>;

using namespace::std::placeholders;

ENDNAMESPACE


/*

	//
	/// Delegates for general functions
	template<typename Result, typename ...Args>
	class Delegate<Result(Args...)> : public DelegateInterface {
	public:
		typedef typename Result result_type;
		typedef result_type(*Signature)(Args...);
	public:
		Delegate(Signature func) : m_Function(func) {}
		
		template<typename ...Args>
		result_type operator()(Args&&... args) {
			return (*m_Function)(std::forward<Args>(args)...);
		}

	private:
		Signature m_Function;
	};

	template<typename Result, typename ...Args>
	class Delegate<Result(*)(Args...)> : public DelegateInterface {
	public:
		typedef Result(*Signature)(Args...);
		typedef Result result_type;
	public:
		Delegate(Signature func) : m_Function(func) {}

		template<typename ...Args>
		result_type operator()(Args&&... args) {
			return (*m_Function)(std::forward<Args>(args)...);
		}

	private:
		Signature m_Function;
	};

	//
	/// Delegates for class function pointer
	template<typename Result, typename Class, typename ...Args>
	class Delegate<Result(Class::*)(Args...)> : public DelegateInterface {
	public:
		typedef Result result_type;
		typedef Class class_type;
		typedef result_type(class_type::*Signature)(Args...);
	public:
		Delegate(Signature func) : m_Function(func) {}

		template<typename ...Args>
		result_type operator()(Args&&... args) {
			return execute(std::forward<Args>(args)...);
		}
	private:
		Signature m_Function;
	};
	template<typename Result, typename Class, typename ...Args>
	class Delegate<Result(Class::*)(Args...) const> : public DelegateInterface {
	public:
		typedef Result result_type;
		typedef Class class_type;
		typedef result_type(class_type::*Signature)(Args...) const;
	public:
		Delegate(Signature func) : m_Function(func) {}

		template<typename ...Args>
		result_type operator()(const class_type* obj, Args&&... args) {
			return (std::forward<Args>(args)...);
		}
	private:
		Signature m_Function;
	};*/