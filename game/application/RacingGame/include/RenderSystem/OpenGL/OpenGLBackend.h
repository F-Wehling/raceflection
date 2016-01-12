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
		
	/*
	//
	/// Vertex Buffer
	virtual VertexBufferHandle createStaticVertexBuffer(size_type bufferSize, Byte* pInitialData);
	virtual VertexBufferHandle createDynamicVertexBuffer(size_type bufferSize, Byte* pInitialData);
	virtual void destroyVertexBuffer(VertexBufferHandle vb);
	
	//
	/// Index Buffer
	virtual IndexBufferHandle createIndexBuffer(size_type bufferSize, void* pInitialData);
	virtual void destroyIndexBuffer(IndexBufferHandle ib);
	//*/

	virtual GeometryHandle createGeometry(const GeometrySpec* specification);
	virtual bool updateGeometry(GeometryHandle handle, const GeometrySpec* specification);
	virtual VertexLayoutHandle createVertexLayout(const VertexLayoutSpec* specification);

	//
	///Constant Buffer
	virtual ConstantBufferHandle createConstantBuffer(nvFX::ICstBuffer* effectBuffer, uint32 size);
	virtual ConstantBufferHandle createConstantBuffer(ConstantBufferSpec specification);
	virtual void destroyConstantBufferFX(ConstantBufferHandle);
	virtual void destroyConstantBuffer(ConstantBufferHandle);

	//
	///Render Targets
	virtual RenderTargetHandle createRenderTarget(RenderTargetLayout& rtl);
	virtual void activateCubeRenderTarget(RenderTargetHandle handle, int32 side, RenderTargetLayout rtl);
	virtual void restoreLastRenderTarget();

	//
	/// Texture
	virtual TextureHandle createTexture(const TextureSpec* specification);
	virtual TextureHandle createEmptyTextureForResource(nvFX::IResource* resource, int32& width, int32& height, int32& depth);
	virtual bool updateTexture(TextureHandle handle, const TextureSpec* specification);

	//
	/// Shader
	virtual ShaderProgramHandle createShaderProgram(ShaderProgramSpec specification);

	//
	/// Viewport size
	virtual void setViewportSize(int32 x, int32 y, int32 width, int32 height);

	//
	/// Dispatch Commands
	//
protected:
	static void Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout);
	static void DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vlHdl);
	static void DrawGeometry(uint32 indexCount, uint32 startIndex, GeometryHandle geoHdl);
	static void ActivateShader(ShaderProgramHandle shaderProgram);
	static void CopyConstantBufferData(ConstantBufferHandle cbHdl, const void* data, uint32 size, uint32 offset);

	static void ClearRenderTarget(RenderTargetHandle rtHdl);

	//
	///Screen
	static void ClearScreen();
	static void ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a);

};

ENDNAMESPACE