#pragma once

#include <Resource/Texture.h>

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE
namespace Importer {

	typedef std::vector<Resource::Texture*> Textures;

	Textures textureAllFromFile(const filesys::path& file);

}



ENDNAMESPACE