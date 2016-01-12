#pragma once

#include <Utilities/Handle.h>

#include <Container/Map.h>
#include <Container/Array.h>
#include <Delegates/Delegate.h>

#include <RenderSystem/RenderHandles.h>
#include <RenderSystem/RenderTypes.h>

namespace nvFX {
	class IContainer;
	class IUniform;
	class ICstBuffer;
	class IPass;
	class IResource;
}

BEGINNAMESPACE

class Main;
class PackageSpec;

typedef Handle<15, 1> EffectHandle;

INVALID_HANDLE_DECL(EffectHandle);

class EffectSystem {
	typedef Map<String, String> SourceMap_t; //a map containing the source code
	typedef struct {
		nvFX::ICstBuffer* buffer;
		ConstantBufferHandle handle;
	} CstBufRef;
	typedef struct {
		nvFX::IUniform* uniform;
		/*
		UniformHandle handle?! Is a connection to the render backend necessary?!
		*/
	} UniformRef;
	typedef Map < String, CstBufRef> CstBufferMap_t;
	typedef Map < String, UniformRef > UniformMap_t;
public:
	struct RenderTargetStorage {
		RenderTargetLayout layout;
		RenderTargetHandle handle;
	};
private:
	typedef Map < String, RenderTargetStorage > CubeRenderTargets_t;
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

	EffectHandle getFirstPostProcessEffect() const;
	EffectHandle getNextPostProcessEffect(EffectHandle hdl) const;
	EffectHandle getSceneEffectByName(const ansichar* name);
    EffectHandle getMaterialEffectByName(const ansichar* name);
    bool renderSceneEffect(EffectHandle handle, EffectRenderDelegate& fn, uint32 techniqueIdx = 0);
	
	ConstantBufferHandle getViewProjectionBufferHandle() const; 
	ConstantBufferHandle getModelBufferHandle() const;
	ConstantBufferHandle getLightBufferHandle() const;
	ConstantBufferHandle getMaterialBufferHandle() const;

	const RenderTargetStorage& getCubeMapTargetByName(const ansichar* name) const;

	void uploadUniform(const ansichar* name, TextureHandle hdl);
	void uploadUniform(const ansichar* name, int32 iValue);
	nvFX::IUniform* getUniformByName(const ansichar* name);
	void updateUniforms();

	inline bool dirty() const { return m_Dirty; }
	void cleanUp();
	

private:
    bool renderEffect(EffectHandle handle, EffectRenderDelegate& fn, uint32 techniqueIdx, EffectContainers_t& containerMgr);
	EffectHandle getEffectByName(const ansichar* name, EffectContainers_t& containerMgr, uint32 length);
	nvFX::IContainer* createEffect(const ansichar* name, const ansichar* source, const ansichar* type);
	void createGlobalsFromEffectContainer(nvFX::IContainer* effectContainer);
	bool validateAndSetupOverrides();
	void destroyGlobals();
	void reset();

	void createRenderTargetFromResource(nvFX::IResource* resource);
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

	uint32 m_NumOfPostProcessEffects;
	uint32 m_NumOfActivePostProcessEffects;
	uint32 m_NumOfSceneEffects;

	bool m_Dirty;

	//some global shared constant buffers for all effects
	CstBufferMap_t m_ConstantBuffers;
	//... uniforms ...
	UniformMap_t m_Uniforms;
	// Cube map render targets are not suppported by nvFX - so create them manually
	CubeRenderTargets_t m_CubeRenderTargets;

	//to pass through the current render pass
	nvFX::IPass* m_CurrentPass;
};

ENDNAMESPACE
