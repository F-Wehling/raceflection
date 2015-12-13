#pragma once

BEGINNAMESPACE

class NamedAllocator {
public:
	NamedAllocator(const ansichar* allocatorName) :
		m_Name(allocatorName){
	}
	inline const ansichar* getName() const { 
		return m_Name; 
	}

	inline NamedAllocator(NamedAllocator&& other) {
		*this = std::forward<NamedAllocator>(other);
	}

	inline NamedAllocator& operator = (NamedAllocator&& rhs) {
		if (this != &rhs) {
			m_Name = rhs.m_Name;
			rhs.m_Name = nullptr;
		}
		return *this;
	}

private:
	const ansichar* m_Name;
};

ENDNAMESPACE