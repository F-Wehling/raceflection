#include "RenderSystem/Scene.h"

#include "Configuration/ConfigSettings.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxSceneNodes("scene.maxSceneNodes", "Defines the maximum number of scene nodes", 40000);

Scene::Scene() :
	m_SceneNodes(cfgMaxSceneNodes),
	m_SceneLights(Light::MaxLights),
	m_SceneMaterials(Material::MaxMaterials),
	m_NumberOfSceneNodes(0),
	m_NumberOfSceneLights(0),
	m_NumberOfSceneMaterials(0)
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

Material* Scene::addMaterial() {
	return &m_SceneMaterials[m_NumberOfSceneMaterials++];
}

const Material* Scene::getMaterialData() const {
	return m_SceneMaterials.data();
}

uint32 Scene::getMaterialCount() const {
	return m_NumberOfSceneMaterials;
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
