#pragma once

#include <Utilities/UUID.h>

BEGINNAMESPACE

struct AudioSpec {

	UUID uuid;

	static const AudioSpec* FromBuffer(const Byte* buffer);
	static uint32 MemSize(const AudioSpec* spec);
	static void ToBuffer(const AudioSpec* res, Byte* buffer);
};

ENDNAMESPACE