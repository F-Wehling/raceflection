#include "RenderSystem/Scene.h"

#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxSceneNodes("maxSceneNodes", "Defines the maximum number of scene nodes", 40000);

Scene::Scene() :
	m_SceneNodes(cfgMaxSceneNodes),
	m_NumberOfSceneNodes(0)
{
}

SceneNode * Scene::addSceneNode()
{
	return &m_SceneNodes[m_NumberOfSceneNodes++];
}

void Scene::removeSceneNode(SceneNode * sceneNode)
{
	std::swap(*sceneNode, m_SceneNodes[--m_NumberOfSceneNodes]);
	std::memset(&m_SceneNodes[m_NumberOfSceneNodes], 0, sizeof(SceneNode));
}


ENDNAMESPACE
