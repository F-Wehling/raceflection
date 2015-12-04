#include "RenderSystem/OpenGL/OpenGLConversion.h"

#include <ACGL/OpenGL/GL.hh>

BEGINNAMESPACE

uint32 glGetInternalFormat(RenderTextureTypeFlags type) {
	switch (type) {
	case RenderTextureType::RGB8:
		return GL_RGBA8;
	case RenderTextureType::RGBA8:
		return GL_RGBA8;
	case RenderTextureType::F32:
		return GL_R32F;
	default: return GL_INVALID_ENUM;
	}
}

uint32 glGetInternalFormat(RenderBufferTypeFlags type) {
	switch (type) {
	case RenderBufferType::RGB8:
		return GL_RGBA8;
	case RenderBufferType::RGBA8:
		return GL_RGBA8;
	case RenderBufferType::F32:
		return GL_R32F;
	case RenderBufferType::DEPTH_STENCIL:
		return GL_DEPTH24_STENCIL8;
	default: return GL_INVALID_ENUM;
	}
}

ENDNAMESPACE