#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/DeferredRenderer.h"
#include "RenderSystem/RenderContext.h"

#include "WindowSystem/Window.h"

#include <glm/glm.hpp>

#include "Logging.h"

BEGINNAMESPACE

size_type gRenderSystemStorage = KILOBYTE(10); //Memory for underlying instantiation

RenderSystem::RenderSystem() :
	m_Allocator("RenderSystemAllocator", gRenderSystemStorage),
	m_Renderer(nullptr),
	m_RenderBackend(nullptr) {
	m_Allocator.initialize();
}

RenderSystem::~RenderSystem() {
	shutdown();
}

bool RenderSystem::initialize(RenderEngineTypeFlags engineType /* = RenderEngineType::OpenGL*/) //here we have a valid context for the RenderBackend to startup
{
	switch (engineType) {
	case RenderEngineType::OpenGL:
		m_RenderBackend = eng_new(GLBackend, m_Allocator);
		break;
	case RenderEngineType::Null:
		m_RenderBackend = eng_new(NullBackend, m_Allocator);
		break;
	default:
		LOG_ERROR(Renderer, "No Renderbackend for type %s available.", RenderEngineType::toString(engineType));
		return false;
	}

	m_EngineType = engineType;

	if (!m_RenderBackend->startupBackend()) {
		LOG_ERROR(Renderer, "Render-backend start failed.");
		return false;
	}

	m_Renderer = eng_new(DeferredRenderer, m_Allocator)(this);

	/*
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
	VertexBufferHandle vbHdl = m_RenderBackend->createStaticVertexBuffer(sizeof(_BOX_VERTICES) * sizeof(float32), (Byte*)_BOX_VERTICES);

	
	
	//
	// constant buffer 
	//
	ConstantBufferHandle cbHdl = m_RenderBackend->createConstantBuffer();
		typedef struct {
		glm::mat4 _viewMatrix;
		glm::mat4 _projMatrix;
	} MatrixStorage;

	typedef struct {
		glm::mat4 _lightMatrix;
		glm::mat4 _shadowMatrix;
	} ShadowStorage;

	MatrixStorage mat;
	*/
	return true;
}

bool RenderSystem::attachWindow(Window * window)
{
	RenderContext * cntx = window->createContext(m_EngineType);

	if (!cntx->valid()) 
		return false;
	
	cntx->makeCurrent();

	if (!m_Renderer->initialize()) {
		LOG_ERROR(Renderer, "Renderer-initialization failed.");
		return false;
	}
	return true;
}

void RenderSystem::shutdown()
{
	if(m_Renderer) m_Renderer->shutdown();
	if(m_RenderBackend) m_RenderBackend->shutdownBackend();

	eng_delete(m_Renderer, m_Allocator);
	eng_delete(m_RenderBackend, m_Allocator);
}

bool RenderSystem::tick(float32 dt)
{
	m_Renderer->render(dt, nullptr);

	return true;
}

ENDNAMESPACE

