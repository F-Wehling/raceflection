#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderTypes.h"

namespace nvFX { //The backend should create valid "backend-objects" for the effect system
	class ICstBuffer;
	class IResource;
}

BEGINNAMESPACE

struct VertexLayoutSpec;
struct GeometrySpec;
struct TextureSpec;

class RenderBackend {
public:
	virtual ~RenderBackend(){}
	virtual bool startupBackend() { return true; }
    virtual bool initializeContext() { return true; }
	virtual void shutdownBackend() {}

	/*
	virtual VertexBufferHandle createStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {	return VertexBufferHandle(); }
	virtual VertexBufferHandle createDynamicVertexBuffer(size_type bufferSize, Byte* pInitialData) { return VertexBufferHandle(); }
	virtual void destroyVertexBuffer(VertexBufferHandle vb) {}

	virtual IndexBufferHandle createIndexBuffer(size_type bufferSize, void* pInitialData) { return IndexBufferHandle(); }
	virtual void destroyIndexBuffer(IndexBufferHandle ib) {}
	*/
	virtual GeometryHandle createGeometry(const GeometrySpec* specification) { return InvalidGeometryHandle; };
	virtual bool updateGeometry(GeometryHandle handle, const GeometrySpec* specification) { return false; }
	virtual VertexLayoutHandle createVertexLayout(const VertexLayoutSpec* specification) { return InvalidVertexLayoutHandle; }

	//
	/// Texture
	virtual TextureHandle createTexture(const TextureSpec* specification) { return InvalidTextureHandle; }
	virtual TextureHandle createEmptyTextureForResource(nvFX::IResource* resource, int32& width, int32& height, int32& depth) { return InvalidTextureHandle; }
	virtual bool updateTexture(TextureHandle handle, const TextureSpec* specification) { return false;  }

	//
	/// Shader
	virtual ShaderProgramHandle createShaderProgram(ShaderProgramSpec specification) { return InvalidShaderProgramHandle; }

	virtual ConstantBufferHandle createConstantBuffer(nvFX::ICstBuffer* effectBuffer, uint32 size) { return InvalidConstantBufferHandle; }
	virtual ConstantBufferHandle createConstantBuffer(ConstantBufferSpec specification) { return InvalidConstantBufferHandle; }
	virtual void destroyConstantBufferFX(ConstantBufferHandle){}
	virtual void destroyConstantBuffer(ConstantBufferHandle){}

	virtual RenderTargetHandle createRenderTarget(RenderTargetLayout& rtl) { return InvalidRenderTargetHandle; }
	virtual void activateCubeRenderTarget(RenderTargetHandle handle, int32 side, RenderTargetLayout rtl) {}
	virtual void restoreLastRenderTarget(){}

	//
	/// Viewport size
	virtual void setViewportSize(int32 x, int32 y, int32 width, int32 height) {};
};

ENDNAMESPACE

#include "RenderSystem/Null/NullBackend.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"
