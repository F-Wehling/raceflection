#pragma once

#include <Utilities/UUID.h>

BEGINNAMESPACE

struct TextureSpec {

	UUID uuid;

	static TextureSpec* FromBuffer(const Byte* buffer);
	static uint32 MemSize(const TextureSpec* spec);
	static void ToBuffer(const TextureSpec* spec, Byte* buffer);
};

ENDNAMESPACE