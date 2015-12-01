#pragma once

#include <MemorySystem.h>

BEGINNAMESPACE

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();
	bool initialize();
	void shutdown();
	bool tick(float32 dt);

private:

private: //The memory for this system
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> RenderSystemAllocator;
	static const size_type sSystemMemorySize = KILOBYTE(10);
	RenderSystemAllocator m_Allocator;
};

ENDNAMESPACE