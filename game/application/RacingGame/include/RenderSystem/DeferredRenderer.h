#pragma once

#include "RenderSystem/RenderBucket.h"
#include "RenderSystem/RenderHandles.h"

#include <Math/Math.h>

BEGINNAMESPACE

class Scene;

class DeferredRenderer {
public:
	typedef uint64 GBufferKey;
private:
	typedef struct {
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
	} _GenGBufferKey;
private:
	GBufferKey GenerateGBufferKey(const BoundingBox& aabb, MaterialHandle matHandle, uint8 pass);
public:
	DeferredRenderer();

	void render(float32 dt, Scene* scene);

private:
	RenderBucket<GBufferKey> m_GBuffer;
};

ENDNAMESPACE