#pragma once

#include <Utilities/UUID.h>

BEGINNAMESPACE

struct AnimationSpec {
	static const uint32 MaxJoints = 4;

	UUID uuid;

	static const AnimationSpec *FromBuffer(const Byte* buffer);
	static void ToBuffer(const AnimationSpec* res, Byte* buffer);
	static uint32 MemSize(const AnimationSpec* spec);
};

ENDNAMESPACE