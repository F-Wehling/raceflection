#pragma once

#include <string>
#include <LinearMath/btVector3.h>

BEGINNAMESPACE

class PhysicsSettings {
public:
    PhysicsSettings();
    ~PhysicsSettings();

    btVector3 mGravity;
    int mMaximumTicksPerStep;
    double mTargetTickLength;

    double mHingeSoftness;
    double mHingeBias;
    double mHingeRelaxation;
};

ENDNAMESPACE
