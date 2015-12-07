#pragma once

#include "RenderSystem/RenderTypes.h"

BEGINNAMESPACE

uint32 glGetInternalFormat(RenderBufferTypeFlags type);
uint32 glGetInternalFormat(RenderTextureTypeFlags type);
uint32 glGetType(VertexElementTypeFlags typeId);
uint32 glGetTypeSize(VertexElementTypeFlags typeId);
uint32 glGetDrawMode(DrawModeFlags modeId);

ENDNAMESPACE