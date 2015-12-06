#pragma once

#include "RenderSystem/RenderCommandPacket.h"
#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/OpenGL/OpenGLBackend.h"

BEGINNAMESPACE

class GLDispatcher {
public:
	static void Initialize();
public:
	DEFINE_DISPATCHER(Draw);
	DEFINE_DISPATCHER(DrawIndexed);
	DEFINE_DISPATCHER(DrawGeometry);
	DEFINE_DISPATCHER(ActivateShader);
	DEFINE_DISPATCHER(CopyConstantBufferData);
	DEFINE_DISPATCHER(ClearTarget);
	DEFINE_DISPATCHER(ClearScreen);
	DEFINE_DISPATCHER(ScreenSetClearColor);
};


ENDNAMESPACE