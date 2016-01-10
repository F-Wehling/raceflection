#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/DeferredRenderer.h"
#include "RenderSystem/RenderContext.h"
#include "RenderSystem/Scene.h"
#include "RenderSystem/MaterialTexture.h"

#include "WindowSystem/Window.h"

#include <glm/glm.hpp>

#include "Logging.h"

#include "ResourceSpec.h"
#include "PackageSpec.h"

#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgRenderSystemStorage("RenderSystemStorage", "Size for the render system", KILOBYTE(10));

RenderSystem::RenderSystem(Main* mainRef) :
	m_MainRef(mainRef),
	m_Allocator("RenderSystemAllocator"),
	m_Renderer(nullptr),
	m_RenderBackend(nullptr),
	m_Materials(Material::MaxMaterials),
	m_NumberOfMaterials(0)
{
	m_Allocator.initialize(cfgRenderSystemStorage);
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
	
	UUID defaultTexture = { 1,1 };
	for (uint32 textureIdx = 0; textureIdx < packageSpec->getTextureCount(); ++textureIdx) {
		const TextureSpec* t = packageSpec->getTextureSpec(textureIdx);
		
		TextureHandle hdl = m_RenderBackend->createTexture(t);
		if (hdl == InvalidTextureHandle) continue;

		m_TextureHandles[t->uuid] = hdl;
	}

	if (m_TextureHandles[defaultTexture] == TextureHandle() || m_TextureHandles[defaultTexture] == InvalidTextureHandle) {
		//there is no default texture specified... this can cause strange 
		LOG_WARNING(Renderer, "There is no default texture specified. This can cause strange render result");
	}

	for (uint32 materialIdx = 0; materialIdx < packageSpec->getMaterialCount(); ++materialIdx) {
		const MaterialSpec* m = packageSpec->getMaterialSpec(materialIdx);

		Material* material = m_Scene->addMaterial(); //Insert into scenes material database (without textures)
		{
			material->v4_diffuseColor = glm::vec4(m->diffuseColor[0], m->diffuseColor[1], m->diffuseColor[2], m->diffuseColor[3]);
			material->v4_specularColor = glm::vec4(m->specularColor[0], m->specularColor[1], m->specularColor[2], m->specularColor[3]);
			material->v4_ambientColor = glm::vec4(m->ambientColor[0], m->ambientColor[1], m->ambientColor[2], m->ambientColor[3]);
			material->v4_emissiveColor = glm::vec4(m->emissiveColor[0], m->emissiveColor[1], m->emissiveColor[2], m->emissiveColor[3]);
			material->v4_transparentColor = glm::vec4(m->transparentColor[0], m->transparentColor[1], m->transparentColor[2], m->transparentColor[3]);
			material->v4_reflectiveColor = glm::vec4(m->reflectiveColor[0], m->reflectiveColor[1], m->reflectiveColor[2], m->reflectiveColor[3]);
			material->f_opacity = m->opacity;
			material->f_shininess = m->shininess;
			material->f_shininessStrength = m->shininessStrength;
			material->f_refracti = m->refracti;
			material->f_bumbScaling = m->bumbScaling;
		}

		//material <=> textures connection
		uint32 matTexIdx = m_NumberOfMaterials++;
		MaterialTexture& matTex = m_Materials[matTexIdx];
		matTex.m_GPUId = material - m_Scene->getMaterialData();
		matTex.m_CreatedFrom = m; 
		matTex.m_GPURepresentation = material; //MaterialHandle => GPU Representation
		for (uint32 tex = 0; tex < MaterialTexture::count; ++tex) {
			if (m->numberOfMaps[tex] <= 0) { // no map specified -> use default
				matTex.m_Textures[tex] = m_TextureHandles[defaultTexture];
			}
			else { //a texture is specified
				matTex.m_Textures[tex] = m_TextureHandles[m->textureRefs[0]]; //only the first texture is supported
			}
		}
		MaterialHandle hdl = { matTexIdx, 1 };
		m_MaterialHandles[m->uuid] = hdl;
	}

	int32 i = 0;
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

	//assume all packaged-lights are static for now
	for (uint32 lightIdx = 0; lightIdx < packageSpec->getLightCount(); ++lightIdx) {
		const LightSpec* l = packageSpec->getLightSpec(lightIdx);
		
		Light* light = m_Scene->addLight();
		{
			light->type = l->type;
			light->v3_position = glm::vec3(l->position[0], l->position[1], l->position[2]);
			light->v3_dir = glm::vec3(l->direction[0], l->direction[1], l->direction[2]);
			light->v3_diffuse = glm::vec3(l->diffuse[0], l->diffuse[1], l->diffuse[2]);
			light->v3_specular = glm::vec3(l->specular[0], l->specular[1], l->specular[2]);
			light->v3_ambient = glm::vec3(l->ambient[0], l->ambient[1], l->ambient[2]);
			light->f_AttenuationCst = l->attenuationConstant;
			light->f_AttenuationLin = l->attenuationLinear;
			light->f_AttenuationSq = l->attenuationQuadratic;
			light->f_AngleInnerCone = l->angleInnerCone;
			light->f_AngleOuterCone = l->angleOuterCone;
		}
	}

	return true;
}

bool RenderSystem::updateResourcesFromPackage(PackageSpec * packageSpec)
{
	//use the resources specified in the package to create renderable definitions	
	//first create geometry
	for (uint32 geometryIdx = 0; geometryIdx < packageSpec->getGeometryCount(); ++geometryIdx) {
		const GeometrySpec* g = packageSpec->getGeometrySpec(geometryIdx);
		m_RenderBackend->updateGeometry(m_GeometryHandles[g->uuid],g); //update geometry by uuid
	}

	for (uint32 textureIdx = 0; textureIdx < packageSpec->getTextureCount(); ++textureIdx) {
		const TextureSpec* t = packageSpec->getTextureSpec(textureIdx);
		m_RenderBackend->updateTexture(m_TextureHandles[t->uuid], t);
	}
	/*
	@TODO update material
	for (uint32 materialIdx = 0; materialIdx < packageSpec->getMaterialCount(); ++materialIdx) {
		const MaterialSpec* m = packageSpec->getMaterialSpec(materialIdx);

		union {
			Material* material;
			Byte* _materialBuffer;
		};
		_materialBuffer = getNthElement<Material>(m_MaterialHandles[m->uuid], m_MaterialAllocator);
		m_Materials[m_NumberOfMaterials] = CreateMaterialFromSpecification(m, material);
		//Connect textures
		uint32 textureCnt = 0;
		for (uint32 i = 0; i < 11; ++i) {
			for (uint32 j = 0; j < material->m_NumberOfMaps[i]; ++j) {
				material->m_TextureHandles[i][j] = m_TextureHandles[m->textureRefs[textureCnt++]];
			}
		}
		MaterialHandle hdl{ m_NumberOfMaterials++, 0 };
		m_MaterialHandles[m->uuid] = hdl;
	}

	int32 i = 0;
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
	*/


	return true;
}

ENDNAMESPACE

