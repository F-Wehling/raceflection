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
    extern bool glewExperimental;
	extern unsigned int glewInit(void);
	extern const unsigned char * glewGetErrorString(unsigned int error);
}
//END FORWARD DECLARE GLEW 

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxMaterialEffectContainer("MaxEffectContainer", "Set the maximum number of material-effect container.", 50);
ConfigSettingUint32 cfgMaxSceneEffectContainer("MaxEffectContainer", "Set the maximum number of material-effect container.", 20);

ConfigSettingAnsichar cfgViewProjectionMatrixBlockName("effect.viewProjectionMatricesBlockName", "Set the block-name for the ViewProjection-matrices.", "ViewProjectionMatrices");
ConfigSettingAnsichar cfgModelMatrixBlockName("effect.modelMatricesBlockName", "Set the block-name for the Model-Matrices", "ModelMatrices");
ConfigSettingAnsichar cfgLightBlockName("effect.lightBlockName", "Set the block-name for the Model-Matrices", "Lights");
ConfigSettingAnsichar cfgMaterialBlockName("effect.materialBlockName", "Set the block-name for the Model-Matrices", "Materials");

extern ConfigSettingUint32 cfgWindowWidth;
extern ConfigSettingUint32 cfgWindowHeight;

extern ConfigSettingAnsichar cfgPostProcessingPipeline;

EffectHandle InvalidEffectHandle = { EffectHandle::_Handle_type(-1), EffectHandle::_Handle_type(-1) };

EffectSystem* g_EffectSystemInstance;
EffectSystem::EffectSystem(Main* mainRef) : 
	m_Dirty(true),
	m_CurrentNumOfMaterialEffects(0),
	m_CurrentNumOfSceneEffects(0),
	m_MainRef(mainRef)/*,
	m_ViewProjectionMatrices(nullptr),
	m_ViewProjMatHandle(InvalidConstantBufferHandle),
	m_ModelMatrices(nullptr),
	m_ModelMatHandle(InvalidConstantBufferHandle)
	*/
{
	g_EffectSystemInstance = nullptr;
}

bool EffectSystem::initialize(uint32 api) {

	switch (api) {
	case RenderEngineType::OpenGL:
#   if OS_LINUX
       glewExperimental = 1;
#   endif
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
		}
	}

	nvFX::getResourceRepositorySingleton()->setParams(0, 0, cfgWindowWidth, cfgWindowHeight, 1, 0, 0);
	if (!nvFX::getResourceRepositorySingleton()->updateValidated()) {
		LOG_ERROR(Effect, "Invalid resource specification");
		return;
	}

	//finally we have to sort the scene effects to match the post processing pipeline
	//this should be done by the assert pipeline ... but for now its okay
	String pipeline(cfgPostProcessingPipeline);
	pipeline += ";BlitToScreen"; //the last effect should be BlitToScreen;
	std::sort(m_SceneEffectContainer.begin(), m_SceneEffectContainer.begin() + m_CurrentNumOfSceneEffects, [&](nvFX::IContainer* l, nvFX::IContainer* r)->bool{
		String name_l = String(l->getName());
		String name_r = String(r->getName());
		if (name_l.find("postProcess") == String::npos) return true;
		if (name_r.find("postProcess") == String::npos) return false;
		name_l = name_l.substr(11);
		name_r = name_r.substr(11);
		size_type pos_l = pipeline.find(name_l);
		size_type pos_r = pipeline.find(name_r);
		return pos_l <= pos_r;
	});

}

EffectHandle EffectSystem::getFirstSceneEffect() const
{
	if (m_SceneEffectContainer.empty()) return InvalidEffectHandle;
	EffectHandle hdl = { 0, 1 };
	return hdl;
}

EffectHandle EffectSystem::getNextSceneEffect(EffectHandle hdl) const
{
	if (m_CurrentNumOfSceneEffects <= hdl.index + 1) return InvalidEffectHandle;
	++hdl.index;
	return hdl;
}

EffectHandle EffectSystem::getSceneEffectByName(const ansichar * name)
{
	EffectHandle hdl = getEffectByName(name, m_SceneEffectContainer, m_CurrentNumOfSceneEffects);
	hdl.generation = 1;
	return hdl;
}

EffectHandle EffectSystem::getMaterialEffectByName(const ansichar * name)
{
	EffectHandle hdl = getEffectByName(name, m_MaterialEffectContainer, m_CurrentNumOfMaterialEffects);
	hdl.generation = 2;
	return hdl;
}

bool EffectSystem::renderSceneEffect(EffectHandle handle, EffectRenderDelegate & fn, uint32 techniqueIdx /* = 0*/)
{
	if (handle == InvalidEffectHandle) return false;
    return renderEffect(handle, fn, techniqueIdx, m_SceneEffectContainer);
}

ConstantBufferHandle EffectSystem::getViewProjectionBufferHandle() const
{
	ConstantBufferHandle hdl = InvalidConstantBufferHandle;
	CstBufferMap_t::const_iterator it = m_ConstantBuffers.find((const ansichar*)cfgViewProjectionMatrixBlockName);
	if (it != m_ConstantBuffers.end()) return it->second.handle;
	return hdl;
}

ConstantBufferHandle EffectSystem::getModelBufferHandle() const
{
	ConstantBufferHandle hdl = InvalidConstantBufferHandle;
	CstBufferMap_t::const_iterator it = m_ConstantBuffers.find((const ansichar*)cfgModelMatrixBlockName);
	if (it != m_ConstantBuffers.end()) return it->second.handle;
	return hdl;
}

ConstantBufferHandle EffectSystem::getLightBufferHandle() const
{
	ConstantBufferHandle hdl = InvalidConstantBufferHandle;
	CstBufferMap_t::const_iterator it = m_ConstantBuffers.find((const ansichar*)cfgLightBlockName);
	if (it != m_ConstantBuffers.end()) return it->second.handle;
	return hdl;
}

ConstantBufferHandle EffectSystem::getMaterialBufferHandle() const
{
	ConstantBufferHandle hdl = InvalidConstantBufferHandle;
	CstBufferMap_t::const_iterator it = m_ConstantBuffers.find((const ansichar*)cfgMaterialBlockName);
	if (it != m_ConstantBuffers.end()) return it->second.handle;
	return hdl;
}

void EffectSystem::uploadUniform(const ansichar * name, TextureHandle hdl)
{
	nvFX::IUniform* uniform = getUniformByName(name);
	if (!uniform) return;

	nvFX::IResource* resource = nvFX::getResourceRepositorySingleton()->find(hdl.index);
	if (resource != nullptr) {
		uniform->setTextureResource(resource);
	}
}

void EffectSystem::uploadUniform(const ansichar * name, int32 iValue)
{
	nvFX::IUniform* uniform = getUniformByName(name);
	if (!uniform) return;
	uniform->setValue1i(iValue);
}

nvFX::IUniform * EffectSystem::getUniformByName(const ansichar * name)
{
	UniformMap_t::iterator it = m_Uniforms.find(name);
	if (it != m_Uniforms.end()) return it->second.uniform;
	return nullptr;
}

void EffectSystem::updateUniforms()
{
	if (m_CurrentPass == nullptr) return;
	for (auto& u : m_Uniforms) {
		nvFX::IUniformEx *uniform = u.second.uniform->getExInterface();
		uniform->update(m_CurrentPass);
	}
}

void EffectSystem::cleanUp()
{
	nvFX::getResourceRepositorySingleton()->setParams(0, 0, cfgWindowWidth, cfgWindowHeight, 1, 0, 0);
	if (!nvFX::getResourceRepositorySingleton()->validateAll()) {
		LOG_ERROR(Effect, "Invalid resource specification");
		return;
	}
	m_Dirty = false;
}

bool EffectSystem::renderEffect(EffectHandle handle, EffectRenderDelegate & fn, uint32 techniqueIdx, EffectContainers_t& containerMgr)
{
	if (m_Dirty) {
		LOG_ERROR(Effect, "The effect system is dirty. Handles may be invalid.");
		return false;
	}
	nvFX::IContainer* effect = containerMgr[handle.index];
    nvFX::ITechnique* tech = effect->findTechnique(techniqueIdx); //TODO: different techniques than 0
	
	nvFX::PassInfo pr;
	int32 np = tech->getNumPasses();
	bool successfull = true;
	for (int32 i = 0; i<np && successfull; i++)
	{
		m_CurrentPass = tech->getPass(i);
		updateUniforms();
		m_CurrentPass->execute(&pr);
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

	createGlobalsFromEffectContainer(container);

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

	return container;
}

uint32 getBufferSize(nvFX::ICstBuffer* buffer) {
	//this is unelegant.. but for now okay (fix later)
	if (strcmp(buffer->getName(), cfgViewProjectionMatrixBlockName) == 0) {
		return sizeof(ViewProjectionMatrices);
	}
	else if (strcmp(buffer->getName(), cfgModelMatrixBlockName) == 0) {
		return sizeof(ModelMatrices);
	}
	else if (strcmp(buffer->getName(), cfgLightBlockName) == 0) {
		return sizeof(Light) * Light::MaxLights + sizeof(uint32) * 4;
	}
	else if (strcmp(buffer->getName(), cfgMaterialBlockName) == 0) {
		return sizeof(Material) * Material::MaxMaterials + sizeof(uint32) * 4;
	}
	return 0;
}

void EffectSystem::createGlobalsFromEffectContainer(nvFX::IContainer * effectContainer)
{
	RenderBackend* backend = m_MainRef->getRenderSystemPtr()->getBackend(); //Get the render backend for creating backend-objects
	int32 cstBufferIdx = 0;
	nvFX::ICstBuffer* cstBuffer = nullptr;
	while ((cstBuffer = effectContainer->findCstBuffer(cstBufferIdx++))) {

		if (m_ConstantBuffers.find(cstBuffer->getName()) == m_ConstantBuffers.end()) { // already tracked?!
			CstBufRef ref;
			ref.handle = backend->createConstantBuffer(cstBuffer, getBufferSize(cstBuffer));
			if (ref.handle != InvalidConstantBufferHandle) {
				ref.buffer = cstBuffer;
				m_ConstantBuffers[cstBuffer->getName()] = ref;
			}
		}

		LOG_INFO(Effect, "Found a constant buffer: %s", cstBuffer->getName());
	}

	int32 uniformIdx = 0;
	nvFX::IUniform* uniform = nullptr;
	while ((uniform = effectContainer->findUniform(uniformIdx++))) {
		UniformRef ref = { uniform };
		m_Uniforms[uniform->getName()] = ref;

		const ansichar* textureName = uniform->annotations()->getAnnotationString("textureName");
		if (!textureName) continue;


		nvFX::IResource* res = nvFX::getResourceRepositorySingleton()->find(textureName);

		if (res != nullptr) {
			uniform->setSamplerResource(res);
		}
		//LOG_INFO(Effect, "Found a uniform: %s", uniform->getName());
	}


	int32 resourceIdx = 0;
	nvFX::IResource* res = nullptr;
	while ((res = effectContainer->findResource(resourceIdx++))) {
		/*
		nvFX::IResourceEx* resource = res->getExInterface();
		if (resource->getUserCnt() <= 0) continue;
		nvFX::IAnnotation* annotations = resource->annotations();
		const ansichar* resourceName = annotations->getAnnotationString("resourceName");
		if (resourceName == nullptr) continue;
		
		LOG_INFO(Effect, "Resource Name: %s", resourceName);	
		*/
	}

	int32 samplerStateIdx = 0;
	nvFX::ISamplerState* samplerState = nullptr;
	while ((samplerState = effectContainer->findSamplerState(samplerStateIdx++))) {
		//LOG_INFO(Effect, "Found a samplerState: %s", samplerState->getName());
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

	for (auto& cBuf : m_ConstantBuffers) {
		nvFX::getCstBufferRepositorySingleton()->getExInterface()->releaseCstBuffer(cBuf.second.buffer);
		backend->destroyConstantBufferFX(cBuf.second.handle);
	}
	m_ConstantBuffers.clear();

	m_CurrentNumOfMaterialEffects = 0;
	m_CurrentNumOfSceneEffects = 0;
	m_HeaderCode.clear();
	m_MaterialFXSourceCode.clear();
	m_SceneFXSourceCode.clear();
	std::fill(m_MaterialEffectContainer.begin(), m_MaterialEffectContainer.end(), nullptr);
	std::fill(m_SceneEffectContainer.begin(), m_SceneEffectContainer.end(), nullptr);

	m_Dirty = true;
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

