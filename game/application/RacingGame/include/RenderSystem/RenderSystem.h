#pragma once

#include "MemorySystem.h"
#include "RenderSystem/DeferredRenderer.h"

BEGINNAMESPACE

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();
	bool initialize();
	void shutdown();
	bool tick(float32 dt);

private:
	DeferredRenderer m_Renderer;
private: //The memory for this system
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> RenderSystemAllocator;
	RenderSystemAllocator m_Allocator;
};

ENDNAMESPACE