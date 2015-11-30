#pragma once

#include <Resource/Audio.h>

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<Resource::Audio*> Audios;

	Audios audioAllFromFile(const filesys::path& path);

}

ENDNAMESPACE