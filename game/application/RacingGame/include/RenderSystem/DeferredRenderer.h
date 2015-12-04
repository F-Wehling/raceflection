#pragma once

#include "RenderSystem/RenderBucket.h"
#include "RenderSystem/RenderHandles.h"

#include <Math/Math.h>

BEGINNAMESPACE

class RenderSystem;
class Scene;

#if OS_LINUX
struct BoundingBox;
#endif

class DeferredRenderer {
public:
	typedef uint64 GBufferKey;
private:
    struct _GenGBufferKey{
		static const size_type MatIdxCount =  MaterialHandle::IndexBitCount;
		static const size_type DepthCount = 24;
		static const size_type PassCount = 4;
		static const size_type PreSortCount = (sizeof(GBufferKey) * 8 - MatIdxCount - DepthCount - PassCount) / 2;
		static const size_type PostSortCount = (sizeof(GBufferKey) * 8 - MatIdxCount - DepthCount - PassCount - PreSortCount);

		GBufferKey preSort : PreSortCount;
		GBufferKey material : MatIdxCount;
		GBufferKey depth : DepthCount;
		GBufferKey pass : PassCount;
		GBufferKey postSort : PostSortCount;
    };
private:
	GBufferKey GenerateGBufferKey(const BoundingBox& aabb, MaterialHandle matHandle, uint8 pass);
public:
	DeferredRenderer(RenderSystem* renderSystem);
	~DeferredRenderer();

	bool initialize();
	void render(float32 dt, Scene* scene);
	void shutdown();
private:
	RenderSystem* m_RefRenderSys;

	RenderBucket<GBufferKey> m_GBuffer;
	RenderTargetHandle m_GBufferTarget;
};

ENDNAMESPACE
