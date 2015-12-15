#include "ObjectSystem/GameObject.h"

#include "Configuration/ConfigSettings.h"

#include <glm/gtx/transform.hpp>

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxNumComponents("MaxNumComponents", "Max number of GameObject Components",16);
extern ConfigSettingUint32 cfgMaxGameObjects;

glm::vec3 GameObject::FORWARD_DIRECTION = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 GameObject::LEFT_DIRECTION = glm::vec3(-1.0, 0.0, 0.0);
glm::vec3 GameObject::UP_DIRECTION = glm::vec3(0.0, 1.0, 0.0);

GameObject::ComponentAllocator_t GameObject::sComponentAllocator("ComponentAllocator");

GameObject::GameObject():
    mID(-1), mPosition(glm::vec3(0.0)), mRotation(glm::quat()), mScaling(glm::vec3(1.0)),
    mComponents(cfgMaxNumComponents)
{
    if(!sComponentAllocator.getStart()) {
        uint32 size = (uint32)cfgMaxNumComponents * (uint32)cfgMaxGameObjects * BYTE(128);
        sComponentAllocator.initialize(size);
    }
}

GameObject::GameObject(glm::vec3 Position, glm::quat Rotation, glm::vec3 Scaling ):
    mID(-1), mPosition(Position), mRotation(Rotation), mScaling(Scaling),
    mComponents(cfgMaxNumComponents)
{
    if(!sComponentAllocator.getStart()) {
        uint32 size = (uint32)cfgMaxNumComponents * (uint32)cfgMaxGameObjects * BYTE(128);
        sComponentAllocator.initialize(size);
    }
}

GameObject::~GameObject(){

}

void GameObject::lookAt(glm::vec3 whereToLookAt, glm::vec3 upVector){
    glm::mat4 m = glm::lookAt(mPosition, whereToLookAt, upVector);
    mRotation = glm::quat_cast(m);
}

void GameObject::lookAt(GameObject *go, glm::vec3 upVector){
    lookAt(go->getPosition(), upVector);
}


ENDNAMESPACE
