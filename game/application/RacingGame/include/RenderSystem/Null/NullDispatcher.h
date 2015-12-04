#pragma once

#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/Null/NullBackend.h"

BEGINNAMESPACE

class NullDispatcher {
public:
	static void Initialize();
public:
	DEFINE_DISPATCHER(IgnoreCall);
};

typedef NullDispatcher dispatcher;

ENDNAMESPACE