#include "RenderSystem/RenderDispatcher.h"
#include "RenderSystem/RenderCommand.h"

BEGINNAMESPACE

namespace command {

	//
	///Draw
	IMPL_RENDER_DISPATCHER(Draw);
	IMPL_RENDER_DISPATCHER(DrawIndexed);
	IMPL_RENDER_DISPATCHER(DrawGeometry);

	IMPL_RENDER_DISPATCHER(ActivateShader);

	//
	/// Buffer
	IMPL_RENDER_DISPATCHER(CopyConstantBufferData);

	//
	/// Render Target
	IMPL_RENDER_DISPATCHER(ClearTarget);

	//
	/// Screen
	IMPL_RENDER_DISPATCHER(ClearScreen);
	IMPL_RENDER_DISPATCHER(ScreenSetClearColor);
}


ENDNAMESPACE