#pragma once

#include <MemorySystem.h>
#include "RenderSystem/RenderHandles.h"

#include "Container/Array.h"

#include "GeometrySpec.h"

#include "RenderSystem/Mesh.h"

BEGINNAMESPACE

class GameObject;
class Camera;


struct SceneNode {	
	GameObject* m_GameObject;
	Mesh m_Mesh;
	bool m_Disabled;
};

class Scene {
	typedef DynArray<SceneNode> SceneNodeContainer_t;
    typedef ProxyNoCheckAllocator<PoolAllocator> PoolAlloc;
public:
	Scene();

	SceneNode* addSceneNode();
	void removeSceneNode(SceneNode* sceneNode);

	const SceneNode* getSceneNodes() const { return m_SceneNodes.data(); }
	uint32 getSceneNodeCount() const { return m_NumberOfSceneNodes; }
	
    Camera *getCamera() {return m_Camera;}
    void setCamera(Camera* cam){ m_Camera = cam;}
private:
    Camera* m_Camera;

	SceneNodeContainer_t m_SceneNodes;
	uint32 m_NumberOfSceneNodes;

};

ENDNAMESPACE
