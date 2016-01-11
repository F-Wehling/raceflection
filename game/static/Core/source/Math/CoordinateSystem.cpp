#include "Math/CoordinateSystem.h"

BEGINNAMESPACE

const float32 CoordinateSystem::POS_X[3] = { 1.0, 0.0, 0.0 };
const float32 CoordinateSystem::POS_Y[3] = { 0.0, 1.0, 0.0 };
const float32 CoordinateSystem::POS_Z[3] = { 0.0, 0.0, 1.0 };
const float32 CoordinateSystem::NEG_X[3] = { -1.0, 0.0, 0.0 };
const float32 CoordinateSystem::NEG_Y[3] = { 0.0, -1.0, 0.0 };
const float32 CoordinateSystem::NEG_Z[3] = { 0.0, 0.0, -1.0 };

const float32* CoordinateSystem::FORWARD = NEG_Z;
const float32* CoordinateSystem::UP = NEG_Y;
const float32* CoordinateSystem::RIGHT = POS_X;

ENDNAMESPACE
