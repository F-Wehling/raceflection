#pragma once

#include <Utilities/UUID.h>

BEGINNAMESPACE

struct LightSpec {

	UUID uuid;

	static const LightSpec* FromBuffer(const Byte* buffer);
	static void ToBuffer(const LightSpec* res, Byte* buffer);
	static uint32 MemSize(const LightSpec* spec);
};

ENDNAMESPACE