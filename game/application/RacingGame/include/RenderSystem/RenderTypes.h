#pragma once

#include <Utilities/Flags.h>

BEGINNAMESPACE

DECLARE_EXCLUSIVE_FLAGS(RenderEngineType, OpenGL, Null);
DECLARE_EXCLUSIVE_FLAGS(RenderBufferType, RGB8, RGBA8, F32, DEPTH_STENCIL);
DECLARE_EXCLUSIVE_FLAGS(RenderTextureType, RGB8, RGBA8, F32);

struct RenderTargetLayout {
	enum {
		COLOR_ATTACHMENT = 0,
		DEPTH_ATTACHMENT = 1,
		STENCIL_ATTACHMENT = 2,
		DEPTH_STENCIL_ATTACHMENT = 4,
		_2D = 0,
		_1D = 8,
		_3D = 16,
		_CUBE = 32
	};
	int16 width, height, depth;

	uint8 numRenderTextures;
	uint8 textureFlags[16];
	RenderTextureTypeFlags textureTypes[16];

	uint8 numRenderBuffer;
	uint8 bufferFlags[16];
	RenderBufferTypeFlags bufferTypes[16];
};

DECLARE_EXCLUSIVE_FLAGS(VertexElementType, FLOAT, INT);
DECLARE_EXCLUSIVE_FLAGS(BufferUsage, STATIC_DRAW, DYNAMIC_DRAW);

struct VertexLayoutSpec{
	static const uint32 MaxElementsPerLayout = 16;
	uint32 numberOfElements;
	uint32 elementType[MaxElementsPerLayout];
	uint8 elementCount[MaxElementsPerLayout];
};

struct GeometrySpec {
	static const uint32 MaxVertexBuffer = 5;

	uint32 numberOfVertexBuffer;
	uint32 numberOfVerticesPerBuffer;
	uint32 bufferUsage;
	uint32 vertexStride[MaxVertexBuffer];

	VertexLayoutSpec vertexLayout[MaxVertexBuffer];
	Byte* vertexData[ MaxVertexBuffer ];

	uint32 numberOfIndices;
	Byte* indexData;
};

struct ShaderProgramSpec {
	enum {
		VERTEX_SHADER = 0,
		FRAGMENT_SHADER = 1,
		GEOMETRY_SHADER = 2,
		TESSELLATION_CONTROL_SHADER = 3,
		TESSELLATION_EVALUATION_SHADER = 4,
		SHADER_TYPE_COUNT
	};
	enum {
		VERTEX_SHADER_SOURCE = 0 << 0,
		VERTEX_SHADER_FILE = 0 << 1,
		FRAGMENT_SHADER_FILE = 0 << 1,
		FRAGMENT_SHADER_SOURCE = 1 << 2,
		GEOMETRY_SHADER_SOURCE = 0 << 3,
		GEOMETRY_SHADER_FILE = 1 << 3,
		TESSCTRL_SHADER_SOURCE = 0 << 4,
		TESSCTRL_SHADER_FILE = 1 << 4,
		TESSEVAL_SHADER_SOURCE = 0 << 5,
		TESSEVAL_SHADER_FILE = 1 << 5
	};
	uint8 sourceOrFile;

	const ansichar* data[SHADER_TYPE_COUNT];
};

ENDNAMESPACE