#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderBackend.h"

BEGINNAMESPACE

class NullBackend : public RenderBackend{
public:
	//
	/// Dispatcher Commands
	//
protected:
	//the nullrenderer does not dispatch any commands 
	/*
	static void Draw(uint32 vertexCount, uint32 startVertex, VertexBufferHandle vbHdl, VertexLayoutHandle vbLayout){}
	static void DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 baseVertex, VertexBufferHandle vbHdl, IndexBufferHandle ibHdl, VertexLayoutHandle vbLayout){}
	static void CopyConstantBufferData(ConstantBufferHandle cbHdl, const void* data, uint32 size){}
	static void ClearRenderTarget(RenderTargetHandle rbHdl) {}
	static void ClearScreen(){}
	static void ScreenSetClearColor(float32 r, float32 g, float32 b, float32 a){}
	*/
};

ENDNAMESPACE
