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
	case RenderTextureType::RGB8:
		return GL_RGB;
	case RenderTextureType::RGBA8:
		return GL_RGBA;
	case RenderTextureType::F32:
		return GL_RED;
	case RenderBufferType::DEPTH_STENCIL:
		return GL_DEPTH_STENCIL;
	default: return GL_INVALID_ENUM;
	}
}

uint32 glGetFormat(RenderTextureTypeFlags type) {
	switch (type) {
	case RenderTextureType::RGB8:
		return GL_RGB;
	case RenderTextureType::RGBA8:
		return GL_RGBA;
	case RenderTextureType::F32:
		return GL_RED;
	default: return GL_INVALID_ENUM;
	}
}

uint32 glGetFormat(RenderBufferTypeFlags type) {
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

uint32 glGetType(VertexAttribTypeFlags typeId)
{
	switch (typeId) {
	case VertexAttribType::Float32:
		return GL_FLOAT;
	case VertexAttribType::Int32:
		return GL_INT;
	default:
		return GL_INVALID_ENUM;
	}

	return GL_INVALID_ENUM;
}

uint32 glGetTypeSize(VertexAttribTypeFlags typeId)
{
	switch (typeId) {
	case VertexAttribType::Float32:
		return sizeof(float32);
	case VertexAttribType::Int32:
		return sizeof(int32);
	default:
		return GL_INVALID_ENUM;
	}

	return GL_INVALID_ENUM;
}

uint32 glGetPrimitiveType(PrimitiveTypeFlags modeId)
{
	switch (modeId) {
	case PrimitiveType::Triangle:
		return GL_TRIANGLES;
	case PrimitiveType::Quads:
		return GL_QUADS;
	case PrimitiveType::Line:
		return GL_LINE;
	case PrimitiveType::Points:
		return GL_POINTS;
	default: 
		return GL_INVALID_ENUM;
	}
}

ENDNAMESPACE