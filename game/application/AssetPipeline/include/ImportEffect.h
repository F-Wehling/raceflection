#pragma once

#include "EffectSpec.h"
#include "Filesystem.h"

#include <vector>

BEGINNAMESPACE

namespace Importer {
	typedef std::vector<EffectSpec*> Effects;
	Effects effectAllFromFile(const filesys::path& file);
}

ENDNAMESPACE