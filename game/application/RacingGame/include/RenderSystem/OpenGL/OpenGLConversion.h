#pragma once

#include "RenderSystem/RenderTypes.h"
#include "GeometrySpec.h"

BEGINNAMESPACE

uint32 glGetInternalFormat(RenderBufferTypeFlags type);
uint32 glGetInternalFormat(RenderTextureTypeFlags type);
uint32 glGetFormat(RenderBufferTypeFlags type);
uint32 glGetFormat(RenderTextureTypeFlags type);
uint32 glGetType(VertexAttribTypeFlags typeId);
uint32 glGetTypeSize(VertexAttribTypeFlags typeId);
uint32 glGetPrimitiveType(PrimitiveTypeFlags modeId);

ENDNAMESPACE