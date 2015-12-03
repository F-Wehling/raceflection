#include "RenderSystem/RenderSystem.h"

#include <glm/glm.hpp>

#if USE_NULLRENDERER
#	include "RenderSystem/Null/NullBackend.h"
#else
#	include "RenderSystem/OpenGL/OpenGLBackend.h"
#endif

#include "Logging.h"

BEGINNAMESPACE

size_type gRenderSystemStorage = KILOBYTE(10); //Memory for underlying instantiation

RenderSystem::RenderSystem() : m_Allocator("RenderSystemAllocator", gRenderSystemStorage)
{
	m_Allocator.initialize();
}

RenderSystem::~RenderSystem()
{
	shutdown();
}

bool RenderSystem::initialize() //here we have a valid context for the RenderBackend to startup
{
	if (!Backend::StartupBackend()) {
		LOG_ERROR(Renderer, "Render-backend start failed.");
		return false;
	}

	float32 _BOX_VERTICES[] = {
		-1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 
		1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, 
		-1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 
		1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f
	};

	//
	/// vertex buffer
	VertexBufferHandle vbHdl = Backend::CreateStaticVertexBuffer(sizeof(_BOX_VERTICES) * sizeof(float32), (Byte*)_BOX_VERTICES);

	
	
	//
	// constant buffer 
	//
	ConstantBufferHandle cbHdl = Backend::CreateConstantBuffer();
		typedef struct {
		glm::mat4 _viewMatrix;
		glm::mat4 _projMatrix;
	} MatrixStorage;

	typedef struct {
		glm::mat4 _lightMatrix;
		glm::mat4 _shadowMatrix;
	} ShadowStorage;

	MatrixStorage mat;
	
	Backend::CopyConstantBufferData(cbHdl, &mat, sizeof(MatrixStorage));

	return true;
}

void RenderSystem::shutdown()
{
	Backend::ShutdownBackend();
}

bool RenderSystem::tick(float32 dt)
{
	m_Renderer.render(dt, nullptr);

	return true;
}

ENDNAMESPACE

