#pragma once

#include "Utilities/Handle.h"

BEGINNAMESPACE

typedef Handle<12, 20> VertexBufferHandle;
typedef Handle<12, 20> IndexBufferHandle;
typedef Handle<12, 20> RenderTargetHandle;
typedef Handle<12, 20> VertexLayoutHandle;
typedef Handle<12, 20> ShaderProgramHandle;
typedef Handle<12, 20> ConstantBufferHandle;
typedef Handle<30, 2> MaterialHandle;

typedef struct {
	VertexBufferHandle vertex_buffer;
	IndexBufferHandle index_buffer;
} VertexArrayHandle;

ENDNAMESPACE
