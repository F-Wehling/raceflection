#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/DeferredRenderer.h"
#include "RenderSystem/RenderContext.h"
#include "RenderSystem/Scene.h"
#include "RenderSystem/Material.h"

#include "WindowSystem/Window.h"

#include <glm/glm.hpp>

#include "Logging.h"

#include "ResourceSpec.h"
#include "PackageSpec.h"

#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgRenderSystemStorage("RenderSystemStorage", "Size for the render system", KILOBYTE(10));
ConfigSettingUint32 cfgMaterialStorageSize("MaterialStorageSize", "Size for the material system", KILOBYTE(10));

RenderSystem::RenderSystem() :
	m_Allocator("RenderSystemAllocator"),
	m_MaterialAllocator("MaterialAllocator"),
	m_Renderer(nullptr),
	m_RenderBackend(nullptr),
	m_NumberOfMaterials(0)
{
	m_Allocator.initialize(cfgRenderSystemStorage);
	m_MaterialAllocator.initialize(cfgMaterialStorageSize);
	m_Materials.resize(1 << MaterialHandle::IndexBitCount);
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
	m_Scene = eng_new(Scene, m_Allocator);
	return true;
}

GeometryHandle demo_Cube;
ShaderProgramHandle demo_Shader;
ConstantBufferHandle demo_CBuffer;
void demo_data(RenderBackend* backend) {

	ShaderProgramSpec shaderProgramSpec = {
		0, //Shader program locations
		{
            "#version 330\n" //Vertex Shader source
            "#extension GL_ARB_shading_language_420pack : enable\n"
			"\n"
			"layout(location=0) in vec3 vert;\n"
            "layout(location=1) in vec3 normal;\n"
            "layout(location=2) in vec4 color;\n"
            "layout(std140, binding = 2) uniform ObjectMatrixBlock { \n"
			"	mat4 model; \n"
			"}; \n"
            "layout(std140, binding = 4) uniform SceneMatrixBlock { \n"
			"	mat4 view; \n"
			"	mat4 projection; \n"
            "};\n"
            "out vec4 vPosition;"
            "out vec4 vNormal;\n"
            "out vec4 vColor;\n"
			"void main() {\n"
            "	mat4 mvp = projection * view * model;\n"
            "   mat3 i_mvp = transpose(inverse(mat3(mvp)));\n"
            "	vPosition = mvp * vec4(vert.xyz, 1.0);\n"
            "   vNormal = vec4(i_mvp * normal, 1.0);"
            "   vColor = color; \n"
            "   gl_Position = vPosition; \n"
			"}",
            "#version 330\n"
            "in vec4 vPosition; \n"
            "in vec4 vNormal; \n"
            "in vec4 vColor; \n"
            "\n"
			"out vec4 out_Color; \n"
            "void main() {\n"
            "   vec3 normal = normalize(vNormal.xyz); \n"
            "   vec3 L = normalize( vec3(0.0, 18.0, -15.0) - vPosition.xyz);\n"
            "   //out_Color = vec4(vec3(dot(normal,L)),1.0); return; \n"
            "   vec4 Idiff = vColor * max(dot(normal,L), 0.0);\n"
            "   Idiff = clamp(Idiff, 0.0, 1.0); \n"
            "	out_Color = Idiff;\n"
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
	m_Renderer->render(dt, m_Scene);

	return true;
}

bool RenderSystem::createResourcesFromPackage(PackageSpec * packageSpec)
{
	//use the resources specified in the package to create renderable definitions	
	//first create geometry
    for (uint32 geometryIdx = 0; geometryIdx < packageSpec->getGeometryCount(); ++geometryIdx) {
		const GeometrySpec* g = packageSpec->getGeometrySpec(geometryIdx);
		m_GeometryHandles[g->uuid] = m_RenderBackend->createGeometry(g);
	}

	for (uint32 textureIdx = 0; textureIdx < packageSpec->getTextureCount(); ++textureIdx) {
		const TextureSpec* t = packageSpec->getTextureSpec(textureIdx);
		m_TextureHandles[t->uuid] = m_RenderBackend->createTexture(t);
	}

	for (uint32 materialIdx = 0; materialIdx < packageSpec->getMaterialCount(); ++materialIdx) {
		const MaterialSpec* m = packageSpec->getMaterialSpec(materialIdx);

		union {
			Material* material;
			Byte* _materialBuffer;
		};
		_materialBuffer = eng_new_N(Byte, MaterialSizeFromSpecification(m), m_MaterialAllocator);
		m_Materials[m_NumberOfMaterials] = CreateMaterialFromSpecification(m, material);
		//Connect textures
		uint32 textureCnt = 0;
		for (uint32 i = 0; i < 11; ++i) {
			for (uint32 j = 0; j < material->m_NumberOfMaps[i]; ++j) {
				material->m_TextureHandles[i][j] = m_TextureHandles[m->textureRefs[textureCnt++]];
			}
		}
		MaterialHandle hdl { m_NumberOfMaterials++, 0 };
		m_MaterialHandles[m->uuid] = hdl;
	}

	for (uint32 meshIdx = 0; meshIdx < packageSpec->getMeshCount(); ++meshIdx) {
		const MeshSpec* m = packageSpec->getMeshSpec(meshIdx);	
		SceneNode* sn = m_Scene->addSceneNode();

		Mesh mesh;
		mesh.m_NumSubMeshes = m->numSubMeshes;
		mesh.m_Geometry = m_GeometryHandles[m->geometry];
		for (uint32 subMesh = 0; subMesh < m->numSubMeshes; ++subMesh) {
			mesh.m_Materials[subMesh] = m_MaterialHandles[m->material[subMesh]];
			mesh.m_Submesh[subMesh].startIndex = m->subMeshes[subMesh].startIndex;
			mesh.m_Submesh[subMesh].indexCount = m->subMeshes[subMesh].indexCount;
		}

		sn->m_Mesh = mesh;
		sn->m_GameObject = nullptr;
		sn->m_Disabled = false;
	}

	return true;
}

ENDNAMESPACE

