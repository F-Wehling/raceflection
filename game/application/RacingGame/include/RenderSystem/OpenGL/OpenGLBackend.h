#pragma once

#include "RenderSystem/RenderHandles.h"

BEGINNAMESPACE

class GLBackend {
public:
	static bool StartupBackend();
	static bool InitializeBackend();
	static bool ShutdownBackend();

	static bool AppQuery();
	
	static VertexBufferHandle CreateStaticVertexBuffer(size_type bufferSize, Byte* pInitialData);
	static VertexBufferHandle CreateDynamicVertexBuffer(size_type bufferSize, Byte* pInitialData);
	static void DestroyVertexBuffer(VertexBufferHandle vb);

	typedef Handle<12, 20> IndexBufferHandle;
	static IndexBufferHandle CreateIndexBuffer(size_type bufferSize, void* pInitialData);
	static void DestroyIndexBuffer(IndexBufferHandle ib);

	//
	///Draw
	static void Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout);
	static void DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout);

	//
	///Constant Buffer
	static void CopyConstantBufferData(ConstantBufferHandle cbHdl, const void* data, uint32 size);
};

typedef GLBackend Backend;

ENDNAMESPACE