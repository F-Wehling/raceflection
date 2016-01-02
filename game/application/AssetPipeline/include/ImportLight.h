#pragma once

#include <LightSpec.h>

#include <assimp/scene.h>
#include <assimp/light.h>

#include <vector>

#include "Filesystem.h"

BEGINNAMESPACE

DECLARE_FLAGS(LightType, Point, Directional, Spot, Ambient);

namespace Importer {
	typedef std::vector<LightSpec*> Lights;

	Lights lightsAllFromScene(const aiScene* scene);


	Lights lightsAllFromFile(const filesys::path& file);
}
ENDNAMESPACE