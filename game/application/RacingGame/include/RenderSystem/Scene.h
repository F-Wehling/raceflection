#pragma once

#include <MemorySystem.h>
#include "RenderSystem/RenderHandles.h"

#include "Container/Array.h"

#include "GeometrySpec.h"
#include "RenderSystem/ConstantBuffer.h"

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
	typedef DynArray<Light> SceneLightContainer_t;
	typedef DynArray<Material> SceneMaterialContainer_t;
    typedef ProxyNoCheckAllocator<PoolAllocator> PoolAlloc;
public:
	Scene();

	SceneNode* addSceneNode();
	void removeSceneNode(SceneNode* sceneNode);

	Light* addLight();
	const Light* getLightData() const;
	uint32 getLightCount() const;

	Material* addMaterial();
	const Material* getMaterialData() const;
	uint32 getMaterialCount() const;

	const SceneNode* getSceneNodes() const { return m_SceneNodes.data(); }
	uint32 getSceneNodeCount() const { return m_NumberOfSceneNodes; }
	
    Camera *getCamera() {return m_Camera;}
    void setCamera(Camera* cam){ m_Camera = cam;}
private:
    Camera* m_Camera;

	SceneNodeContainer_t m_SceneNodes;
	SceneLightContainer_t m_SceneLights;
	SceneMaterialContainer_t m_SceneMaterials;
	uint32 m_NumberOfSceneNodes;
	uint32 m_NumberOfSceneLights;
	uint32 m_NumberOfSceneMaterials;
};

ENDNAMESPACE
