#pragma once

#include <Package/Package.h>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>

#include <Resource/Mesh.h>

#include <experimental/filesystem>
namespace filesys = std::experimental::filesystem;

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<Resource::Mesh*> Meshes;
	Meshes meshAllFromScene(const aiScene* scene);
	Resource::Mesh* meshFromNode(const aiNode* node, const aiScene* scene, Meshes& outMeshes, const aiMatrix4x4& transformation);
	uint32 meshFromAiPrimitive(uint32 type);


	Meshes meshAllFromFile(const filesys::path& file);

}
/*
Mesh* mesh_process(const aiMesh* mesh, const aiMatrix4x4& transformation, Package& package); //get mesh from within a scene 

void mesh_write(FILE* file, Mesh* mesh);
void mesh_setOffsets(Mesh* mesh);
uint32 mesh_getSize(const Mesh* mesh);
*/

ENDNAMESPACE