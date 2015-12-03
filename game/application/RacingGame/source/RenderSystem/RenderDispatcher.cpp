#include "RenderSystem/RenderDispatcher.h"
#include "RenderSystem/RenderCommand.h"

BEGINNAMESPACE

namespace command {

	//
	///Draw
	SET_RENDER_DISPATCHER(Draw) = dispatcher::Draw;
	SET_RENDER_DISPATCHER(DrawIndexed) = dispatcher::DrawIndexed;

	//
	/// Buffer
	SET_RENDER_DISPATCHER(CopyConstantBufferData) = dispatcher::CopyConstantBufferData;

	//
	/// Render Target
	SET_RENDER_DISPATCHER(ClearTarget) = dispatcher::ClearTarget;

	//
	///
	SET_RENDER_DISPATCHER(ClearScreen) = dispatcher::ClearScreen;
	SET_RENDER_DISPATCHER(ScreenSetClearColor) = dispatcher::ScreenSetClearColor;
}


ENDNAMESPACE