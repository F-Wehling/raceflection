#include "ImportScene.h"
#include "Package.h"

#include "ImportAnimation.h"
#include "ImportAudio.h"
#include "ImportGeometry.h"
#include "ImportLight.h"
#include "ImportMaterial.h"
#include "ImportMesh.h"
#include "ImportTexture.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>

#include <Utilities/Hash.h>


#include <AnimationSpec.h>
#include <AudioSpec.h>
#include <GeometrySpec.h>
#include <LightSpec.h>
#include <MaterialSpec.h>
#include <MeshSpec.h>
#include <TextureSpec.h>

BEGINNAMESPACE

Assimp::Importer importer;

namespace Importer {

	bool sceneLoad(const path & path, Package& package, uint32 timeStamp)
	{
		const aiScene* scene = importer.ReadFile(path.string(), aiProcessPreset_TargetRealtime_Quality);
		if (!scene) return false;

		Materials sceneMaterials = materialAllFromScene(scene);

		for (MaterialSpec* mat : sceneMaterials) {
			package.addMaterialFromScene(mat, path, MaterialSpec::MemSize(mat), 0, 0);
		}

		Meshes sceneMeshes = meshAllFromScene(scene);
		
		for (MeshSpec* mesh : sceneMeshes) {
			GeometrySpec* geometry = geometryByUUID(mesh->geometry);
			package.addGeometryFromScene( geometry, path, GeometrySpec::MemSize(geometry), 0, 0);
			package.addMeshFromScene(mesh, path, MeshSpec::MemSize(mesh), 0, 0);
		}

		return true;
	}

}

ENDNAMESPACE
