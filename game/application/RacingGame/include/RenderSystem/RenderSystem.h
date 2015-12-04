#pragma once

#include "MemorySystem.h"
#include "RenderSystem/RenderTypes.h"

BEGINNAMESPACE

class Window;
class RenderBackend; 
class DeferredRenderer;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();
	bool initialize(RenderEngineTypeFlags engineType = RenderEngineType::OpenGL);
	bool attachWindow(Window* window);
	void shutdown();
	bool tick(float32 dt);

	inline RenderBackend* getBackend() { return m_RenderBackend; }

private:

private:
	RenderEngineTypeFlags m_EngineType;
	RenderBackend* m_RenderBackend;
	DeferredRenderer* m_Renderer;
private: //The memory for this system
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> RenderSystemAllocator;
	RenderSystemAllocator m_Allocator;
};

ENDNAMESPACE