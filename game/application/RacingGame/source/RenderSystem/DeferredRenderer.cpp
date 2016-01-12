#include "RenderSystem/DeferredRenderer.h"

#include "CompileTime/Templates.h"

#include "ObjectSystem/GameObject.h"

#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/Scene.h"
#include "RenderSystem/Camera.h"
#include "RenderSystem/ConstantBuffer.h"

#include "Utilities/FloatCompressor.h"
#include "Utilities/Number.h"
#include "Utilities/Random.h"

#include "Multithreading/JobScheduler.h"
#include "Multithreading/ParallelFor.h"

#include "Logging.h"

#include "Configuration/ConfigSettings.h"

#include "Main.h"
#include "EffectSystem/EffectSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxGBufferCommands("render.maxGBufferCommands", "Sets the maximum number of commands per frame for the G-Buffer", 40000);
ConfigSettingAnsichar cfgDeferredRenderEffect("render.deferredRenderEffect", "Sets the name of the deferred-renderer effect", "deferredRendering");
ConfigSettingUint32 cfgDeferredRenderTechniqueIdx("render.deferredRenderTechniqueIdx", "Set the deferred-render-technique to be used", 0);

//Uniforms
ConfigSettingAnsichar cfgMaterialDiffuseSampler("material.diffuseSampler", "Set the materials diffuse sampler name", "diffuseSampler");
ConfigSettingAnsichar cfgMaterialSpecularSampler("material.specularSampler", "Set the materials diffuse sampler name", "specularSampler");
ConfigSettingAnsichar cfgMaterialAmbientSampler("material.ambientSampler", "Set the materials diffuse sampler name", "ambientSampler");
ConfigSettingAnsichar cfgMaterialEmissiveSampler("material.emissiveSampler", "Set the materials diffuse sampler name", "emissiveSampler");
ConfigSettingAnsichar cfgMaterialHeightSampler("material.heightSampler", "Set the materials diffuse sampler name", "heightSampler");
ConfigSettingAnsichar cfgMaterialNormalSampler("material.normalSampler", "Set the materials diffuse sampler name", "normalSampler");
ConfigSettingAnsichar cfgMaterialShininessSampler("material.shininessSampler", "Set the materials diffuse sampler name", "shininessSampler");
ConfigSettingAnsichar cfgMaterialOpacitySampler("material.opacitySampler", "Set the materials diffuse sampler name", "opacitySampler");
ConfigSettingAnsichar cfgMaterialDisplacementSampler("material.displacementSampler", "Set the materials diffuse sampler name", "displacementSampler");
ConfigSettingAnsichar cfgMaterialLightSampler("material.lightSampler", "Set the materials diffuse sampler name", "lightSampler");
ConfigSettingAnsichar cfgMaterialReflectionSampler("material.reflectionSampler", "Set the materials diffuse sampler name", "reflectionSampler");
ConfigSettingAnsichar cfgMaterialMaterialIDSampler("material.materialID", "Set the materials diffuse sampler name", "materialID");

ConfigSettingAnsichar cfgPostProcessingPipeline("render.PostProcessPipeline", "Set the post processing pipeline seperated by ;", "");

DeferredRenderer::GBufferKey DeferredRenderer::GBufferKey::Generate() { 
	union { GBufferKey k; uint64 v; };
	v = 0;
	return k; 
}

DeferredRenderer::GBufferKey DeferredRenderer::GBufferKey::Generate(float32 depth, MaterialHandle material, uint8 pass) {
	static const uint8 MAX_PASS = MaxUnsignedWithNBits<uint32, _GenGBufferKey::PassCount>::value;
	ASSERT(pass <= MAX_PASS, "The G-Buffer is only capable of %d render passes", MAX_PASS);

	union {	_GenGBufferKey g; GBufferKey k; };

	g.preSort = 0;
	g.material = material.index;
	
	g.depth = Float16Compressor::compress(depth);

	CutToNBits<uint64, _GenGBufferKey::PassCount> cutter;
	cutter.set = pass;
	g.pass = (cutter.get);

	g.postSort = 0;
	return k;
}

void DeferredRenderer::GBufferKey::Decode(GBufferKey key, MaterialHandle& material) {
	union { _GenGBufferKey g; GBufferKey k; };
	k = key;

	material.index = g.material;
}

Random<float32> rnd;

DeferredRenderer::DeferredRenderer(RenderSystem* renderSys) : 
	m_DeferredRenderingEffect(InvalidEffectHandle),
	m_RefRenderSys(renderSys),
	m_GBuffer(cfgMaxGBufferCommands),
	m_ReflectionBuffer(cfgMaxGBufferCommands)
{}

DeferredRenderer::~DeferredRenderer()
{
	shutdown();
}


bool DeferredRenderer::render_doNothing() {
	return true;
}

bool DeferredRenderer::render_fullScreenQuad() {
	return true; //internally done by nvFX - just return true, to not abort rendering
}

template<typename Bucket>
struct ParallelData {
	DeferredRenderer* renderer;
	Bucket* bucket;
};
bool DeferredRenderer::renderSceneGraphShaded()
{
	m_RenderScene->getCamera()->update();
	update_matrices(m_RenderScene->getCamera(), m_GBuffer);

	ParallelData<GBufferBucket> deferredData = {
		this,
		&m_GBuffer
	};
	JobScheduler::Wait(
		parallel_for(m_RenderScene->getSceneNodes(), m_RenderScene->getSceneNodeCount(), &DeferredRenderer::RenderSceneNode, (void*)&deferredData)
		);

	m_GBuffer.sort();
	m_GBuffer.submit(m_GBufferBucketCallbacks);
	m_GBuffer.resetVariables();
	return true;
}

bool DeferredRenderer::renderModeNotImplemented()
{
	LOG_ERROR(Renderer, "This render mode isn't implemented");
	return false;
}

Camera g_ReflectionCamera;
GameObject g_ReflectionCameraObject;

bool DeferredRenderer::render_ReflectionCube() {
	const EffectSystem::RenderTargetStorage& hdl = m_EffectSystemRef->getCubeMapTargetByName("reflectionColor");
	glm::vec3 position = m_RenderScene->getCamera()->getGameObject()->getPosition();

	for (int32 side = 0; side < 6; ++side) {
		switch (side)
		{
		case 0: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(10.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); break;
		case 1: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(-10.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); break;
		case 2: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(0.0, 10.0, 0.0), glm::vec3(1.0, 0.0, 0.0)); break;
		case 3: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(0.0, -10.0, 0.0), glm::vec3(1.0, 0.0, 0.0)); break;
		case 4: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 1.0, 0.0)); break;
		case 5: g_ReflectionCameraObject.lookInDirection(position, glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 1.0, 0.0)); break;
		}
		g_ReflectionCamera.update();
		m_RefRenderSys->getBackend()->activateCubeRenderTarget(hdl.handle, side, hdl.layout);

		update_matrices(&g_ReflectionCamera, m_ReflectionBuffer);
		//update_matrices(m_RenderScene->getCamera(), m_ReflectionBuffer);

		ParallelData<GBufferBucket> deferredData = {
			this,
			&m_ReflectionBuffer
		};
		JobScheduler::Wait(
			parallel_for(m_RenderScene->getSceneNodes(), m_RenderScene->getSceneNodeCount(), &DeferredRenderer::RenderSceneNode, (void*)&deferredData)
			);

		m_ReflectionBuffer.sort();
		m_ReflectionBuffer.submit(m_GBufferBucketCallbacks);
		m_ReflectionBuffer.resetVariables();

		m_RefRenderSys->getBackend()->restoreLastRenderTarget();
	}
    return true;
}

bool DeferredRenderer::initialize()
{
	// A deferred renderer renders into different render targets
	RenderBackend* backend = m_RefRenderSys->getBackend();
	//
	/// Resource Management and deferredRendering-FBO-layout is handled by the effect system
	m_EffectRenderDelegates.sceneGraphShaded.bind<DeferredRenderer, &DeferredRenderer::renderSceneGraphShaded>(this);
	m_EffectRenderDelegates.sceneGraphNoShading.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.fullscreenQuad.bind<DeferredRenderer, &DeferredRenderer::render_fullScreenQuad>(this);
	m_EffectRenderDelegates.sceneGraphShadedOpaqueOnly.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphShadedTransparentOnly.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphNoShadingOpaqueOnly.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphNoShadingTransparentOnly.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphOutliesOnly.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad0.bind<DeferredRenderer, &DeferredRenderer::render_fullScreenQuad>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad1.bind<DeferredRenderer, &DeferredRenderer::render_fullScreenQuad>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad2.bind<DeferredRenderer, &DeferredRenderer::render_fullScreenQuad>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad3.bind<DeferredRenderer, &DeferredRenderer::render_fullScreenQuad>(this);
	m_EffectRenderDelegates.sceneGraphDebugScene.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugSceneLines.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.doNothing.bind<DeferredRenderer, &DeferredRenderer::render_doNothing>(this);
	m_EffectRenderDelegates.optixReflection.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optixShadow.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optixReflectionAndShadow.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optix.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.cuda.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.glslCompute.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
    m_EffectRenderDelegates.custom0.bind<DeferredRenderer, &DeferredRenderer::render_ReflectionCube>(this);
    m_EffectRenderDelegates.custom1.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
    m_EffectRenderDelegates.custom2.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
    m_EffectRenderDelegates.custom3.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
    m_EffectRenderDelegates.custom4.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
    m_EffectRenderDelegates.custom5.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.custom6.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.custom7.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.custom8.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.custom9.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.stencilFillPath.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.stencilStrokePath.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.coverFillPath.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.coverStrokePath.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.undefined.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.error.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);

	m_GBufferBucketCallbacks.uploadMaterial.bind<DeferredRenderer, &DeferredRenderer::uploadMaterial>(this);

	m_EffectSystemRef = m_RefRenderSys->getMainRef()->getEffectSystemPtr();


	g_ReflectionCamera.setViewportSize(glm::uvec2(512, 512));
	g_ReflectionCamera.setVerticalFieldOfView(90);
	g_ReflectionCamera.attachToObject(&g_ReflectionCameraObject);

	return true;
}

void DeferredRenderer::render(float32 dt, Scene * scene)
{
	if (m_RenderScene != scene || m_EffectSystemRef->dirty()) { //render a new scene -> update light / material library
		if (!initializeScene(scene)) return;		
	}
		
    if (!m_EffectSystemRef->renderSceneEffect(m_DeferredRenderingEffect, m_EffectRenderDelegates, cfgDeferredRenderTechniqueIdx)) {
		return;
	}

	/*
	//set back scene matrices
	update_matrices(m_RenderScene->getCamera(), m_GBuffer);
	m_GBuffer.sort();
	m_GBuffer.submit(m_GBufferBucketCallbacks);
	m_GBuffer.resetVariables();
	*/
	//m_RefRenderSys->getBackend()->setViewportSize(0, 0, scene->getCamera()->getViewportSize().x, scene->getCamera()->getViewportSize().y);
	//apply post processing effects
	EffectHandle hdl = m_EffectSystemRef->getFirstPostProcessEffect();
	while (hdl != InvalidEffectHandle) {
		m_EffectSystemRef->renderSceneEffect(hdl, m_EffectRenderDelegates);
		hdl = m_EffectSystemRef->getNextPostProcessEffect(hdl);
	}
}

void DeferredRenderer::shutdown()
{
}

bool DeferredRenderer::initializeScene(Scene * scene)
{
	m_RenderScene = nullptr;
	//check if we can get the effect from the EffectSystem
	m_DeferredRenderingEffect = m_EffectSystemRef->getSceneEffectByName(cfgDeferredRenderEffect);

	if (m_DeferredRenderingEffect == InvalidEffectHandle) {
		return false; 
	}

	// update handles
	m_ViewProjectionMatrixBufferHandle = m_EffectSystemRef->getViewProjectionBufferHandle();
	m_ModelMatrixBufferHandle = m_EffectSystemRef->getModelBufferHandle();
	m_LightsBufferHandle = m_EffectSystemRef->getLightBufferHandle();
	m_MaterialBufferHandle = m_EffectSystemRef->getMaterialBufferHandle();

	if (
		m_ViewProjectionMatrixBufferHandle == InvalidConstantBufferHandle ||
		m_ModelMatrixBufferHandle == InvalidConstantBufferHandle ||
		m_LightsBufferHandle == InvalidConstantBufferHandle || 
		m_MaterialBufferHandle == InvalidConstantBufferHandle
		) {
		LOG_ERROR(Renderer, "Not all buffer for uploading the necessary data found");
		return false;
	}

	//everything is good so far... update libraries
	m_RenderScene = scene;
	
	RenderBackend* backend = m_RefRenderSys->getBackend();

	command::CopyConstantBufferData* lightLibraryUpdateSize = m_GBuffer.addCommand<command::CopyConstantBufferData>(GBufferKey::Generate(), sizeof(uint32));
	lightLibraryUpdateSize->constantBuffer = m_LightsBufferHandle;
	*(uint32*)renderCommandPacket::GetAuxiliaryMemory(lightLibraryUpdateSize) =  scene->getLightCount();
	lightLibraryUpdateSize->data = renderCommandPacket::GetAuxiliaryMemory(lightLibraryUpdateSize);
	lightLibraryUpdateSize->size = sizeof(uint32);

	command::CopyConstantBufferData* lightLibraryUpdateData = m_GBuffer.appendCommand<command::CopyConstantBufferData, command::CopyConstantBufferData>(lightLibraryUpdateSize,0);
	lightLibraryUpdateData->constantBuffer = m_LightsBufferHandle;
	lightLibraryUpdateData->data = scene->getLightData();
	lightLibraryUpdateData->size = scene->getLightCount() * sizeof(Light);
	lightLibraryUpdateData->offset = sizeof(uint32) * 4; //*4 due to padding

	//upload material database
	command::CopyConstantBufferData* materialLibraryUpdateSize = m_GBuffer.addCommand<command::CopyConstantBufferData>(GBufferKey::Generate(), sizeof(uint32));
	materialLibraryUpdateSize->constantBuffer = m_MaterialBufferHandle;
	*(uint32*)renderCommandPacket::GetAuxiliaryMemory(materialLibraryUpdateSize) = scene->getMaterialCount();
	materialLibraryUpdateSize->data = renderCommandPacket::GetAuxiliaryMemory(materialLibraryUpdateSize);
	materialLibraryUpdateSize->size = sizeof(uint32);

	command::CopyConstantBufferData* materialLibraryUpdateData = m_GBuffer.appendCommand<command::CopyConstantBufferData, command::CopyConstantBufferData>(materialLibraryUpdateSize, 0);
	materialLibraryUpdateData->constantBuffer = m_MaterialBufferHandle;
	materialLibraryUpdateData->data = scene->getMaterialData();
	materialLibraryUpdateData->size = scene->getMaterialCount() * sizeof(Material);
	materialLibraryUpdateData->offset = sizeof(uint32) * 4; //*4 due to padding
	
	m_EffectSystemRef->cleanUp(); //reset dirty flag

	return true;
}

void DeferredRenderer::uploadMaterial(MaterialHandle hdl)
{//we have to think about a more elegant methode...
	const MaterialTexture& matTex = m_RefRenderSys->getMaterial(hdl);
	m_EffectSystemRef->uploadUniform(cfgMaterialDiffuseSampler, matTex.m_Textures[MaterialTexture::Diffuse]);
	m_EffectSystemRef->uploadUniform(cfgMaterialSpecularSampler, matTex.m_Textures[MaterialTexture::Specular]);
	m_EffectSystemRef->uploadUniform(cfgMaterialAmbientSampler, matTex.m_Textures[MaterialTexture::Ambient]);
	m_EffectSystemRef->uploadUniform(cfgMaterialEmissiveSampler, matTex.m_Textures[MaterialTexture::Emissive]);
	m_EffectSystemRef->uploadUniform(cfgMaterialHeightSampler, matTex.m_Textures[MaterialTexture::Height]);
	m_EffectSystemRef->uploadUniform(cfgMaterialNormalSampler, matTex.m_Textures[MaterialTexture::Normal]);
	m_EffectSystemRef->uploadUniform(cfgMaterialShininessSampler, matTex.m_Textures[MaterialTexture::Shininess]);
	m_EffectSystemRef->uploadUniform(cfgMaterialOpacitySampler, matTex.m_Textures[MaterialTexture::Opacity]);
	m_EffectSystemRef->uploadUniform(cfgMaterialDisplacementSampler, matTex.m_Textures[MaterialTexture::Displacement]);
	m_EffectSystemRef->uploadUniform(cfgMaterialLightSampler, matTex.m_Textures[MaterialTexture::Light]);
	m_EffectSystemRef->uploadUniform(cfgMaterialReflectionSampler, matTex.m_Textures[MaterialTexture::Reflection]);
	m_EffectSystemRef->uploadUniform(cfgMaterialMaterialIDSampler, matTex.m_GPUId);	

	m_EffectSystemRef->updateUniforms();
}

void DeferredRenderer::update_matrices(Camera* cam, GBufferBucket& bucket)
{
	glm::mat4 view = cam->getViewMatrix();
	glm::mat4 proj = cam->getProjectionMatrix();
	glm::vec3 eye = cam->getGameObject()->getPosition();

	m_RenderViewProjectionMatrices.m4_PreviousViewProjection = cam->getPreviousViewProjection();// m_RenderViewProjectionMatrices.m4_ViewProjection; // 
	m_RenderViewProjectionMatrices.m4_ViewProjection = proj * view;
	m_RenderViewProjectionMatrices.m4_ViewProjectionI = glm::inverse(proj * view);
	m_RenderViewProjectionMatrices.m4_Projection = proj;
	m_RenderViewProjectionMatrices.m4_ProjectionI = glm::inverse(proj);
	m_RenderViewProjectionMatrices.m4_View = view;
	m_RenderViewProjectionMatrices.m4_ViewIT = glm::transpose(glm::inverse(view));
	m_RenderViewProjectionMatrices.v3_EyePos = eye;
	m_RenderViewProjectionMatrices.dummy = 0.0;
	m_RenderViewProjectionMatrices.v3_ViewDirection = glm::normalize(cam->getGameObject()->getForward());
	m_RenderViewProjectionMatrices.v2_ClippingPlanes = cam->getClippingPlanes();
	glm::ivec2 viewportSize = cam->getViewportSize();
	m_RenderViewProjectionMatrices.iv2_ViewportSize = viewportSize;

	m_RefRenderSys->getBackend()->setViewportSize(0, 0, viewportSize.x, viewportSize.y);

	command::CopyConstantBufferData* cmd = bucket.addCommand<command::CopyConstantBufferData>(GBufferKey::Generate(), sizeof(ViewProjectionMatrices));
	cmd->constantBuffer = m_ViewProjectionMatrixBufferHandle;
	cmd->data = (const void*)&m_RenderViewProjectionMatrices;
	cmd->size = sizeof(ViewProjectionMatrices);
}

void DeferredRenderer::RenderSceneNode(const SceneNode * sceneNode, uint32 count, void * instance) {
	ParallelData<GBufferBucket>* parallelData = (ParallelData<GBufferBucket>*)instance;
	for (uint32 i = 0; i < count; ++i) {
		parallelData->renderer->renderSceneNode(sceneNode + i, *parallelData->bucket);
	}
}

void DeferredRenderer::renderSceneNode(const SceneNode * sceneNode, GBufferBucket& bucket)
{
	if (!sceneNode || sceneNode->m_Disabled) return;

	ModelMatrices modelMatrices;
	const GameObject* gameObject = sceneNode->m_GameObject;
	float32 z = 0.0f;
	if (gameObject != nullptr) { //a game object associated -> dynamic object
		z = gameObject->getPosition().z;
		gameObject->getRotation();

		modelMatrices.m4_Model = glm::translate(gameObject->getPosition()) * glm::mat4_cast(gameObject->getRotation()) * glm::scale(gameObject->getScaling());
	}
	else {
		modelMatrices.m4_Model = glm::mat4(1.0);
	}

	modelMatrices.m4_ModelIT = glm::transpose(glm::inverse(modelMatrices.m4_Model));
	modelMatrices.m4_ModelView = m_RenderViewProjectionMatrices.m4_View * modelMatrices.m4_Model;
	modelMatrices.m4_ModelViewProjection = m_RenderViewProjectionMatrices.m4_ViewProjection * modelMatrices.m4_Model;
	modelMatrices.m4_ModelViewIT = glm::transpose(glm::inverse(modelMatrices.m4_ModelView));

    for (uint32 i = 0; i < sceneNode->m_Mesh.m_NumSubMeshes; ++i) {
		command::CopyConstantBufferData* matrixUpload = bucket.addCommand<command::CopyConstantBufferData>(GBufferKey::Generate(z, sceneNode->m_Mesh.m_Materials[i], 1), sizeof(ModelMatrices));
		matrixUpload->constantBuffer = m_ModelMatrixBufferHandle; //get the handle where to upload the data
		*(ModelMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload) = modelMatrices;
		matrixUpload->data = (ModelMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload);
		matrixUpload->size = sizeof(ModelMatrices);
				
		command::DrawGeometry* geometry = bucket.addCommand<command::DrawGeometry>(GBufferKey::Generate(z, sceneNode->m_Mesh.m_Materials[i], 1), 0);
		geometry->geometryHandle = sceneNode->m_Mesh.m_Geometry;
		geometry->startIndex = sceneNode->m_Mesh.m_Submesh[i].startIndex;
		geometry->indexCount = sceneNode->m_Mesh.m_Submesh[i].indexCount;
	}
}

ENDNAMESPACE
