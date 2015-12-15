#include "RenderSystem/DeferredRenderer.h"

#include "CompileTime/Templates.h"

#include "ObjectSystem/GameObject.h"

#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/Scene.h"
#include "RenderSystem/Camera.h"

#include "Utilities/FloatCompressor.h"
#include "Utilities/Number.h"
#include "Utilities/Random.h"

#include "Multithreading/JobScheduler.h"
#include "Multithreading/ParallelFor.h"

#include "Logging.h"

#include "Configuration/ConfigSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxGBufferCommands("maxGBufferCommands", "Sets the maximum number of commands per frame for the G-Buffer", 40000);

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
	m_RefRenderSys(renderSys),
	m_GBuffer(cfgMaxGBufferCommands)
{}

DeferredRenderer::~DeferredRenderer()
{
	shutdown();
}

bool DeferredRenderer::initialize()
{
	command::ScreenSetClearColor* clsColor = m_GBuffer.addCommand<command::ScreenSetClearColor>(0, 0);
	clsColor->r = rnd() * rnd(); clsColor->g = rnd(); clsColor->b = rnd(); clsColor->a = 1.0f;

	// A deferred renderer renders into different render targets
	RenderBackend* backend = m_RefRenderSys->getBackend();

	RenderTargetLayout GBufferLayout = {
		1920, 1080, -1, //-1 -> no depth as it is 2D
		4, //4 textures
		{
			RenderTargetLayout::COLOR_ATTACHMENT | RenderTargetLayout::_2D,
			RenderTargetLayout::COLOR_ATTACHMENT | RenderTargetLayout::_2D,
			RenderTargetLayout::COLOR_ATTACHMENT | RenderTargetLayout::_2D,
			RenderTargetLayout::COLOR_ATTACHMENT | RenderTargetLayout::_2D
		},
		{
			RenderTextureType::RGBA8,
			RenderTextureType::RGBA8,
			RenderTextureType::F32,
			RenderTextureType::RGBA8
		},
		1, 
		{
			RenderTargetLayout::DEPTH_STENCIL_ATTACHMENT
		},
		{
			RenderBufferType::DEPTH_STENCIL
		}
	};

    m_GBufferTarget = backend->createRenderTarget(GBufferLayout);

    ConstantBufferSpec sceneMatrixBufferSpec = {
        4
    };
    m_SceneMatrixBuffer = backend->createConstantBuffer(sceneMatrixBufferSpec);

	ConstantBufferSpec objectMatrixBufferSpec = {
		2
	};
    m_ObjectMatrixBuffer = backend->createConstantBuffer(objectMatrixBufferSpec);

	return true;
}

struct SceneMatrices_t{
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;
} SceneMatrices;


extern ShaderProgramHandle demo_Shader;
void DeferredRenderer::render(float32 dt, Scene * scene)
{
	/*
	command::ClearTarget* clTgt = m_GBuffer.addCommand<command::ClearTarget>(0, 0);
	clTgt->renderTarget = m_GBufferTarget;
	*/

	//;
	//clTgt->renderTarget = m_GBufferTarget;
    scene->getCamera()->update();
    SceneMatrices._viewMatrix = scene->getCamera()->getViewMatrix();
    SceneMatrices._projMatrix = scene->getCamera()->getProjectionMatrix();

	command::ClearScreen* cls = m_GBuffer.addCommand<command::ClearScreen>(0, 0);
	command::ActivateShader* aSh = m_GBuffer.addCommand<command::ActivateShader>(1, 0);
    aSh->shaderProgram = demo_Shader;

    command::CopyConstantBufferData* cBuf = m_GBuffer.addCommand<command::CopyConstantBufferData>(1, sizeof(SceneMatrices_t));
    *(SceneMatrices_t*)renderCommandPacket::GetAuxiliaryMemory(cBuf) = SceneMatrices;
    cBuf->constantBuffer = m_SceneMatrixBuffer;
    cBuf->data = renderCommandPacket::GetAuxiliaryMemory(cBuf);
    cBuf->size = sizeof(SceneMatrices_t);

    JobScheduler::Wait(
        parallel_for(scene->getSceneNodes(), scene->getSceneNodeCount(), &DeferredRenderer::RenderSceneNode, this)
    );

	m_GBuffer.sort();
	m_GBuffer.submit();
}

void DeferredRenderer::shutdown()
{
}

void DeferredRenderer::renderSceneNode(const SceneNode * sceneNode)
{
	if (!sceneNode || sceneNode->m_Disabled) return;
	struct ObjectMatrices {
		glm::mat4x4 modelMatrix;
	} objectMatrices;
	const GameObject* gameObject = sceneNode->m_GameObject;
	float32 z = 0.0f;
	if (gameObject != nullptr) { //a game object associated -> probably dynamic object
		z = gameObject->getPosition().z;
		gameObject->getRotation();

		objectMatrices.modelMatrix = glm::translate(gameObject->getPosition()) * glm::mat4_cast(gameObject->getRotation()) * glm::scale(gameObject->getScaling());
	}
	else {
        objectMatrices.modelMatrix = glm::mat4(1.0);
	}

    for (uint32 i = 0; i < sceneNode->m_Mesh.m_NumSubMeshes; ++i) {

		command::CopyConstantBufferData* matrixUpload = m_GBuffer.addCommand<command::CopyConstantBufferData>(GenerateGBufferKey(z, sceneNode->m_Mesh.m_Materials[i], 1), sizeof(ObjectMatrices));
		matrixUpload->constantBuffer = m_ObjectMatrixBuffer;
		*(ObjectMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload) = objectMatrices;
		matrixUpload->data = (ObjectMatrices*)renderCommandPacket::GetAuxiliaryMemory(matrixUpload);
		matrixUpload->size = sizeof(ObjectMatrices);

		command::DrawGeometry* geometry = m_GBuffer.appendCommand<command::DrawGeometry, command::CopyConstantBufferData>(matrixUpload, 0);
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
