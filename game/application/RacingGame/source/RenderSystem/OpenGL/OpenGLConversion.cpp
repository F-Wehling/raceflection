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

uint32 glGetType(VertexElementTypeFlags typeId)
{
	switch (typeId) {
	case VertexElementType::FLOAT:
		return GL_FLOAT;
	case VertexElementType::INT:
		return GL_INT;
	default:
		return GL_INVALID_ENUM;
	}

	return GL_INVALID_ENUM;
}

uint32 glGetTypeSize(VertexElementTypeFlags typeId)
{
	switch (typeId) {
	case VertexElementType::FLOAT:
		return sizeof(float32);
	case VertexElementType::INT:
		return sizeof(int32);
	default:
		return GL_INVALID_ENUM;
	}

	return GL_INVALID_ENUM;
}

uint32 glGetDrawMode(DrawModeFlags modeId)
{
	switch (modeId) {
	case DrawMode::TRIANGLE:
		return GL_TRIANGLES;
	case DrawMode::QUADS:
		return GL_QUADS;
	case DrawMode::LINE:
		return GL_LINE;
	case DrawMode::POINTS:
		return GL_POINTS;
	default: 
		return GL_INVALID_ENUM;
	}
}

ENDNAMESPACE