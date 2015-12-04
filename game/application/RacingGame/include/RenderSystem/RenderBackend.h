#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderTypes.h"

BEGINNAMESPACE


class RenderBackend {
public:
	virtual ~RenderBackend(){}
	virtual bool startupBackend() { return true; }
	virtual bool initialiizeContext() { return true; }
	virtual void shutdownBackend() {}

	virtual VertexBufferHandle createStaticVertexBuffer(size_type bufferSize, Byte* pInitialData) {	return VertexBufferHandle(); }
	virtual VertexBufferHandle createDynamicVertexBuffer(size_type bufferSize, Byte* pInitialData) { return VertexBufferHandle(); }
	virtual void destroyVertexBuffer(VertexBufferHandle vb) {}

	virtual IndexBufferHandle createIndexBuffer(size_type bufferSize, void* pInitialData) { return IndexBufferHandle(); }
	virtual void destroyIndexBuffer(IndexBufferHandle ib) {}

	virtual ConstantBufferHandle createConstantBuffer() { return ConstantBufferHandle(); }

	virtual RenderTargetHandle createRenderTarget(RenderTargetLayout rtl) { return RenderTargetHandle(); }
};

ENDNAMESPACE

#include "RenderSystem/Null/NullBackend.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"