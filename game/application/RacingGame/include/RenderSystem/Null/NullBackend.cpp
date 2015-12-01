#include <RenderSystem/Null/NullBackend.h>

BEGINNAMESPACE

#if USE_NULLRENDERER

bool NullBackend::StartupBackend(GenericApplication* app)
{
	return true;
}

bool NullBackend::ShutdownBackend(GenericApplication* app)
{
	return true;
}

#endif

ENDNAMESPACE

