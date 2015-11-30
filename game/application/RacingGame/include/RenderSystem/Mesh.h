#pragma once

#include <RenderSystem/RenderHandles.h>

BEGINNAMESPACE

struct Mesh {
	VertexBufferHandle vertices;
	VertexDescHandle vertexDesc;
	IndexBufferHandle indices;
};

ENDNAMESPACE