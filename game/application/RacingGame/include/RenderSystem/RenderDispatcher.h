#pragma once

#if USE_NULLRENDERER
#	include "RenderSystem/Null/NullDispatcher.h"
#else 
#	include "RenderSystem/OpenGL/OpenGLDispatcher.h"
#endif