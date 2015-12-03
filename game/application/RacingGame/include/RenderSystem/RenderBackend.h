#pragma once

#if USE_NULL_RENDERER
#	include "RenderSystem/Null/NullBackend.h"
#else 
#	include "RenderSystem/OpenGL/OpenGLBackend.h"
#endif