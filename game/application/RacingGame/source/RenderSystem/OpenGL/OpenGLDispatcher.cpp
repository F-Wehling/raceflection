#include "RenderSystem/OpenGL/OpenGLDispatcher.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"
#include "RenderSystem/RenderCommand.h"

#include "MemorySystem.h"
#include "Allocator/PoolAllocator.h"

BEGINNAMESPACE
typedef GLDispatcher dispatcher; //to make sure the type is defined for the makros

IMPL_DISPATCHER(Draw)
	GLBackend::Draw(cmd->vertexCount, cmd->startVertex, cmd->vertexBuffer, cmd->vertexLayout);
}

IMPL_DISPATCHER(DrawIndexed)
	GLBackend::DrawIndexed(cmd->indexCount, cmd->startIndex, cmd->baseVertex, cmd->vertexBuffer, cmd->indexBuffer, cmd->vertexLayout);
}

IMPL_DISPATCHER(DrawGeometry)
	GLBackend::DrawGeometry(cmd->indexCount, cmd->startIndex, cmd->geometryHandle);
}

IMPL_DISPATCHER(ActivateShader)
	GLBackend::ActivateShader(cmd->shaderProgram);
}

IMPL_DISPATCHER(CopyConstantBufferData)
	GLBackend::CopyConstantBufferData(cmd->constantBuffer, cmd->data, cmd->size);
}

IMPL_DISPATCHER(ClearTarget)
	GLBackend::ClearRenderTarget(cmd->renderTarget);
}

IMPL_DISPATCHER(ClearScreen)
	GLBackend::ClearScreen();
}

IMPL_DISPATCHER(ScreenSetClearColor)
	GLBackend::ScreenSetClearColor(cmd->r, cmd->g, cmd->b, cmd->a);
}

void GLDispatcher::Initialize()
{
	SET_RENDER_DISPATCHER(Draw) = GLDispatcher::Draw;
	SET_RENDER_DISPATCHER(DrawIndexed) = GLDispatcher::DrawIndexed;
	SET_RENDER_DISPATCHER(DrawGeometry) = GLDispatcher::DrawGeometry;
	SET_RENDER_DISPATCHER(CopyConstantBufferData) = GLDispatcher::CopyConstantBufferData;
	SET_RENDER_DISPATCHER(ClearTarget) = GLDispatcher::ClearTarget;
	SET_RENDER_DISPATCHER(ClearScreen) = GLDispatcher::ClearScreen;
	SET_RENDER_DISPATCHER(ScreenSetClearColor) = GLDispatcher::ScreenSetClearColor;
    SET_RENDER_DISPATCHER(ActivateShader) = GLDispatcher::ActivateShader;
 }

ENDNAMESPACE

