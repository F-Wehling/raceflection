#pragma once

#include "AnimationSpec.h"

#include <assimp/scene.h>
#include <assimp/anim.h>

#include <vector>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<AnimationSpec*> Animations;
	
	Animations animationAllFromScene(const aiScene* scene);
	Animations animationAllFromFile(const filesys::path& file);
}

ENDNAMESPACE