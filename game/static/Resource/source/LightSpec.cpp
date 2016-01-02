#include "LightSpec.h"

BEGINNAMESPACE

const LightSpec* LightSpec::FromBuffer(const Byte* buffer) {
	return (const LightSpec*)buffer;
}

void LightSpec::ToBuffer(const LightSpec * res, Byte * buffer)
{
	std::memcpy(buffer, res, sizeof(LightSpec));
}

uint32 LightSpec::MemSize(const LightSpec * spec)
{
	return sizeof(LightSpec);
}

ENDNAMESPACE