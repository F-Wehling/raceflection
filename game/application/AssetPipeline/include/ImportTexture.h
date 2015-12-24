#pragma once

#include "TextureSpec.h"

#include <vector>

#include "Filesystem.h"

BEGINNAMESPACE
namespace Importer {

	typedef std::vector<TextureSpec*> Textures;

	Textures textureAllFromFile(const filesys::path& file);

}



ENDNAMESPACE