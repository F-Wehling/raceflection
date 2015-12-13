#include <Allocator/Allocator.h>
#include <Memory/Malloc.h>

#include <utility>

BEGINNAMESPACE

Allocator::Allocator() : m_Start(nullptr), m_Size(0), m_Malloced(0){}

Allocator::Allocator(Allocator && rhs) {
	*this = std::forward<Allocator>(rhs);
}

Allocator& Allocator::operator = (Allocator&& rhs) {
	if (this != &rhs) {
		m_Size = rhs.m_Size;
		m_Start = rhs.m_Start;
		m_Malloced = rhs.m_Malloced;
		rhs.m_Size = 0;
		rhs.m_Start = nullptr;
		rhs.m_Malloced = 0;
	}
	return *this;
}


Allocator::~Allocator() {
	if (m_Malloced) 
		Malloc::Free((void*)m_Start);
}

Allocator::Allocator(size_type size) : Allocator() {
	initialize(size);
}
/*
Allocator::Allocator(void* ptr, void* end) : Allocator(){
	initialize(ptr, end);
}

Allocator::Allocator(void* ptr, size_type size) {
	initialize(ptr, size);
}
*/

void Allocator::initialize(void* ptr, void* end) {
	m_Start = static_cast<Byte*>(ptr);
	m_Size = std::distance(m_Start, static_cast<Byte*>(end));
}

void Allocator::initialize(size_type size) {
	//We allocate the memory
	void* ptr = (Byte*)Malloc::Alloc(size, alignof(Byte), 0);
	m_Malloced = 1;
	initialize(ptr, size);
}

void Allocator::initialize(void* ptr, size_type size) {
	m_Start = static_cast<Byte*>(ptr);
	m_Size = size;
}

void* Allocator::getStart() const { return m_Start; }

void* Allocator::getEnd() const { return m_Start + m_Size; }

size_type Allocator::getSize() const
{
	return m_Size;
}

ENDNAMESPACE