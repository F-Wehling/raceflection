#include <PhysicsSystem/PhysicsSettings.h>
#include "Configuration/Configuration.h"
#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingFloat32 cfgGravity("Gravity", "", 9.8);
ConfigSettingUint32 cfgMaximumTicksPerStep("MaximumTicksPerStep", "", 20);
ConfigSettingUint32 cfgTicksPerSecond("TicksPerSecond", "", 120);
ConfigSettingFloat32 cfgHingeSoftness("HingeSoftness", "", 0.9);
ConfigSettingFloat32 cfgHingeBias("HingeBias", "", 0.3);
ConfigSettingFloat32 cfgHingeRelaxation("HingeRelaxation", "", 1.0);

PhysicsSettings::PhysicsSettings(){
    parseConfigFile("resource/physics.cfg", false);

    mGravity = btVector3(0.0, -(float) cfgGravity, 0.0);
    mMaximumTicksPerStep = (uint32_t) cfgMaximumTicksPerStep;
    mTargetTickLength = 1.0 / (float)(uint32_t) cfgTicksPerSecond;
    mHingeSoftness = (float) cfgHingeSoftness;
    mHingeBias = (float) cfgHingeBias;
    mHingeRelaxation = (float) cfgHingeRelaxation;

}

PhysicsSettings::~PhysicsSettings(){
}

ENDNAMESPACE
