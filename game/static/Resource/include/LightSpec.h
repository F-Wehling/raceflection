#pragma once

#include <Utilities/UUID.h>
#include <Utilities/Flags.h>

BEGINNAMESPACE

struct LightSpec {

	UUID uuid;

	uint32 type;

	typedef float32 Vec3[3];
	Vec3 position;
	Vec3 direction;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 ambient;

	float32 attenuationConstant;
	float32 attenuationLinear;
	float32 attenuationQuadratic;
	float32 angleInnerCone;
	float32 angleOuterCone;

	static const LightSpec* FromBuffer(const Byte* buffer);
	static void ToBuffer(const LightSpec* res, Byte* buffer);
	static uint32 MemSize(const LightSpec* spec);
};

ENDNAMESPACE