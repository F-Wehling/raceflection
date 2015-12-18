#pragma once

#include "Utilities/Handle.h"

BEGINNAMESPACE

typedef Handle<12, 20> VertexBufferHandle;
typedef Handle<12, 20> IndexBufferHandle;
typedef Handle<12, 20> RenderTargetHandle;
typedef Handle<12, 20> VertexLayoutHandle;
typedef Handle<12, 20> ShaderProgramHandle;
typedef Handle<12, 20> ConstantBufferHandle;
typedef Handle<16, 16> MaterialHandle;
typedef Handle<16, 16> TextureHandle;
typedef Handle<12, 20> GeometryHandle;

extern VertexBufferHandle InvalidVertexBufferHandle;
extern IndexBufferHandle InvalidIndexBufferHandle;
extern RenderTargetHandle InvalidRenderTargetHandle;
extern VertexLayoutHandle InvalidVertexLayoutHandle;
extern ShaderProgramHandle InvalidShaderProgramHandle;
extern ConstantBufferHandle InvalidConstantBufferHandle;
extern MaterialHandle InvalidMaterialHandle;
extern TextureHandle InvalidTextureHandle;
extern GeometryHandle InvalidGeometryHandle;

ENDNAMESPACE
