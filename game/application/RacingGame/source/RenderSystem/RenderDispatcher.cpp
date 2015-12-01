#include <RenderSystem/RenderDispatcher.h>
#include <RenderSystem/RenderCommand.h>

BEGINNAMESPACE

namespace commands {

	//
	///Draw
	SET_RENDER_DISPATCHER(Draw) = dispatcher::Draw;
	SET_RENDER_DISPATCHER(DrawIndexed) = dispatcher::DrawIndexed;

	//
	/// Buffer
	SET_RENDER_DISPATCHER(CopyConstantBufferData) = dispatcher::CopyConstantBufferData;

}


ENDNAMESPACE