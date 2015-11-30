#pragma once

BEGINNAMESPACE

class Allocator {
public:
	Allocator();
	Allocator(void* start, size_type size);
	Allocator(void* start, void* end);

	void* getStart() const;
	void* getEnd() const;
protected:

	Allocator(const Allocator&) = delete;
	Allocator& operator = (const Allocator&) = delete;

	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	Byte* m_Start;
	size_type m_Size;
};

ENDNAMESPACE