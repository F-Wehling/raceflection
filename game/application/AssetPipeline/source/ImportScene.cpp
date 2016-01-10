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

	bool sceneLoad(const filesys::path & path, Package& package, uint32 timeStamp, bool update /* = false */)
	{
        const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);
		if (!scene) return false;

		Lights sceneLights = lightsAllFromScene(scene);
		for (LightSpec* light : sceneLights) {
			package.addLightFromScene(light, path, LightSpec::MemSize(light), 0, timeStamp);
		}

		Materials sceneMaterials = materialAllFromScene(scene);
		for (MaterialSpec* mat : sceneMaterials) {
			package.addMaterialFromScene(mat, path, MaterialSpec::MemSize(mat), 0, timeStamp);
		}

		Meshes sceneMeshes = meshAllFromScene(scene);		
		for (MeshSpec* mesh : sceneMeshes) {
			GeometrySpec* geometry = geometryByUUID(mesh->geometry);
			package.addGeometryFromScene( geometry, path, GeometrySpec::MemSize(geometry), 0, timeStamp);
			package.addMeshFromScene(mesh, path, MeshSpec::MemSize(mesh), 0, timeStamp);
		}

		return true;
	}

}

ENDNAMESPACE
