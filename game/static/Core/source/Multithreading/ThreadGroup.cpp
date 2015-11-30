#include <Multithreading/ThreadGroup.h>

BEGINNAMESPACE

ThreadGroup::ThreadGroup()
{}

ThreadGroup::~ThreadGroup()
{
	joinAll();
}

void ThreadGroup::joinAll()
{
	for (size_type i = 0; i < m_Threads.size(); ++i) {
		if (m_Threads[i].joinable())
			m_Threads[i].join();
	}
}

ENDNAMESPACE

