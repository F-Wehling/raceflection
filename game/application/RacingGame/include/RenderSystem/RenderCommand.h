#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderCommandPacket.h"

#include "CompileTime/Templates.h"

BEGINNAMESPACE

#	define IS_RENDER_COMMAND(command) \
	static const RenderDispatcher sDispatcher

#	define RENDER_DISPATCHER(command) command::sDispatcher

#	define SET_RENDER_DISPATCHER( command ) \
		const RenderDispatcher RENDER_DISPATCHER(command)

#	define RenderCommand( Name ) \
	struct Name { \
		IS_RENDER_COMMAND( Name );

#	define EndRenderCommand(command) \
	}; \
	static_assert( IsPOD<command>::value, "A render command must be a POD");

#	define DEFINE_DISPATCHER( Name ) \
	static void Name (const void* data)

#	define IMPL_DISPATCHER( Name ) \
	void dispatcher::Name (const void* data) { \
		const commands::Name * cmd = reinterpret_cast<const commands::Name*>(data);

//
/// define rendering commands
namespace commands {

	RenderCommand(Draw)
		uint32 vertexCount;
		uint32 startVertex;

		VertexLayoutHandle vertexLayout;
		VertexBufferHandle vertexBuffer;
	EndRenderCommand(Draw);

	RenderCommand(DrawIndexed)
		uint32 indexCount;
		uint32 startIndex;
		uint32 baseVertex;

		VertexLayoutHandle vertexLayout;
		VertexBufferHandle vertexBuffer;
		IndexBufferHandle indexBuffer;
	EndRenderCommand(DrawIndexed);

	RenderCommand(CopyConstantBufferData)
		ConstantBufferHandle constantBuffer;
		void* data;
		uint32 size;
	EndRenderCommand(CopyConstantBufferData);
}

ENDNAMESPACE