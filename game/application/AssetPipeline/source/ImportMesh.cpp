#include <ImportMesh.h>
#include <ImportGeometry.h>
#include <ImportMaterial.h>

#include <functional>
#include <numeric>

#include <Package.h>
#include <Utilities/Assert.h>

#include <Utilities/Number.h>

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/color4.h>
#include <assimp/Importer.hpp>

BEGINNAMESPACE

#define MAX_JOINTS 4
#define MAX_PER_16BIT 65535

namespace Importer {
	
#if defined _DEBUG
#	define CHECK( result, cond, message, ... ) \
	ASSERT(cond, message, __VA_ARGS__); \
	result = result && cond
#	else
#	define CHECK( result, cond, message ) result = result && cond
#endif
    void meshFromNode(const aiNode* node, const aiScene * scene, Meshes& outMeshes, const aiMatrix4x4& transformation, bool globalCoords){
        aiMatrix4x4 matrix = node->mTransformation * transformation;
		if (node->mNumMeshes > 0) {
			//geometryFromNode(node, scene, outGeo, node->mTransformation);
			
			std::vector<aiMesh*> subMeshes(node->mNumMeshes);
			for (uint32 i = 0; i < node->mNumMeshes; ++i) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				if (i > 0 && subMeshes[0]->mPrimitiveTypes != mesh->mPrimitiveTypes) {
					LOG_ERROR(General, "Skip mesh because it is not a correct-primitive-type. (node: %s, mesh: %s)", node->mName.C_Str(), mesh->mName.C_Str());
					continue;
				}
				subMeshes[i] = mesh;
			}

			matrix = aiMatrix4x4(
				1.0, 0.0, 0.0, 0.0,
                0.0, 0.0, -1.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 1.0
				) * matrix;

			GeometrySpec* outGeo = geometryFromMeshVec(subMeshes, matrix, globalCoords);
			uint32 nsubMeshes = outGeo->numberOfSubmeshes; //each submesh should have a material assigned

			MeshSpec* mesh = new MeshSpec;
			std::memset(mesh, 0, sizeof(MeshSpec));
		
			mesh->uuid = generateUUID();
			mesh->geometry = outGeo->uuid;
			mesh->numSubMeshes = nsubMeshes;

			for (uint32 meshIdx = 0; meshIdx < nsubMeshes; ++meshIdx) {
				aiString materialName;
				scene->mMaterials[subMeshes[meshIdx]->mMaterialIndex]->Get(AI_MATKEY_NAME, materialName);
				mesh->material[meshIdx] = materialByName(materialName.C_Str());

				mesh->subMeshes[meshIdx].startIndex = outGeo->indices[meshIdx].startIndex;
				mesh->subMeshes[meshIdx].indexCount = outGeo->indices[meshIdx].indexCount;
			}
			outMeshes.push_back(mesh);
		}
		for (uint32 i = 0; i < node->mNumChildren; ++i) {
            meshFromNode(node->mChildren[i], scene, outMeshes, matrix, globalCoords);
		}
		
	}

	Meshes meshAllFromScene(const aiScene * scene)
	{

		Meshes out; //combine them
		meshFromNode(scene->mRootNode, scene, out);

		return out;
	}

	Meshes meshAllFromFile(const filesys::path & file)
	{
		return Meshes();
	}
}

ENDNAMESPACE

