#pragma once

#include <Utilities/Number.h>

BEGINNAMESPACE

template<typename Work, size_type NumEntries>
class WorkStealingQueue {
	typedef Work value_type;
private:
	static const size_type Size = NumEntries;
	static const size_type MASK = Size - 1;
	static_assert(Number::IsPowerOf2(NumEntries), "The Work-Stealing Queue size must be a power-of-2");
public:
	WorkStealingQueue() : m_Bottom(0), m_Top(0) {}
	
	void push(const Work& work) {
		size_type b = m_Bottom;
		m_Values[b & MASK] = work;

		//COMPILER_BARRIER;

		m_Bottom = b + 1;
	}

	value_type steal() {
		size_type t = m_Top;

		//COMPILER_BARRIER;

		size_type b = m_Bottom;
		if (t < b) {
			value_type v = m_Values[t & MASK];
			
			if (!m_Top.compare_exchange_strong(t, t + 1)) {
				return value_type();
			}
			return v;
		}
		return value_type();
	}

	value_type pop(void)
	{
		size_type b = m_Bottom - 1;
		m_Bottom = b;

		MEMORY_BARRIER;
		
		size_type t = m_Top;
		if (t <= b)
		{
			value_type v = m_Values[b & MASK];
			if (t != b)
			{
				return v;
			}

			if (!m_Top.compare_exchange_strong(t, t + 1))
			{
				v = value_type();
			}

			m_Bottom = t + 1;
			return v;
		}
		m_Bottom = t;
		return value_type();
	}

	size_type capacity() const { return Size; }

private:
	value_type m_Values[Size];
	size_type m_Bottom;
	std::atomic<size_type> m_Top;
};


ENDNAMESPACE