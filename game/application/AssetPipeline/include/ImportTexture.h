#pragma once

#include "TextureSpec.h"

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE
namespace Importer {

	typedef std::vector<TextureSpec*> Textures;

	Textures textureAllFromFile(const filesys::path& file);

}



ENDNAMESPACE