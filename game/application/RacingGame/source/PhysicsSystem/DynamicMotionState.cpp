#include <PhysicsSystem/DynamicMotionState.h>

#include <ObjectSystem/ObjectSystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

BEGINNAMESPACE

using Vector3 = glm::vec3;
using Quaternion = glm::quat;

DynamicMotionState::DynamicMotionState(ObjectSystem* objectSystem, const btTransform &initialPosition, GameObjectId gameObjectId) :
    mObjectSystem(objectSystem), mCurrentPosition(initialPosition), mGameObjectId(gameObjectId) {}

void DynamicMotionState::setGameObject(GameObjectId gameObjectId){
    mGameObjectId = gameObjectId;
}

void DynamicMotionState::getWorldTransform(btTransform &worldTrans) const {
    worldTrans = mCurrentPosition;
}

void DynamicMotionState::setWorldTransform(const btTransform &worldTrans){
    if(mGameObjectId == -1.0) return;
    //Gets new values
    btQuaternion rot = worldTrans.getRotation();
    btVector3 pos = worldTrans.getOrigin();
    
    Vector3 resultPosition = Vector3(pos.getX(), pos.getY(), pos.getZ());
    Quaternion resultOrientation = Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
    
    mObjectSystem->getObjectByID(mGameObjectId)->setPosition(resultPosition);
    mObjectSystem->getObjectByID(mGameObjectId)->setRotation(resultOrientation);
}

void DynamicMotionState::setCurrentPos(const btTransform &currentPosition){
    mCurrentPosition = currentPosition;
}

ENDNAMESPACE
