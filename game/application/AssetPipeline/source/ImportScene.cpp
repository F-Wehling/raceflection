#include <ImportScene.h>
#include <Package/Package.h>

#include <Mesh/ImportMesh.h>
#include <Material/ImportMaterial.h>
#include <Texture/ImportTexture.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>

#include <Utilities/Hash.h>

BEGINNAMESPACE

Assimp::Importer importer;

namespace Importer {

	bool sceneLoad(const path & path, Package& package, uint32 timeStamp)
	{
		const aiScene* scene = importer.ReadFile(path.string(), aiProcessPreset_TargetRealtime_Quality);
		if (!scene) return false;

		Materials sceneMaterials = materialAllFromScene(scene);
		Meshes sceneMeshes = meshAllFromScene(scene);
		
		return true;
	}

}

ENDNAMESPACE