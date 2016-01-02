#pragma once

#include "MemorySystem.h"
#include "RenderSystem/RenderTypes.h"
#include "RenderSystem/RenderHandles.h"

#include "Utilities/UUID.h"
#include "Container/Map.h"
#include "Container/Array.h"

BEGINNAMESPACE

class Main;
class Window;
class RenderBackend; 
class DeferredRenderer;
class PackageSpec;
class Scene;
struct Material;

class RenderSystem {
public:
	RenderSystem(Main* mainRef);
	~RenderSystem();
	bool initialize(RenderEngineTypeFlags engineType = RenderEngineType::OpenGL);
	bool attachWindow(Window* window);
	void shutdown();
	bool tick(float32 dt);

	inline RenderEngineTypeFlags getEngineType() const { m_EngineType; }

	bool createResourcesFromPackage(PackageSpec* packageSpec);
	bool updateResourcesFromPackage(PackageSpec* packageSpec);

	inline RenderBackend* getBackend() { return m_RenderBackend; }
    inline Scene* getScene() { return m_Scene; }
	inline Main* getMainRef() { return m_MainRef; }
private:

private:
	Main* m_MainRef;
	RenderEngineTypeFlags m_EngineType;
	RenderBackend* m_RenderBackend;
	DeferredRenderer* m_Renderer;
	Scene* m_Scene;
private: //The memory for this system
	typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> RenderSystemAllocator;
	RenderSystemAllocator m_Allocator;

	typedef Map< UUID, GeometryHandle> GeometryHandles_t; //we need a custom allocator here
	typedef Map< UUID, MaterialHandle> MaterialHandle_t; //...
	typedef Map< UUID, TextureHandle> TextureHandle_t; //...
	GeometryHandles_t m_GeometryHandles;
	MaterialHandle_t m_MaterialHandles;
	TextureHandle_t m_TextureHandles;

	//Material
	RenderSystemAllocator m_MaterialAllocator;
	DynArray<Material*> m_Materials;
	uint32 m_NumberOfMaterials;
};

ENDNAMESPACE
