#pragma once

#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/Null/NullBackend.h"

BEGINNAMESPACE

class NullDispatcher {
public:
	DEFINE_DISPATCHER(Draw);
	DEFINE_DISPATCHER(DrawIndexed);
	DEFINE_DISPATCHER(CopyConstantBufferData);
};

typedef NullDispatcher dispatcher;

ENDNAMESPACE