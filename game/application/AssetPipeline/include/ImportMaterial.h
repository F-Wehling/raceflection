#pragma once

#include "MaterialSpec.h"

#include <assimp/scene.h>
#include <assimp/material.h>

#include <vector>

#include "Filesystem.h"

BEGINNAMESPACE
namespace Importer {

	typedef std::vector<MaterialSpec*> Materials;

	UUID materialByName(const ansichar* name);
	UUID materialByIndex(uint32 index);

	Materials materialAllFromScene(const aiScene* scene);

	Materials materialAllFromFile(const filesys::path& file);
	
}
ENDNAMESPACE