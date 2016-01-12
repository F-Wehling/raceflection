#pragma once

#include "MeshSpec.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>

#include <vector>

#include "Filesystem.h"

BEGINNAMESPACE

namespace Importer {

	typedef std::vector<MeshSpec*> Meshes;
	Meshes meshAllFromScene(const aiScene* scene);
    void meshFromNode(const aiNode* node, const aiScene* scene, Meshes& outMeshes, const aiMatrix4x4& transformation = aiMatrix4x4(), bool globalCoords = true);

	Meshes meshAllFromFile(const filesys::path& file);

    MeshSpec* meshByName(const std::string& name);

}
/*
Mesh* mesh_process(const aiMesh* mesh, const aiMatrix4x4& transformation, Package& package); //get mesh from within a scene 

void mesh_write(FILE* file, Mesh* mesh);
void mesh_setOffsets(Mesh* mesh);
uint32 mesh_getSize(const Mesh* mesh);
*/

ENDNAMESPACE
