#pragma once

#include "GameObjectSpec.h"
#include "Filesystem.h"

BEGINNAMESPACE

namespace Importer {
    typedef std::vector<GameObjectSpec*> GameObjects;
    GameObjects gameObjectsAllFromFile(const filesys::path& file);
    GameObjectSpec* gameObjectFromStream(IFileStream& stream);

}

ENDNAMESPACE
