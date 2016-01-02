#include "RenderSystem/Scene.h"

#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxSceneNodes("scene.maxSceneNodes", "Defines the maximum number of scene nodes", 40000);
ConfigSettingUint32 cfgMaxSceneLights("scene.maxSceneLights", "Defines the maximum number of scene lights (be careful to not exceed the number of lights defined in the shader)", 100);

Scene::Scene() :
	m_SceneNodes(cfgMaxSceneNodes),
	m_SceneLights(cfgMaxSceneLights),
	m_NumberOfSceneNodes(0),
	m_NumberOfSceneLights(0)
{
}

Light* Scene::addLight() {
	return &m_SceneLights[m_NumberOfSceneLights++];
}

const Light* Scene::getLightData() const {
	return m_SceneLights.data();
}

uint32 Scene::getLightCount() const {
	return m_NumberOfSceneLights;
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
