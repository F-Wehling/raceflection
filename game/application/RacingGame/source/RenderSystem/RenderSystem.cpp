#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/DeferredRenderer.h"
#include "RenderSystem/RenderContext.h"

#include "WindowSystem/Window.h"

#include <glm/glm.hpp>

#include "Logging.h"

#include "ResourceSpec.h"
#include "PackageSpec.h"

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
	return true;
}

GeometryHandle demo_Cube;
ShaderProgramHandle demo_Shader;
ConstantBufferHandle demo_CBuffer;
void demo_data(RenderBackend* backend) {
	/*
	float32 cube_Vertices[8][6] = {
		{  1,  1,  1,  1, 1, 1 }, // 0
		{ -1,  1,  1,  0, 1, 1 }, // 1
		{ -1, -1,  1,  0, 0, 1 }, // 2
		{  1, -1,  1,  1, 0, 1 }, // 3
		{  1, -1, -1,  1, 0, 0 }, // 4
		{ -1, -1, -1,  0, 0, 0 }, // 5
		{ -1,  1, -1,  0, 1, 0 }, // 6
		{  1,  1, -1,  1, 1, 0 }, // 7
	};

	uint16 cube_Indices[24] = {
		0, 1, 2, 3,                 // Front face
		7, 4, 5, 6,                 // Back face
		6, 5, 2, 1,                 // Left face
		7, 0, 3, 4,                 // Right face
		7, 6, 1, 0,                 // Top face
		3, 2, 5, 4,                 // Bottom face
	};

   
	//
	///Specify the geometry to create
	GeometrySpec geo_spec = {
		1, 8, BufferUsage::STATIC_DRAW, DrawMode::QUADS, //#VertexBuffer, #VerticesPerVertexbuffer, Buffer Usage
		{ //per VertexBuffer:
			6 * sizeof(float32) //Vertex stride (sizeof whole vertex)
		}, 
		{ //per VertexBuffer:
			{// a VertexElementAttribute Layout Specification
				2, //1 Element
				{ //Per Element:
					VertexElementType::FLOAT, // type of i.th Element
					VertexElementType::FLOAT
				},
				{ //Per Element:
					3, 3 //Element count of i.th Element
				}
			}
		},
		{
			(Byte*)cube_Vertices //Data for i.th vertex buffer
		},
		24, //number of indices (n < 65536 ? sizeof(INDEX) == sizeof(int16) : sizeof(INDEX) == sizeof(int32))
		(Byte*)cube_Indices //data for indices
	};
	//*/

	ShaderProgramSpec shaderProgramSpec = {
		0, //Shader program locations
		{
			"#version 420\n" //Vertex Shader source
			"\n"
			"layout(location=0) in vec3 vert;\n"
			"layout(binding = 3) uniform MatrixBlock { \n"
			"	mat4 modelView; \n"
			"	mat4 projection; \n"
			"};\n"
			"//out vec3 out_Vertex;\n"
			"void main() {\n"
			"	//out_Vertex = vert;\n"
			"	gl_Position = projection * modelView * vec4(0.1 * vert.xyz, 1.0);\n"
			"}",
			"#version 420\n"
			"//in vec3 out_Vertex; \n"
            "\n"
			"out vec4 out_Color; \n"
			"void main() {\n"
            "	out_Color = vec4(1.0,0.0,0.0,1.0);\n"
			"}",
			nullptr,//Geometry Shader source
			nullptr, //Tessellation Control Shader source
			nullptr //Tessellation Evaluation Shader source
		}
	};

	demo_Shader = backend->createShaderProgram(shaderProgramSpec);

	
	//
	// constant buffer 
	//
	demo_CBuffer = backend->createConstantBuffer({ 3 });
}

bool RenderSystem::attachWindow(Window * window)
{
	RenderContext * cntx = window->createContext(m_EngineType);

	if (!cntx->valid()) 
		return false;
	
	cntx->makeCurrent();

    if(!m_RenderBackend->initializeContext()){
        LOG_ERROR(Renderer, "Context-initalization failed.");
        return false;
    }

	if (!m_Renderer->initialize()) {
		LOG_ERROR(Renderer, "Renderer-initialization failed.");
		return false;
	}

	demo_data(m_RenderBackend);

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

bool RenderSystem::createResourcesFromPackage(PackageSpec * packageSpec)
{
	//use the resources specified in the package to create renderable definitions
	
	//first create geometry
	for (int32 geometryIdx = 0; geometryIdx < packageSpec->getGeometryCount(); ++geometryIdx) {
		const GeometrySpec* g = packageSpec->getGeometrySpec(geometryIdx);
		demo_Cube = m_RenderBackend->createGeometry(g);
	}

	return true;
}

ENDNAMESPACE

