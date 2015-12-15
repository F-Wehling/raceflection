#pragma once

#include "MemorySystem.h"
#include "RenderSystem/RenderTypes.h"
#include "RenderSystem/RenderHandles.h"

#include "Utilities/UUID.h"
#include "Container/Map.h"

BEGINNAMESPACE

class Window;
class RenderBackend; 
class DeferredRenderer;
class PackageSpec;
class Scene;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();
	bool initialize(RenderEngineTypeFlags engineType = RenderEngineType::OpenGL);
	bool attachWindow(Window* window);
	void shutdown();
	bool tick(float32 dt);

	bool createResourcesFromPackage(PackageSpec* packageSpec);

	inline RenderBackend* getBackend() { return m_RenderBackend; }
    inline Scene* getScene() { return m_Scene; }
private:

private:
	RenderEngineTypeFlags m_EngineType;
	RenderBackend* m_RenderBackend;
	DeferredRenderer* m_Renderer;
	Scene* m_Scene;
private: //The memory for this system
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> RenderSystemAllocator;
	RenderSystemAllocator m_Allocator;


	typedef Map< UUID, GeometryHandle> GeometryHandles_t; //we need a custom allocator here
	typedef Map< UUID, MaterialHandle> MaterialHandle_t; //...
	GeometryHandles_t m_GeometryHandles;
	MaterialHandle_t m_MaterialHandles;
};

ENDNAMESPACE
