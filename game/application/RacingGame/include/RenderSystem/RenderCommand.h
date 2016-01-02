#pragma once

#include "RenderSystem/RenderHandles.h"
#include "RenderSystem/RenderCommandPacket.h"

#include "CompileTime/Templates.h"

BEGINNAMESPACE

#	define IS_RENDER_COMMAND(command) \
	static RenderDispatcher sDispatcher

#	define RENDER_DISPATCHER(command) command::sDispatcher

#	define IMPL_RENDER_DISPATCHER( command ) \
		RenderDispatcher RENDER_DISPATCHER(command) = NullDispatcher::IgnoreCall;

#	define SET_RENDER_DISPATCHER( commandName ) \
		command::RENDER_DISPATCHER(commandName)

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
		const command::Name * cmd = reinterpret_cast<const command::Name*>(data);

//
/// define rendering commands
namespace command {

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

	RenderCommand(DrawGeometry)
		uint32 indexCount;
		uint32 startIndex;

		GeometryHandle geometryHandle;
	EndRenderCommand(DrawGeometry);

	RenderCommand(ActivateShader)
		ShaderProgramHandle shaderProgram;
	EndRenderCommand(ActivateShader);

	RenderCommand(CopyConstantBufferData)
		ConstantBufferHandle constantBuffer;
		const void* data;
		uint32 size;
		uint32 offset;
	EndRenderCommand(CopyConstantBufferData);

	RenderCommand(ClearTarget)
		RenderTargetHandle renderTarget;
	EndRenderCommand(ClearTarget);

	RenderCommand(ClearScreen)
	EndRenderCommand(ClearScreen);

	RenderCommand(ScreenSetClearColor)
		float32 r, g, b, a;
	EndRenderCommand(ScreenSetClearColor);
}

ENDNAMESPACE