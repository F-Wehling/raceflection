#pragma once

#include <RenderSystem/RenderHandles.h>

BEGINNAMESPACE

struct Mesh {
	GeometryHandle m_Geometry;
	MaterialHandle m_Materials[GeometrySpec::MaxSubMeshes];
	struct {
		uint32 startIndex;
		uint32 indexCount;
	} m_Submesh[GeometrySpec::MaxSubMeshes];
	uint32 m_NumSubMeshes;
};

ENDNAMESPACE