#pragma once

#include <Resource/Material.h>
#include <assimp/scene.h>
#include <assimp/material.h>

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<Resource::Material*> Materials;

	Resource::Material* materialByIndex(uint32 index);

	Materials materialAllFromScene(const aiScene* scene);

	Materials materialAllFromFile(const filesys::path& file);
	
}
ENDNAMESPACE