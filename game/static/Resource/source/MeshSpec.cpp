#include "MeshSpec.h"

BEGINNAMESPACE

const MeshSpec* MeshSpec::FromBuffer(const Byte* buffer) {
	return (const MeshSpec*)buffer;
}

uint32 MeshSpec::MemSize(const MeshSpec * spec)
{
	return sizeof(MeshSpec);
}

void MeshSpec::ToBuffer(const MeshSpec * res, Byte * buffer)
{
	*((MeshSpec*)buffer) = *res;
}

ENDNAMESPACE