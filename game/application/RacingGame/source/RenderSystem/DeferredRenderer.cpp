#include "RenderSystem/DeferredRenderer.h"

#include "CompileTime/Templates.h"

#include "RenderSystem/RenderCommand.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/RenderSystem.h"

#include "Utilities/FloatCompressor.h"
#include "Utilities/Number.h"
#include "Utilities/Random.h"

#include "Multithreading/JobScheduler.h"
#include "Multithreading/ParallelFor.h"

#include "Logging.h"

BEGINNAMESPACE

DeferredRenderer::GBufferKey DeferredRenderer::GenerateGBufferKey(const BoundingBox& aabb, MaterialHandle material, uint8 pass) {
	static const uint8 MAX_PASS = MaxUnsignedWithNBits<uint32, _GenGBufferKey::PassCount>::value;
	ASSERT(pass <= MAX_PASS, "The G-Buffer is only capable of %d render passes", MAX_PASS);

	union {	_GenGBufferKey g; GBufferKey k; };

	g.preSort = 0;
	g.material = material.index;
	
	float32 computeDepthHere = 0.0f;
	g.depth = Float16Compressor::compress(computeDepthHere);

	CutToNBits<GBufferKey, _GenGBufferKey::PassCount> cutter;
	cutter.set = pass;
	g.pass = (cutter.get);

	g.postSort = 0;
	return k;
}

Random<float32> rnd;

DeferredRenderer::DeferredRenderer(RenderSystem* renderSys) : 
	m_RefRenderSys(renderSys),
	m_GBuffer(40000) //40000 commands can be stored in this queueu
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

	return true;
}

extern GeometryHandle demo_Cube; //DEMO CUBE
extern ShaderProgramHandle demo_Shader; //DEMO PROGRAM
void DeferredRenderer::render(float32 dt, Scene * scene)
{
	//command::ClearTarget* clTgt = m_GBuffer.addCommand<command::ClearTarget>(1, 0);
	//clTgt->renderTarget = m_GBufferTarget;
	command::ClearScreen* cls = m_GBuffer.addCommand<command::ClearScreen>(0, 0);
	command::ActivateShader* aSh = m_GBuffer.addCommand<command::ActivateShader>(1, 0);
	aSh->shaderProgram = demo_Shader;
	command::DrawGeometry* rdc = m_GBuffer.addCommand<command::DrawGeometry>(2, 0);
	rdc->geometryHandle = demo_Cube;
	rdc->indexCount = 0; //0 -> all indices
	rdc->startIndex = 0;

	m_GBuffer.submit();
}

void DeferredRenderer::shutdown()
{
}

ENDNAMESPACE