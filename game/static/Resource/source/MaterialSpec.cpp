#include "MaterialSpec.h"

BEGINNAMESPACE

MaterialSpec* MaterialSpec::FromBuffer(const Byte* buffer) {
	union {
		const Byte* _buffer;
		MaterialSpec* material;
	};
	_buffer = buffer;
	material->__textureRefLocation = UIntOfPtrSize(buffer) + sizeof(MaterialSpec);
	return material;
}

uint32 MaterialSpec::MemSize(const MaterialSpec * spec)
{
	uint32 numMaps = 0;
	for (int32 i = 0; i < 11; ++i) numMaps += spec->numberOfMaps[i];
	return sizeof(MaterialSpec) + numMaps * sizeof(UUID);
}

void MaterialSpec::ToBuffer(const MaterialSpec * res, Byte * buffer)
{
	uint32 numMaps = 0;
	for (int32 i = 0; i < 11; ++i) numMaps += res->numberOfMaps[i];
	std::memcpy(buffer, (const void*)res, sizeof(MaterialSpec));
	std::memcpy(buffer + sizeof(MaterialSpec), res->textureRefs, numMaps * sizeof(UUID));
}

ENDNAMESPACE