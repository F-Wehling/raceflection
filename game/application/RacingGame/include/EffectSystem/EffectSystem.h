#pragma once

#include <Utilities/Handle.h>

#include <Container/Map.h>
#include <Container/Array.h>
#include <Delegates/Delegate.h>

#include <RenderSystem/RenderHandles.h>

namespace nvFX {
	class IContainer;
	class ICstBuffer;
}

BEGINNAMESPACE

class Main;
class PackageSpec;

typedef Handle<15, 1> EffectHandle;

INVALID_HANDLE_DECL(EffectHandle);

class EffectSystem {
	typedef Map<String, String> SourceMap_t; //a map containing the source code
	typedef DynArray < nvFX::IContainer* > EffectContainers_t;
public:
	struct EffectRenderDelegate {
		Delegate<bool()> sceneGraphShaded;
		Delegate<bool()> sceneGraphNoShading;
		Delegate<bool()> fullscreenQuad;
		Delegate<bool()> sceneGraphShadedOpaqueOnly;
		Delegate<bool()> sceneGraphShadedTransparentOnly;
		Delegate<bool()> sceneGraphNoShadingOpaqueOnly;
		Delegate<bool()> sceneGraphNoShadingTransparentOnly;
		Delegate<bool()> sceneGraphOutliesOnly;
		Delegate<bool()> sceneGraphDebugQuad0;
		Delegate<bool()> sceneGraphDebugQuad1;
		Delegate<bool()> sceneGraphDebugQuad2;
		Delegate<bool()> sceneGraphDebugQuad3;
		Delegate<bool()> sceneGraphDebugScene;
		Delegate<bool()> sceneGraphDebugSceneLines;
		Delegate<bool()> doNothing;
		Delegate<bool()> optixReflection;
		Delegate<bool()> optixShadow;
		Delegate<bool()> optixReflectionAndShadow;
		Delegate<bool()> optix;
		Delegate<bool()> cuda;
		Delegate<bool()> glslCompute;
		Delegate<bool()> custom0;
		Delegate<bool()> custom1;
		Delegate<bool()> custom2;
		Delegate<bool()> custom3;
		Delegate<bool()> custom4;
		Delegate<bool()> custom5;
		Delegate<bool()> custom6;
		Delegate<bool()> custom7;
		Delegate<bool()> custom8;
		Delegate<bool()> custom9;
		Delegate<bool()> stencilFillPath;
		Delegate<bool()> stencilStrokePath;
		Delegate<bool()> coverFillPath;
		Delegate<bool()> coverStrokePath;
		Delegate<bool()> undefined;
		Delegate<bool()> error;
	};
public:
	EffectSystem(Main* mainRef);

	bool initialize(uint32 api);
	void shutdown();

	bool createMaterialEffect(const ansichar* name, const ansichar* effectSource);
	bool createSceneEffect(const ansichar* name, const ansichar* effectSource);

	void createEffectLibraryFromPackageSpec(const PackageSpec* spec);
	void updateEffectLibraryFromPackageSpec(const PackageSpec* spec);

	EffectHandle getSceneEffectByName(const ansichar* name);
	EffectHandle getMaterialEffectByName(const ansichar* name);
	bool renderSceneEffect(EffectHandle handle, EffectRenderDelegate& fn);
	

	void uploadViewProjectionMatrices(const void* data);
	void uploadModelMatrices(const void* data);

	inline ConstantBufferHandle getViewProjectionBufferHandle() const { return m_ViewProjMatHandle; }
	inline ConstantBufferHandle getModelBufferHandle() const { return m_ModelMatHandle; }
	
private:
	bool renderEffect(EffectHandle handle, EffectRenderDelegate& fn, EffectContainers_t& containerMgr);
	EffectHandle getEffectByName(const ansichar* name, EffectContainers_t& containerMgr, uint32 length);
	nvFX::IContainer* createEffect(const ansichar* name, const ansichar* source, const ansichar* type);
	void createGlobalsFromEffectContainer(nvFX::IContainer* effectContainer);
	bool validateAndSetupOverrides();
	void destroyGlobals();
	void reset();
private:
	static void nvFXErrorCallback(const ansichar* error);
	static void nvFXMessageCallback(const ansichar* message);
	static void nvFXIncludeCallback(const ansichar* includeName, FILE*& fp, const ansichar* &buf);

private:
	Main* m_MainRef;
	uint32 m_RenderAPI;

	SourceMap_t m_HeaderCode;
	SourceMap_t m_SceneFXSourceCode;
	SourceMap_t m_MaterialFXSourceCode;
	EffectContainers_t m_MaterialEffectContainer;
	EffectContainers_t m_SceneEffectContainer;
	uint32 m_CurrentNumOfMaterialEffects;
	uint32 m_CurrentNumOfSceneEffects;

	//some global shared constant buffers for all effects
	nvFX::ICstBuffer* m_ViewProjectionMatrices;
	ConstantBufferHandle m_ViewProjMatHandle;

	nvFX::ICstBuffer* m_ModelMatrices;
	ConstantBufferHandle m_ModelMatHandle;
};

ENDNAMESPACE
