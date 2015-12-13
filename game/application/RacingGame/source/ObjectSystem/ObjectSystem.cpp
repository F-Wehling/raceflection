#include "ObjectSystem/ObjectSystem.h"

#include "PackageSpec.h"

BEGINNAMESPACE

ObjectSystem::ObjectSystem():mGameObjects("GameObjectManager")
{
    mGameObjects.initialize(sizeof(GameObject)*MAX_OBJECTS, sizeof(GameObject), alignof(GameObject));
}

ObjectSystem::~ObjectSystem()
{
    //delete mGameObjects;
}

GameObject* ObjectSystem::getObjectByID(GameObjectID ID)
{
	return getNthElement<GameObject>(ID, mGameObjects);
}

void ObjectSystem::deleteObject(GameObjectID ID){

    GameObject* object = getObjectByID(ID);
    eng_delete(object,mGameObjects);
}

bool ObjectSystem::isTriggerArea(GameObjectID ID)
{
    // TODO: Some objects may trigger an event, detect if object is a trigger area
    return false;
}

bool ObjectSystem::createObjectsFromPackageSpec(PackageSpec * pkgSpec)
{
	//if we have something in the package to create game objects... use it here
	return true;
}

ENDNAMESPACE
