#pragma once

#include <thread>
#include <Container/Array.h>

BEGINNAMESPACE

class ThreadGroup {

public:
	ThreadGroup();
	~ThreadGroup();
	void joinAll();

	template<typename F>
	void attach(F func) {
		m_Threads.emplace_back(std::thread(func));
	}

private:
	typedef DynArray<std::thread> Threads;
	Threads m_Threads;
};

ENDNAMESPACE