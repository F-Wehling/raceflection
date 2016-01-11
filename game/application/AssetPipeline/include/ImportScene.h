#pragma once

#include <Package.h>

#include "Filesystem.h"

BEGINNAMESPACE
namespace Importer {
	bool sceneLoad(const filesys::path& path, Package& package, uint32 timeStamp, bool update = false);
}
ENDNAMESPACE