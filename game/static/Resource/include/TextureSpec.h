#pragma once

#include <Utilities/UUID.h>

BEGINNAMESPACE

struct TextureSpec {

	UUID uuid;

    uint32 m_Width, m_Height, m_Depth;
    uint32 m_DataSize;
    Byte* m_TextureData;

	static TextureSpec* FromBuffer(const Byte* buffer);
	static uint32 MemSize(const TextureSpec* spec);
	static void ToBuffer(const TextureSpec* spec, Byte* buffer);
};

ENDNAMESPACE
