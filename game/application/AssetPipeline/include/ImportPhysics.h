#pragma once

#include "PhysicsSpec.h"
#include "Filesystem.h"

BEGINNAMESPACE

namespace Importer {
    typedef std::vector<PhysicsSpec*> Physics;
    Physics physicsFromFile(const filesys::path& file);
    PhysicsSpec* physicsByFileName(const std::string& filename);

}

ENDNAMESPACE
