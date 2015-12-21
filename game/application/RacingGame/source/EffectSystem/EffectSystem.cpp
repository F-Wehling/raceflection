#include "EffectSystem/EffectSystem.h"

#include "FxLib.h"
#include "FxLibEx.h"
#include "FxParser.h"

#include "Logging.h"

BEGINNAMESPACE

EffectSystem::EffectSystem() {
	
}

bool EffectSystem::initialize() {
	
	nvFX::setErrorCallback(&EffectSystem::nvFXErrorCallback);
	nvFX::setMessageCallback(&EffectSystem::nvFXMessageCallback);
	nvFX::setIncludeCallback(&EffectSystem::nvFXIncludeCallback);

	return true;
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
	LOG_ERROR(Effect, "Include callback not implemented yet (%s)", includeName);
}


ENDNAMESPACE

