#pragma once

#include "GeometrySpec.h"
#include "MaterialSpec.h"
#include <Utilities/UUID.h>

BEGINNAMESPACE

struct MeshSpec {
	static const uint32 MaxSubMeshes = GeometrySpec::MaxSubMeshes;

	UUID uuid;

	UUID geometry;
	UUID material[MaxSubMeshes];

	uint32 numSubMeshes;

	struct {
		uint32 startIndex;
		uint32 indexCount;
	} subMeshes[MaxSubMeshes];

	//GeometrySpec* meshGeometry;
	//MaterialSpec* meshMaterial[MaxSubMeshes];

	static const MeshSpec* FromBuffer(const Byte* buffer);
	static uint32 MemSize(const MeshSpec* spec);
	static void ToBuffer(const MeshSpec* res, Byte* buffer);
};

ENDNAMESPACE