#include "TextureSpec.h"

BEGINNAMESPACE

const TextureSpec* TextureSpec::FromBuffer(const Byte* _buffer) {
	union {
		const Byte* buffer;
		TextureSpec* textureSpec;
		UIntOfPtrSize location;
	};
	buffer = _buffer;
	textureSpec->__TextureDataLocation = location + sizeof(TextureSpec);
	return textureSpec;
}

uint32 TextureSpec::MemSize(const TextureSpec * spec)
{
	return sizeof(TextureSpec) + spec->m_DataSize;
}

void TextureSpec::ToBuffer(const TextureSpec * spec, Byte * buffer)
{
	*(TextureSpec*)buffer = *spec;
	std::memcpy(buffer + sizeof(TextureSpec), spec->m_TextureData, spec->m_DataSize);
}

ENDNAMESPACE