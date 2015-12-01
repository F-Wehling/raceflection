#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/HardwareResourcePool.h"

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

bool RenderSystem::initialize()
{
	if (!ACGL::init()) {
		LOG_ERROR(Renderer, "ACGL Initialization failed.");
		return false;
	}
	
	m_ResourcePool = eng_new(HardwareResourcePool, m_Allocator);

	return true;
}

void RenderSystem::shutdown()
{
	eng_delete(m_ResourcePool, m_Allocator);
}

bool RenderSystem::tick(float32 dt)
{
    glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

ENDNAMESPACE

