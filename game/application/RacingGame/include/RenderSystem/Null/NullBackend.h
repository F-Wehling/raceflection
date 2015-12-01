#pragma once

#include "RenderSystem/RenderHandles.h"

BEGINNAMESPACE

class NullBackend {
public:
	static bool StartupBackend();
	static bool ShutdownBackend();
};

typedef NullBackend Backend;

ENDNAMESPACE
