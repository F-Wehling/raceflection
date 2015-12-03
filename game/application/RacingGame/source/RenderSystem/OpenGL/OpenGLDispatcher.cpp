#include "RenderSystem/OpenGL/OpenGLDispatcher.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"

#include "MemorySystem.h"
#include "Allocator/PoolAllocator.h"

BEGINNAMESPACE

IMPL_DISPATCHER(Draw)
	GLBackend::Draw(cmd->vertexCount, cmd->startVertex, cmd->vertexBuffer, cmd->vertexLayout);
}

IMPL_DISPATCHER(DrawIndexed)
	GLBackend::DrawIndexed(cmd->indexCount, cmd->startIndex, cmd->baseVertex, cmd->vertexBuffer, cmd->indexBuffer, cmd->vertexLayout);
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


ENDNAMESPACE