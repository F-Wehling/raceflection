#pragma once

#include <Utilities/Handle.h>

BEGINNAMESPACE

typedef Handle<12, 20> VertexBufferHandle;
typedef Handle<12, 20> IndexBufferHandle;
typedef Handle<12, 20> VertexDescHandle;
typedef Handle<12, 20> ShaderProgramHandle;
typedef Handle<12, 20> RenderTargetHandle;

typedef struct {
	VertexBufferHandle vertex_buffer;
	IndexBufferHandle index_buffer;
} VertexArrayHandle;

ENDNAMESPACE
