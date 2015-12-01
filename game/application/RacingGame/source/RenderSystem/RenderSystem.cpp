#include "RenderSystem/RenderSystem.h"

#if USE_NULLRENDERER
#	include "RenderSystem/Null/NullBackend.h"
#else
#	include "RenderSystem/OpenGL/OpenGLBackend.h"
#endif

#include <ACGL/ACGL.hh>
#include <ACGL/OpenGL/Objects.hh>

#include <Logging/Logging.h>

BEGINNAMESPACE

Byte* gRenderSystemMemory = nullptr;

RenderSystem::RenderSystem() : m_Allocator("RenderSystemAllocator")
{
	if (gRenderSystemMemory) {
		LOG_WARNING(Renderer, "All Renderer instances share their workspace-memory.");
		return;
	}
	gRenderSystemMemory = eng_new_array(Byte[sSystemMemorySize]);
	m_Allocator.initialize(gRenderSystemMemory, sSystemMemorySize);
}

RenderSystem::~RenderSystem()
{
	shutdown();
	if (gRenderSystemMemory) {
		eng_delete_array(gRenderSystemMemory);
	}
}

bool RenderSystem::initialize() //here we have a valid context for the RenderBackend to startup
{
	if (!Backend::StartupBackend()) {
		LOG_ERROR(Renderer, "Render-backend start failed.");
		return false;
	}
	
	if (!Backend::InitializeBackend()) {
		LOG_ERROR(Renderer, "Render-backend initialization failed");
		return false;
	}

	return true;
}

void RenderSystem::shutdown()
{
	Backend::ShutdownBackend();
}

bool RenderSystem::tick(float32 dt)
{
	glClearColor(0.4, 0.4, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

ENDNAMESPACE

