#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderBackend.h"

BEGINNAMESPACE

class GLBackend : public RenderBackend {
	friend class GLDispatcher;
public:
	virtual bool startupBackend();
	virtual bool initializeContext();
	virtual void shutdownBackend();
		
	//
	/// Vertex Buffer
	virtual VertexBufferHandle createStaticVertexBuffer(size_type bufferSize, Byte* pInitialData);
	virtual VertexBufferHandle createDynamicVertexBuffer(size_type bufferSize, Byte* pInitialData);
	virtual void destroyVertexBuffer(VertexBufferHandle vb);
	
	//
	/// Index Buffer
	virtual IndexBufferHandle createIndexBuffer(size_type bufferSize, void* pInitialData);
	virtual void destroyIndexBuffer(IndexBufferHandle ib);
	
	//
	///Constant Buffer
	virtual ConstantBufferHandle createConstantBuffer();
	//
	///Render Targets
	virtual RenderTargetHandle createRenderTarget(RenderTargetLayout rtl);

	//
	/// Dispatch Commands
	//
protected:
	static void Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout);
	static void DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout);
	static void CopyConstantBufferData(ConstantBufferHandle cbHdl, const void* data, uint32 size);

	static void ClearRenderTarget(RenderTargetHandle rbHdl);

	//
	///Screen
	static void ClearScreen();
	static void ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a);

};

ENDNAMESPACE