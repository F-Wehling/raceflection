#pragma once

BEGINNAMESPACE

namespace combiner {

	template<typename ResultType>
	struct LastResult {
		typedef ResultType result_type;
		inline void operator()(typename std::add_rvalue_reference<result_type>::type val) {
			m_Result = val;
		}
		inline result_type get() const {
			return m_Result;
		}
	private:
		result_type m_Result;
	};
	template<>
	struct LastResult<void> {
		typedef void result_type;
		inline void operator()(void) {}
		result_type get() const {}
	};

	template<typename ResultType, ResultType initial>
	struct AndResult {
		typedef ResultType result_type;
		inline AndResult() : m_Result(initial) {}
		inline void operator()(typename std::add_rvalue_reference<result_type>::type val) {
			m_Result = m_Result && val;
		}
		inline result_type get() const {
			return m_Result;
		}
	private:
		ResultType m_Result;
	};

	template<typename ResultType, ResultType initial>
	struct OrResult {
		typedef ResultType result_type;
		inline OrResult() : m_Result(initial) {}
		inline void operator()(typename std::add_rvalue_reference<result_type>::type val) {
			m_Result = m_Result || val;
		}
		inline result_type get() const {
			return m_Result;
		}
	private:
		ResultType m_Result;
	};

}


ENDNAMESPACE