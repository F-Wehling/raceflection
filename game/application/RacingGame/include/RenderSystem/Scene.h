#pragma once

#include <MemorySystem.h>
#include "RenderSystem/RenderHandles.h"

#include "Container/Array.h"

#include "GeometrySpec.h"

BEGINNAMESPACE

class GameObject;

struct Mesh {
	GeometryHandle m_Geometry;
	MaterialHandle m_Materials[GeometrySpec::MaxSubMeshes];
	struct {
		uint32 startIndex;
		uint32 indexCount;
	} m_Submesh[ GeometrySpec::MaxSubMeshes];
	uint32 m_NumSubMeshes;
};

struct SceneNode {	
	GameObject* m_GameObject;
	Mesh m_Mesh;
	bool m_Disabled;
};

class Scene {
	typedef DynArray<SceneNode> SceneNodeContainer_t;
public:
	Scene();

	SceneNode* addSceneNode();
	void removeSceneNode(SceneNode* sceneNode);

	const SceneNode* getSceneNodes() const { return m_SceneNodes.data(); }
	uint32 getSceneNodeCount() const { return m_NumberOfSceneNodes; }
	
private:
	
	SceneNodeContainer_t m_SceneNodes;
	uint32 m_NumberOfSceneNodes;

};

ENDNAMESPACE