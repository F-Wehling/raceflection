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

ConfigSettingUint32 cfgMaxGBufferCommands("maxGBufferCommands", "Sets the maximum number of commands per frame for the G-Buffer", 40000);
ConfigSettingAnsichar cfgDeferredRenderEffect("deferredRenderEffect", "Sets the name of the deferred-renderer effect", "deferredRendering");

DeferredRenderer::GBufferKey DeferredRenderer::GenerateGBufferKey(float32 depth, MaterialHandle material, uint8 pass) {
	static const uint8 MAX_PASS = MaxUnsignedWithNBits<uint32, _GenGBufferKey::PassCount>::value;
	ASSERT(pass <= MAX_PASS, "The G-Buffer is only capable of %d render passes", MAX_PASS);

	union {	_GenGBufferKey g; GBufferKey k; };

	g.preSort = 0;
	g.material = material.index;
	
	g.depth = Float16Compressor::compress(depth);

	CutToNBits<GBufferKey, _GenGBufferKey::PassCount> cutter;
	cutter.set = pass;
	g.pass = (cutter.get);

	g.postSort = 0;
	return k;
}

Random<float32> rnd;

DeferredRenderer::DeferredRenderer(RenderSystem* renderSys) : 
	m_DeferredRenderingEffect(InvalidEffectHandle),
	m_RefRenderSys(renderSys),
	m_GBuffer(cfgMaxGBufferCommands)
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

bool DeferredRenderer::renderSceneGraphShaded()
{
	/*
	command::ClearScreen* cls = m_GBuffer.addCommand<command::ClearScreen>(0, 0);
	command::ActivateShader* aSh = m_GBuffer.addCommand<command::ActivateShader>(1, 0);
	aSh->shaderProgram = demo_Shader;

	command::CopyConstantBufferData* cBuf = m_GBuffer.addCommand<command::CopyConstantBufferData>(1, sizeof(SceneMatrices_t));
	*(SceneMatrices_t*)renderCommandPacket::GetAuxiliaryMemory(cBuf) = SceneMatrices;
	cBuf->constantBuffer = m_SceneMatrixBuffer;
	cBuf->data = renderCommandPacket::GetAuxiliaryMemory(cBuf);
	cBuf->size = sizeof(SceneMatrices_t);

	JobScheduler::Wait(
		parallel_for(m_RenderScene->getSceneNodes(), m_RenderScene->getSceneNodeCount(), &DeferredRenderer::RenderSceneNode, this)
		);

	m_GBuffer.sort();
	m_GBuffer.submit();
	*/

	JobScheduler::Wait(
		parallel_for(m_RenderScene->getSceneNodes(), m_RenderScene->getSceneNodeCount(), &DeferredRenderer::RenderSceneNode, this)
		);

	m_GBuffer.sort();
	m_GBuffer.submit();

	return true;
}

bool DeferredRenderer::renderModeNotImplemented()
{
	LOG_ERROR(Renderer, "This render mode isn't implemented");
	return false;
}

bool DeferredRenderer::initialize()
{
	command::ScreenSetClearColor* clsColor = m_GBuffer.addCommand<command::ScreenSetClearColor>(0, 0);
	clsColor->r = rnd() * rnd(); clsColor->g = rnd(); clsColor->b = rnd(); clsColor->a = 1.0f;

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
	m_EffectRenderDelegates.sceneGraphDebugQuad0.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad1.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad2.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugQuad3.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugScene.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.sceneGraphDebugSceneLines.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.doNothing.bind<DeferredRenderer, &DeferredRenderer::render_doNothing>(this);
	m_EffectRenderDelegates.optixReflection.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optixShadow.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optixReflectionAndShadow.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.optix.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.cuda.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.glslCompute.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
	m_EffectRenderDelegates.custom0.bind<DeferredRenderer, &DeferredRenderer::renderModeNotImplemented>(this);
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

	m_EffectSystemRef = m_RefRenderSys->getMainRef()->getEffectSystemPtr();

	return true;
}

void DeferredRenderer::render(float32 dt, Scene * scene)
{
	m_RenderScene = scene;
	if (m_DeferredRenderingEffect == InvalidEffectHandle) {
		//check if we can get the effect from the EffectSystem
		m_DeferredRenderingEffect = m_EffectSystemRef->getSceneEffectByName(cfgDeferredRenderEffect);
		return; //retry next frame
	}

	scene->getCamera()->update();
	glm::mat4 view = scene->getCamera()->getViewMatrix();
	glm::mat4 proj = scene->getCamera()->getProjectionMatrix();
	glm::vec3 eye = scene->getCamera()->getGameObject()->getPosition();

	ViewProjectionMatrices viewProjMatrices{
		proj * view,
		proj,
		view,
		glm::transpose(glm::inverse(view)),
		eye,
		0.0
	};
	m_RenderViewProjectionMatrices = viewProjMatrices;
	m_EffectSystemRef->uploadViewProjectionMatrices(&viewProjMatrices);

	if (!m_EffectSystemRef->renderSceneEffect(m_DeferredRenderingEffect, m_EffectRenderDelegates)) {
		m_RenderScene = nullptr;
		return;
	}
	m_RenderScene = nullptr;
}

void DeferredRenderer::shutdown()
{
}

void DeferredRenderer::renderSceneNode(const SceneNode * sceneNode)
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

    for (uint32 i = 0; i < sceneNode->m_Mesh.m_NumSubMeshes; ++i) {
		/*
		command::CopyConstantBufferData* matrixUpload = m_GBuffer.addCommand<command::CopyConstantBufferData>(GenerateGBufferKey(z, sceneNode->m_Mesh.m_Materials[i], 1), sizeof(ObjectMatrices));
		matrixUpload->constantBuffer = m_ObjectMatrixBuffer;
		*(ObjectMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload) = objectMatrices;
		matrixUpload->data = (ObjectMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload);
		matrixUpload->size = sizeof(ObjectMatrices);

		command::DrawGeometry* geometry = m_GBuffer.appendCommand<command::DrawGeometry, command::CopyConstantBufferData>(matrixUpload, 0);
		geometry->geometryHandle = sceneNode->m_Mesh.m_Geometry;
		geometry->startIndex = sceneNode->m_Mesh.m_Submesh[i].startIndex;
		geometry->indexCount = sceneNode->m_Mesh.m_Submesh[i].indexCount;
		*/
		command::CopyConstantBufferData* matrixUpload = m_GBuffer.addCommand<command::CopyConstantBufferData>(GenerateGBufferKey(z, sceneNode->m_Mesh.m_Materials[i], 1), sizeof(ModelMatrices));
		matrixUpload->constantBuffer = m_EffectSystemRef->getModelBufferHandle(); //get the handle where to upload the data
		*(ModelMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload) = modelMatrices;
		matrixUpload->data = (ModelMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload);
		matrixUpload->size = sizeof(ModelMatrices);

		command::DrawGeometry* geometry = m_GBuffer.addCommand<command::DrawGeometry>(GenerateGBufferKey(z, sceneNode->m_Mesh.m_Materials[i], 1), 0);
		geometry->geometryHandle = sceneNode->m_Mesh.m_Geometry;
		geometry->startIndex = sceneNode->m_Mesh.m_Submesh[i].startIndex;
		geometry->indexCount = sceneNode->m_Mesh.m_Submesh[i].indexCount;
	}
}

void DeferredRenderer::RenderSceneNode(const SceneNode * sceneNode, uint32 count, void * instance) {
	DeferredRenderer* renderer = (DeferredRenderer*)instance;
	for (uint32 i = 0; i < count; ++i) {
		renderer->renderSceneNode(sceneNode + i);
	}
}

ENDNAMESPACE
