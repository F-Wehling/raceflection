#pragma once

#include "RenderSystem/RenderBucket.h"
#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/ConstantBuffer.h"

#include "EffectSystem/EffectSystem.h"

#include <Math/Math.h>

BEGINNAMESPACE

class RenderSystem;
class Scene;
struct SceneNode;

#if OS_LINUX
struct BoundingBox;
#endif

class DeferredRenderer {
public:
	struct GBufferKey {
		struct _GenGBufferKey{
			static const size_type MatIdxCount =  MaterialHandle::IndexBitCount;
			static const size_type DepthCount = 24;
			static const size_type PassCount = 4;
			static const size_type PreSortCount = (sizeof(uint64) * 8 - MatIdxCount - DepthCount - PassCount) / 2;
			static const size_type PostSortCount = (sizeof(uint64) * 8 - MatIdxCount - DepthCount - PassCount - PreSortCount);

			uint64 preSort : PreSortCount;
			uint64 material : MatIdxCount;
			uint64 depth : DepthCount;
			uint64 pass : PassCount;
			uint64 postSort : PostSortCount;
		};
		uint64 _key;
		static GBufferKey Generate();
		static GBufferKey Generate(float32 depth, MaterialHandle matHandle, uint8 pass);
		static void Decode(GBufferKey key, MaterialHandle& matHandle);
	};
private:
public:
	DeferredRenderer(RenderSystem* renderSystem);
	~DeferredRenderer();

	bool renderModeNotImplemented();

	bool initialize();
	void render(float32 dt, Scene* scene);
	void shutdown();
private:
	bool initializeScene(Scene* scene);

	void uploadMaterial(MaterialHandle hdl);
	void renderSceneNode(const SceneNode* sceneNode);

    bool render_doNothing();
    bool render_fullScreenQuad();
    bool renderSceneGraphShaded();
    bool render_ReflectionsFront();
    bool render_ReflectionsBack();
    bool render_ReflectionsLeft();
    bool render_ReflectionsRight();
    bool render_ReflectionsTop();
    bool render_ReflectionsDown();

private:
	static void RenderSceneNode(const SceneNode* sceneNode, uint32 count, void* instance);

private:
	RenderSystem* m_RefRenderSys;

	Scene* m_RenderScene;
	ViewProjectionMatrices m_RenderViewProjectionMatrices;

	EffectSystem* m_EffectSystemRef;
	EffectHandle m_DeferredRenderingEffect;
	EffectSystem::EffectRenderDelegate m_EffectRenderDelegates;

	ConstantBufferHandle m_ViewProjectionMatrixBufferHandle;
	ConstantBufferHandle m_ModelMatrixBufferHandle;
	ConstantBufferHandle m_LightsBufferHandle;
	ConstantBufferHandle m_MaterialBufferHandle;

	typedef RenderBucket<GBufferKey> GBufferBucket;
	GBufferBucket m_GBuffer;
	GBufferBucket::RenderBucketCallbacks m_GBufferBucketCallbacks;

	RenderTargetHandle m_GBufferTarget;
};

ENDNAMESPACE
