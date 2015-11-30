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
private:
	const ansichar* m_Name;
};

ENDNAMESPACE