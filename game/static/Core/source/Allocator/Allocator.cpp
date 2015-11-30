#include <Allocator/Allocator.h>

#include <utility>

BEGINNAMESPACE

Allocator::Allocator() : m_Start(nullptr), m_Size(0) {
}

Allocator::Allocator(void* ptr, void* end){
	initialize(ptr, end);
}

Allocator::Allocator(void* ptr, size_type size){
	initialize(ptr, size);
}

void Allocator::initialize(void* ptr, void* end) {
	m_Start = static_cast<Byte*>(ptr);
	m_Size = std::distance(m_Start, static_cast<Byte*>(end));
}

void Allocator::initialize(void* ptr, size_type size) {
	m_Start = static_cast<Byte*>(ptr);
	m_Size = size;
}

void* Allocator::getStart() const { return m_Start; }

void* Allocator::getEnd() const { return m_Start + m_Size; }

ENDNAMESPACE