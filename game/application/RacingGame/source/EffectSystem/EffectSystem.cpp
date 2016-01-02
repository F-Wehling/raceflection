#include "EffectSystem/EffectSystem.h"

#include "PackageSpec.h"
#include "EffectSpec.h"

#include "FxLib.h"
#include "FxLibEx.h"
#include "FxParser.h"

#include "Logging.h"

#include "Configuration/ConfigSettings.h"

#include "Container/Array.h"

#include "Main.h"

#include "RenderSystem/RenderTypes.h"
#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/RenderBackend.h"
#include "RenderSystem/ConstantBuffer.h"

//FORWARD DECLARE GLEW
extern "C" {
	extern unsigned int glewInit(void);
	extern const unsigned char * glewGetErrorString(unsigned int error);
}
//END FORWARD DECLARE GLEW 

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxMaterialEffectContainer("MaxEffectContainer", "Set the maximum number of material-effect container.", 50);
ConfigSettingUint32 cfgMaxSceneEffectContainer("MaxEffectContainer", "Set the maximum number of material-effect container.", 20);

ConfigSettingAnsichar cfgViewProjectionMatrixBlockName("effect.viewProjectionMatricesBlockName", "Set the block-name for the ViewProjection-matrices.", "ViewProjectionMatrices");
ConfigSettingAnsichar cfgModelMatrixBlockName("effect.modelMatricesBlockName", "Set the block-name for the Model-Matrices", "ModelMatrices");

extern ConfigSettingUint32 cfgWindowWidth;
extern ConfigSettingUint32 cfgWindowHeight;

EffectHandle InvalidEffectHandle = { EffectHandle::_Handle_type(-1), EffectHandle::_Handle_type(-1) };

EffectSystem* g_EffectSystemInstance;
EffectSystem::EffectSystem(Main* mainRef) : 
	m_CurrentNumOfMaterialEffects(0),
	m_CurrentNumOfSceneEffects(0),
	m_MainRef(mainRef),
	m_ViewProjectionMatrices(nullptr),
	m_ViewProjMatHandle(InvalidConstantBufferHandle),
	m_ModelMatrices(nullptr),
	m_ModelMatHandle(InvalidConstantBufferHandle)
{
	g_EffectSystemInstance = nullptr;
}

bool EffectSystem::initialize(uint32 api) {

	switch (api) {
	case RenderEngineType::OpenGL:
		uint32 g = glewInit(); //the opengl part needs to initialize
		if (g != 0) {
			LOG_ERROR(Effect, "Glew-init failed: %s.", glewGetErrorString(g));
			return false;
		}
		break;
	}

	m_RenderAPI = api;

	m_MaterialEffectContainer.resize(cfgMaxMaterialEffectContainer);
	m_SceneEffectContainer.resize(cfgMaxSceneEffectContainer);

	nvFX::setErrorCallback(&EffectSystem::nvFXErrorCallback);
	nvFX::setMessageCallback(&EffectSystem::nvFXMessageCallback);
	nvFX::setIncludeCallback(&EffectSystem::nvFXIncludeCallback);
		
	return true;
}

void EffectSystem::shutdown()
{
}

bool EffectSystem::createMaterialEffect(const ansichar* name, const ansichar* effectSource)
{
	ASSERT(m_CurrentNumOfMaterialEffects < cfgMaxMaterialEffectContainer, "Maximum number of effects reached!");
	ASSERT(g_EffectSystemInstance == nullptr, "The effect compilation should only be called by the main-thread");
	g_EffectSystemInstance = this;
	

	nvFX::IContainer* container = createEffect(name, effectSource, "MATERIAL");
	if (container != nullptr) {
		m_MaterialEffectContainer[m_CurrentNumOfMaterialEffects++] = container;
	}
	g_EffectSystemInstance = nullptr;
	return container != nullptr;
}

bool EffectSystem::createSceneEffect(const ansichar * name, const ansichar * effectSource)
{
	ASSERT(m_CurrentNumOfSceneEffects < cfgMaxSceneEffectContainer, "Maximum number of effects reached!");
	ASSERT(g_EffectSystemInstance == nullptr, "The effect compilation should only be called by the main-thread");
	g_EffectSystemInstance = this;
	
	nvFX::IContainer* container = createEffect(name, effectSource, "SCENE");
	if (container != nullptr) {
		m_SceneEffectContainer[m_CurrentNumOfSceneEffects++] = container;
	}
	g_EffectSystemInstance = nullptr;
	return container != nullptr;
}

void EffectSystem::createEffectLibraryFromPackageSpec(const PackageSpec * spec)
{
	updateEffectLibraryFromPackageSpec(spec);
}

void EffectSystem::updateEffectLibraryFromPackageSpec(const PackageSpec * spec)
{
	reset();
	for (uint32 i = 0; i < spec->getEffectCount(); ++i) {
		const EffectSpec* effSpec = spec->getEffectSpec(i);
		switch (effSpec->effectType) {
		case EffectType::FXHeader:
			m_HeaderCode[effSpec->name] = effSpec->effectSource;
			break;
		case EffectType::MATERIAL_FX:
			m_MaterialFXSourceCode[effSpec->name] = effSpec->effectSource;
			break;
		case EffectType::SCENE_FX:
			m_SceneFXSourceCode[effSpec->name] = effSpec->effectSource;
			break;
		default: break;
		}
	}
	
	for (SourceMap_t::value_type& source : m_MaterialFXSourceCode) {
		LOG_INFO(Effect, "Compile %s", source.first.c_str());
		if (!createMaterialEffect(source.first.c_str(), source.second.c_str())) {
			LOG_ERROR(Effect, "Compilation failed.");
		}
	}

	for (SourceMap_t::value_type& source : m_SceneFXSourceCode) {
		LOG_INFO(Effect, "Compile %s", source.first.c_str());
		if (!createSceneEffect(source.first.c_str(), source.second.c_str())) {
			LOG_ERROR(Effect, "Compilation failed.");
		}
	}	

	nvFX::getResourceRepositorySingleton()->setParams(0, 0, cfgWindowWidth, cfgWindowHeight, 1, 0, 0);
	if (!nvFX::getResourceRepositorySingleton()->validateAll()) {
		LOG_ERROR(Effect, "Invalid resource specification");
		return;
	}

	if (!validateAndSetupOverrides()) {
		LOG_ERROR(Effect, "Can't setup overrides!");
		return;
	}

	for (nvFX::IContainer* container : m_MaterialEffectContainer) {
		if (!container) break;
		for (int32 t = 0; ; ++t) {
			nvFX::ITechnique* tech = container->findTechnique(t);
			if (!tech) break;
			if (!tech->validate() || !tech->validateResources()) return;
		}
	}

	for (nvFX::IContainer* container : m_SceneEffectContainer) {
		if (!container) break;
		for (int32 t = 0; ; ++t) {
			nvFX::ITechnique* tech = container->findTechnique(t);
			if (!tech) break;
			if (!tech->validate() || !tech->validateResources()) return;

			tech->getExInterface();
		}
	}

	nvFX::getResourceRepositorySingleton()->setParams(0, 0, cfgWindowWidth, cfgWindowHeight, 1, 0, 0);
	if (!nvFX::getResourceRepositorySingleton()->updateValidated()) {
		LOG_ERROR(Effect, "Invalid resource specification");
		return;
	}
}

EffectHandle EffectSystem::getSceneEffectByName(const ansichar * name)
{
	return getEffectByName(name, m_SceneEffectContainer, m_CurrentNumOfSceneEffects);
}

EffectHandle EffectSystem::getMaterialEffectByName(const ansichar * name)
{
	return getEffectByName(name, m_MaterialEffectContainer, m_CurrentNumOfMaterialEffects);
}

bool EffectSystem::renderSceneEffect(EffectHandle handle, EffectRenderDelegate & fn)
{
	if (handle == InvalidEffectHandle) return false;
	return renderEffect(handle, fn, m_SceneEffectContainer);
}

bool EffectSystem::renderEffect(EffectHandle handle, EffectRenderDelegate & fn, EffectContainers_t& containerMgr)
{
	nvFX::IContainer* effect = containerMgr[handle.index];
	nvFX::ITechnique* tech = effect->findTechnique(0); //TODO: different techniques than 0
	
	nvFX::PassInfo pr;
	int32 np = tech->getNumPasses();
	bool successfull = true;
	for (int32 i = 0; i<np && successfull; i++)
	{
		nvFX::IPass* pass = tech->getPass(i);
		pass->execute(&pr);
		switch (pr.renderingMode)
		{
		case nvFX::PASS_DONE:
			break;
		case nvFX::RENDER_SCENEGRAPH_SHADED:
			successfull = fn.sceneGraphShaded();
			break;
		case nvFX::RENDER_SCENEGRAPH_NOSHADING:
			successfull = fn.sceneGraphNoShading();
			break;
		case nvFX::RENDER_FULLSCREEN_QUAD:
			successfull = fn.fullscreenQuad(); 
			break;
		case nvFX::RENDER_SCENEGRAPH_SHADED_OPAQUE_ONLY:
			successfull = fn.sceneGraphShadedOpaqueOnly();
			break;
		case nvFX::RENDER_SCENEGRAPH_SHADED_TRANSPARENT_ONLY:
			successfull = fn.sceneGraphShadedTransparentOnly();
			break;
		case nvFX::RENDER_SCENEGRAPH_NOSHADING_OPAQUE_ONLY:
			successfull = fn.sceneGraphNoShadingOpaqueOnly();
			break;
		case nvFX::RENDER_SCENEGRAPH_NOSHADING_TRANSPARENT_ONLY:
			successfull = fn.sceneGraphNoShadingTransparentOnly();
			break;
		case nvFX::RENDER_SCENEGRAPH_OUTLINES_ONLY:
			successfull = fn.sceneGraphOutliesOnly();
			break;
		case nvFX::RENDER_DEBUG_QUAD0:
			successfull = fn.sceneGraphDebugQuad0();
			break;
		case nvFX::RENDER_DEBUG_QUAD1:
			successfull = fn.sceneGraphDebugQuad1();
			break;
		case nvFX::RENDER_DEBUG_QUAD2:
			successfull = fn.sceneGraphDebugQuad2();
			break;
		case nvFX::RENDER_DEBUG_QUAD3:
			successfull = fn.sceneGraphDebugQuad3();
			break;
		case nvFX::RENDER_DEBUG_SCENE:
			successfull = fn.sceneGraphDebugScene();
			break;
		case nvFX::RENDER_DEBUG_SCENE_LINES:
			successfull = fn.sceneGraphDebugSceneLines();
			break;
		case nvFX::DO_NOTHING:
			successfull = fn.doNothing();
			break;
		case nvFX::RENDER_OPTIX_REFLECTION:
			successfull = fn.optixReflection();
			break;
		case nvFX::RENDER_OPTIX_SHADOW:
			successfull = fn.optixShadow();
			break;
		case nvFX::RENDER_OPTIX_REFLECTION_AND_SHADOW:
			successfull = fn.optixReflectionAndShadow();
			break;
		case nvFX::RENDER_OPTIX:
			successfull = fn.optix();
			break;
		case nvFX::RUN_CUDA:
			successfull = fn.cuda();
			break;
		case nvFX::RUN_GLSLCOMPUTE:
			successfull = fn.glslCompute();
			break;
		case nvFX::RENDER_CUSTOM0: ///< custom rendering mode :for any purpose...
			successfull = fn.custom0();
			break;
		case nvFX::RENDER_CUSTOM1:
			successfull = fn.custom1();
			break;
		case nvFX::RENDER_CUSTOM2:
			successfull = fn.custom2();
			break;
		case nvFX::RENDER_CUSTOM3:
			successfull = fn.custom3();
			break;
		case nvFX::RENDER_CUSTOM4:
			successfull = fn.custom4();
			break;
		case nvFX::RENDER_CUSTOM5:
			successfull = fn.custom5();
			break;
		case nvFX::RENDER_CUSTOM6:
			successfull = fn.custom6();
			break;
		case nvFX::RENDER_CUSTOM7:
			successfull = fn.custom7();
			break;
		case nvFX::RENDER_CUSTOM8:
			successfull = fn.custom8();
			break;
		case nvFX::RENDER_CUSTOM9:
			successfull = fn.custom9();
			break;
		// render modes for NVIDIA Path rendering extension
		case nvFX::RENDER_STENCILFILLPATH:
			successfull = fn.stencilFillPath();
			break;
		case nvFX::RENDER_STENCILSTROKEPATH:
			successfull = fn.stencilStrokePath();
			break;
		case nvFX::RENDER_COVERFILLPATH:
			successfull = fn.coverFillPath();
			break;
		case nvFX::RENDER_COVERSTROKEPATH:
			successfull = fn.coverStrokePath();
			break;
		case nvFX::RENDERING_UNDEFINED:
			successfull = fn.undefined();
			break;
		case nvFX::PASS_ERROR:
			successfull = fn.error();
			break;
		default:
			break;
		}
	}

	return successfull;
}

void EffectSystem::uploadViewProjectionMatrices(const void * data)
{
	if (m_ViewProjectionMatrices) {
		void* pdata = nullptr;
		if (!m_ViewProjectionMatrices->mapBuffer(&pdata)) return;
		std::memcpy(pdata, data, sizeof(ViewProjectionMatrices));
		m_ViewProjectionMatrices->unmapBuffer();
	}
}

void EffectSystem::uploadModelMatrices(const void * data)
{
	if (m_ModelMatrices) {
		void* pdata;
		if (!m_ModelMatrices->mapBuffer(&pdata)) return;
		std::memcpy(pdata, data, sizeof(ModelMatrices));
		m_ModelMatrices->unmapBuffer();
	}
}

EffectHandle EffectSystem::getEffectByName(const ansichar * name, EffectContainers_t & containerMgr, uint32 length)
{
	EffectHandle hdl = InvalidEffectHandle;
	for (uint32 i = 0; i < length; ++i) {
		if (strcmp(containerMgr[i]->getName(), name) == 0) {
			hdl.index = i;
			hdl.generation = 0;
			break;
		}
	}
	return hdl;
}

nvFX::IContainer * EffectSystem::createEffect(const ansichar * name, const ansichar * source, const ansichar * type)
{
	nvFX::IContainer* container = nvFX::IContainer::create();
	if (!nvFX::loadEffect(container, source)) {
		g_EffectSystemInstance = nullptr;
		LOG_ERROR(Effect, "Effect-Parsing failed.");
		return false;
	}
	container->setName(name);
	LOG_INFO(Effect, "Loaded new %s effect: %s", type, container->getName());

	bool anyValid = false;
	for (int32 techIdx = 0; techIdx < container->getNumTechniques(); ++techIdx) {
		nvFX::ITechnique* tech = container->findTechnique(techIdx);

		if (tech && tech->validate()) { //valid technique
			anyValid = true;
		}
	}
	if (!anyValid) {
		return nullptr;
	}

	createGlobalsFromEffectContainer(container);
	return container;
}

void EffectSystem::createGlobalsFromEffectContainer(nvFX::IContainer * effectContainer)
{
	RenderBackend* backend = m_MainRef->getRenderSystemPtr()->getBackend(); //Get the render backend for creating backend-objects
	int32 cstBufferIdx = 0;
	nvFX::ICstBuffer* cstBuffer = nullptr;
	while ((cstBuffer = effectContainer->findCstBuffer(cstBufferIdx++))) {
		if (!m_ViewProjectionMatrices && strcmp(cstBuffer->getName(), cfgViewProjectionMatrixBlockName) == 0) {
			m_ViewProjMatHandle =  backend->createConstantBuffer(cstBuffer, sizeof(ViewProjectionMatrices));
			if (m_ViewProjMatHandle != InvalidConstantBufferHandle) {
				m_ViewProjectionMatrices = cstBuffer;
			}
			continue;
		}
		if (!m_ModelMatrices && strcmp(cstBuffer->getName(), cfgModelMatrixBlockName) == 0) {
			m_ModelMatHandle = backend->createConstantBuffer(cstBuffer, sizeof(ModelMatrices));
			if (m_ModelMatHandle != InvalidConstantBufferHandle) {
				m_ModelMatrices = cstBuffer;
			}
			continue;
		}

		LOG_INFO(Effect, "Found a constant buffer: %s", cstBuffer->getName());
	}

	int32 uniformIdx = 0;
	nvFX::IUniform* uniform = nullptr;
	while ((uniform = effectContainer->findUniform(uniformIdx++))) {
		LOG_INFO(Effect, "Found a uniform: %s", uniform->getName());
	}

	int32 resourceIdx = 0;
	nvFX::IResource* resource = nullptr;
	while ((resource = effectContainer->findResource(resourceIdx++))) {
		LOG_INFO(Effect, "Found a resource: %s", resource->getName());
	}

	int32 samplerStateIdx = 0;
	nvFX::ISamplerState* samplerState = nullptr;
	while ((samplerState = effectContainer->findSamplerState(samplerStateIdx++))) {
		LOG_INFO(Effect, "Found a samplerState: %s", samplerState->getName());
	}
}

bool EffectSystem::validateAndSetupOverrides()
{
	bool success = true;

	//
	// gather techniques of materials that we want to instanciate
	// depending on scene effects
	//
	DynArray<nvFX::ITechnique*> techs;
	for (nvFX::IContainer* effect : m_MaterialEffectContainer) {
		if (!effect) break;
		int32 Sz = effect->getNumTechniques();
		for (int32 t = 0; t < Sz; t++) {
			techs.push_back(effect->findTechnique(t));
		}
	}
	//
	// validate scene-level techniques
	//
	for (nvFX::IContainer* techEffect : m_SceneEffectContainer) {
		if (!techEffect) break;
		nvFX::ITechnique* scTech = nullptr;
		for (int32 t = 0; ; t++)
		{
			scTech = techEffect->findTechnique(t);
			if (scTech == NULL)
				break;
			bool bRes = scTech->validate();
			if (!bRes)
			{
				LOG_ERROR(Effect, "Error>> couldn't validate the scene technique %d\n", t);
				success = false;
			}
			int np = scTech->getNumPasses();
			for (int32 i = 0; i < np; i++)
			{
				// in this special example, only one 'material' technique is used...
				nvFX::IPass* p = scTech->getPass(i);
				//
				// instanciate what is inside the array
				//
				bool bRes = p->setupOverrides(techs.data(), techs.size());
			}
		}
	}
	//
	// Let's find all the techniques and validates them
	// subsequent passes will get validated and related shaders/programs
	// will be created
	//
	for (int32 t = 0; t<techs.size(); t++)
	{
		bool bRes = techs[t]->validate();
		if (!bRes)
		{
			LOG_ERROR(Effect, "Error>> couldn't validate the material technique %s\n", techs[t]->getName());
			success = false;
		}
	}

	return success;
}

void EffectSystem::destroyGlobals()
{
	/*
	nvFX::IContainer* effect = nullptr;
	for (int32 effIdx = 0; effIdx < m_CurrentNumOfEffects; ++effIdx) {
		effect = m_EffectContainer[effIdx];
		nvFX::ITechnique* tech;
		for (int32 mt = 0; tech = effect->findTechnique(mt); mt++)
		{
			nvFX::IContainer* dedicated = nullptr;
			for (int32 dedIdx = 0; dedIdx < m_CurrentNumOfEffects; ++dedIdx) {
				dedicated = m_EffectContainer[dedIdx];
				nvFX::ITechnique* dedTech;
				for (int32 t = 0; ; t++)
				{
					dedTech = dedicated->findTechnique(t);
					if (dedTech == NULL)
						break;
					int np = dedTech->getNumPasses();
					for (int32 i = 0; i < np; i++)
					{
						nvFX::IPass* p = dedTech->getPass(i);
						// this will remove the pass instances in 'tech' that were dedicated to the pass p
						bool bRes = p->releaseOverrides(&tech, 1);
					}
				}
			}
		}
		effect->destroy();
	}
	m_CurrentNumOfEffects = 0;
	*/
}

void EffectSystem::reset()
{
	RenderBackend* backend = m_MainRef->getRenderSystemPtr()->getBackend(); //Get the render backend for destroying backend-objects
		
	for (nvFX::IContainer* effect : m_MaterialEffectContainer) {
		if (effect) {
			nvFX::IContainer::destroy(effect);
		}
	}
	for (nvFX::IContainer* effect : m_SceneEffectContainer) {
		if (effect) {
			nvFX::IContainer::destroy(effect);
		}
	}

	if (m_ViewProjectionMatrices) {
		nvFX::getCstBufferRepositorySingleton()->getExInterface()->releaseCstBuffer(m_ViewProjectionMatrices);
		//delete m_ViewProjectionMatrices;
		m_ViewProjectionMatrices = nullptr;
	}
	if (m_ModelMatrices) {
		nvFX::getCstBufferRepositorySingleton()->getExInterface()->releaseCstBuffer(m_ModelMatrices);
		//delete m_ModelMatrices;
		m_ModelMatrices = nullptr;
	}
	if (m_ViewProjMatHandle != InvalidConstantBufferHandle) {
		backend->destroyConstantBufferFX(m_ViewProjMatHandle);
		m_ViewProjMatHandle = InvalidConstantBufferHandle;
	}
	if (m_ModelMatHandle != InvalidConstantBufferHandle) {
		backend->destroyConstantBufferFX(m_ModelMatHandle);
		m_ModelMatHandle = InvalidConstantBufferHandle;
	}

	m_CurrentNumOfMaterialEffects = 0;
	m_CurrentNumOfSceneEffects = 0;
	m_HeaderCode.clear();
	m_MaterialFXSourceCode.clear();
	m_SceneFXSourceCode.clear();
}

void EffectSystem::nvFXErrorCallback(const ansichar * error) {
	LOG_ERROR(Effect, "Error in nvFX effet system: %s", error);
}

#ifndef EFFECT_INFO_OUTPUT
#	define EFFECT_INFO_OUTPUT 1
#endif
void EffectSystem::nvFXMessageCallback(const ansichar * message)
{
#	if EFFECT_INFO_OUTPUT
	LOG_INFO(Effect, "Message from nvFX effect system: %s", message);
#	endif
}

void EffectSystem::nvFXIncludeCallback(const ansichar * includeName, FILE *& fp, const ansichar *& buf)
{
	fp = tmpfile();
	buf = nullptr;

	SourceMap_t::iterator it = g_EffectSystemInstance->m_HeaderCode.find(includeName);
	if (it != g_EffectSystemInstance->m_HeaderCode.end()) {
		buf = it->second.c_str(); //This is the code to include
		return;
	}
	
	LOG_ERROR(Effect, "Include %s not found.", includeName);
}


ENDNAMESPACE

