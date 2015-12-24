#pragma once

#include "AudioSpec.h"

#include <vector>

#include "Filesystem.h"

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<AudioSpec*> Audios;

	Audios audioAllFromFile(const filesys::path& path);

}

ENDNAMESPACE