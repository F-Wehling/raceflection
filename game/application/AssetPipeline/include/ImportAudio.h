#pragma once

#include "AudioSpec.h"

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<AudioSpec*> Audios;

	Audios audioAllFromFile(const filesys::path& path);

}

ENDNAMESPACE