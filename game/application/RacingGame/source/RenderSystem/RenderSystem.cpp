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
	return true;
}

GeometryHandle demo_Cube;
ShaderProgramHandle demo_Shader;
void demo_data(RenderBackend* backend) {

	static const float32 cubeVertices[] = {
        -1.0, -1.0, -0.5,
        1.0, -1.0,  -0.5,
        -1.0,  1.0, -0.5,
        1.0,  1.0,  -0.5,
        -1.0, -1.0, 0.5,
        1.0, -1.0, 0.5,
        -1.0,  1.0, 0.5,
        1.0,  1.0, 0.5,
	};

    static const float32 screenSpaceBox[] = {
        1.0, 1.0, 0.5,
        -1.0, 1.0, 0.5,
        1.0, -1.0, 0.5,
        -1.0,-1.0, 0.5,
    };


	static const uint16 cubeIndices[] = {
		0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
	};

    static const uint16 screenSpaceBoxIndices[] = {
        0, 1, 2, 3
    };

    //*
	//
	///Specify the geometry to create
	GeometrySpec geo_spec = {
		1, 8, BufferUsage::STATIC_DRAW, //#VertexBuffer, #VerticesPerVertexbuffer, Buffer Usage
		{ //per VertexBuffer:
			3 * sizeof(float32) //Vertex stride (sizeof whole vertex)
		}, 
		{ //per VertexBuffer:
			{// a VertexElementAttribute Layout Specification
				1, //1 Element
				{ //Per Element:
					VertexElementType::FLOAT // type of i.th Element
				},
				{ //Per Element:
					3 //Element count of i.th Element
				}
			}
		},
		{
			(Byte*)cubeVertices //Data for i.th vertex buffer
		},
		14, //number of indices (14 < 65536 ? sizeof(INDEX) == sizeof(int16) : sizeof(INDEX) == sizeof(int32))
		(Byte*)cubeIndices //data for indices
	};
	demo_Cube = backend->createGeometry(geo_spec);
    //*/


    /*
    //
    ///Specify the geometry to create
    GeometrySpec geo_spec = {
        1, 4, BufferUsage::STATIC_DRAW, //#VertexBuffer, #VerticesPerVertexbuffer, Buffer Usage
        { //per VertexBuffer:
            3 * sizeof(float32) //Vertex stride (sizeof whole vertex)
        },
        { //per VertexBuffer:
            {// a VertexElementAttribute Layout Specification
                1, //1 Element
                { //Per Element:
                    VertexElementType::FLOAT // type of i.th Element
                },
                { //Per Element:
                    3 //Element count of i.th Element
                }
            }
        },
        {
            (Byte*)screenSpaceBox //Data for i.th vertex buffer
        },
        4, //number of indices (14 < 65536 ? sizeof(INDEX) == sizeof(int16) : sizeof(INDEX) == sizeof(int32))
        (Byte*)screenSpaceBoxIndices //data for indices
    };
    demo_Cube = backend->createGeometry(geo_spec);
    //*/

	ShaderProgramSpec shaderProgramSpec = {
		0, //Shader program locations
		{
			"#version 330\n" //Vertex Shader source
			"\n"
			"layout(location=0) in vec3 vert;\n"
            "//out vec3 out_Vertex;\n"
			"void main() {\n"
            "	//out_Vertex = vert;\n"
            "	gl_Position = vec4(0.1*vert.xyz, 1.0);\n"
			"}",
			"#version 330\n"
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

	/*
	//
	// constant buffer 
	//
	ConstantBufferHandle cbHdl = backend->createConstantBuffer();
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

ENDNAMESPACE

