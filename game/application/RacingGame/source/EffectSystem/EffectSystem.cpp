#include "EffectSystem/EffectSystem.h"

#include "PackageSpec.h"
#include "EffectSpec.h"

#include "FxLib.h"
#include "FxLibEx.h"
#include "FxParser.h"

#include "Logging.h"

#include "Container/Array.h"

#include "RenderSystem/RenderTypes.h"

//FORWARD DECLARE GLEW
extern "C" {
	extern unsigned int glewInit(void);
	extern const unsigned char * glewGetErrorString(unsigned int error);
}
//END FORWARD DECLARE GLEW 

BEGINNAMESPACE

EffectHandle InvalidEffectHandle = { EffectHandle::_Handle_type(-1), EffectHandle::_Handle_type(-1) };

typedef std::pair<ansichar[EffectSpec::MaxEffectName], const ansichar*> FXHeaderPair;
typedef DynArray<FXHeaderPair> FXHeaderMap;
FXHeaderMap g_FXHeader;

EffectSystem::EffectSystem() {
	g_FXHeader.reserve(1 << EffectHandle::IndexBitCount);
}

bool EffectSystem::initialize(int32 api) {
	
	switch (api) {
	case RenderEngineType::OpenGL:
		uint32 g = glewInit(); //the opengl part needs to initialize
		if (g != 0) { 
			LOG_ERROR(Effect, "Glew-init failed: %s.", glewGetErrorString(g)); 
			return false;
		}
		break;
	}

	nvFX::setErrorCallback(&EffectSystem::nvFXErrorCallback);
	nvFX::setMessageCallback(&EffectSystem::nvFXMessageCallback);
	nvFX::setIncludeCallback(&EffectSystem::nvFXIncludeCallback);
	
	return true;
}

void EffectSystem::shutdown()
{
}

bool EffectSystem::createEffect(const ansichar* effectSource)
{
	nvFX::IContainer* container = nvFX::IContainer::create();
	if (!nvFX::loadEffect(container, effectSource)) {
		LOG_ERROR(Effect, "Effect-Parsing failed.");
		return false;
	}

	bool anyValid = false;
	for (int32 techIdx = 0; techIdx < container->getNumTechniques(); ++techIdx) {
		nvFX::ITechnique* tech = container->findTechnique(techIdx);
		if (tech && tech->validate()) { //valid technique
			anyValid = true;
		}
	}
	if (!anyValid) return false;
	
	return true;
}

void EffectSystem::createEffectLibraryFromPackageSpec(const PackageSpec * spec)
{
	//search for include files (*fxh)
	for (uint32 i = 0; i < spec->getEffectCount(); ++i) {
		const EffectSpec* effSpec = spec->getEffectSpec(i);
		if (effSpec->effectType == EffectType::FXHeader) {
			FXHeaderPair header;
			std::memcpy(header.first, effSpec->name, EffectSpec::MaxEffectName);
			header.second = effSpec->effectSource;
			g_FXHeader.push_back(header); //append: to be found by the effect compiler
		}
	}

	//compile effects (*fx)
	for (uint32 i = 0; i < spec->getEffectCount(); ++i) {
		const EffectSpec* effSpec = spec->getEffectSpec(i);
		if (effSpec->effectType == EffectType::FX) {
			if (!createEffect(effSpec->effectSource)) {
				LOG_ERROR(Effect, "Error while compiling effect %s", effSpec->name);
				continue;
			}
		}
	}
}

void EffectSystem::nvFXErrorCallback(const ansichar * error) {
	LOG_ERROR(Effect, "Error in nvFX effet system: %s", error);
}

void EffectSystem::nvFXMessageCallback(const ansichar * message)
{
	LOG_INFO(Effect, "Message from nvFX effect system: %s", message);
}

void EffectSystem::nvFXIncludeCallback(const ansichar * includeName, FILE *& fp, const ansichar *& buf)
{
	//include callback 
	fp = nullptr;
	for (auto& header : g_FXHeader) {
		if (strcmp(header.first, includeName) == 0) {
			fp = tmpfile(); // the underlying api requires a valid FILE-handle - create a temporary one
			buf = header.second;
			return;
		}
	}
	LOG_ERROR(Effect, "Include %s not found.", includeName);
}


ENDNAMESPACE

