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

ENDNAMESPACE