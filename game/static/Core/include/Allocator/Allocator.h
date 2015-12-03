#pragma once

BEGINNAMESPACE

class Allocator {
public:
	Allocator();
	Allocator(size_type size);
	
	//Allocator(void* start, size_type size);
	//Allocator(void* start, void* end);

	~Allocator();

	void* getStart() const;
	void* getEnd() const;
	size_type getSize() const;
protected:

	Allocator(const Allocator&) = delete;
	Allocator& operator = (const Allocator&) = delete;

	void initialize(size_type size);
	void initialize(void* start, size_type size);
	void initialize(void* start, void* end);

	Byte* m_Start;
	struct {
		size_type m_Malloced : 1;
		size_type m_Size : 63;
	};
};

ENDNAMESPACE