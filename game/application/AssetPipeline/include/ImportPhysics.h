#pragma once

#include "PhysicsSpec.h"
#include "Filesystem.h"

BEGINNAMESPACE

namespace Importer {
    typedef std::vector<PhysicsSpec*> Physics;
    Physics physicsAllFromFile(const filesys::path& file);

}

ENDNAMESPACE
