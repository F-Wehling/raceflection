#pragma once

#include "RenderSystem/RenderCommandPacket.h"
#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"

BEGINNAMESPACE

class GLDispatcher {
public:
	DEFINE_DISPATCHER(Draw);
	DEFINE_DISPATCHER(DrawIndexed);
	DEFINE_DISPATCHER(CopyConstantBufferData);
	DEFINE_DISPATCHER(ClearTarget);
	DEFINE_DISPATCHER(ClearScreen);
	DEFINE_DISPATCHER(ScreenSetClearColor);
};

typedef GLDispatcher dispatcher;

ENDNAMESPACE