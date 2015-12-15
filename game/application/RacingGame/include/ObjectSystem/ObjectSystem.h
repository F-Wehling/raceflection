#pragma once

#include <vector>
#include "ObjectSystem/GameObject.h"
#include "MemorySystem.h"

#include "Container/Array.h"

BEGINNAMESPACE

using GameObjectID = int;

class PackageSpec;
class ObjectSystem {

    typedef ProxyAllocator < PoolAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> GameObjectAlloc;
    //
    /// to be able to iterate over all game objects i switched back to a vector of game objects
    /// to be able to use our Pool-Allocator as the allocator for the std::vector container we would have
    /// to create a new allocator class... but for now this will work too, because we simply preallocate
    /// enough objects
    //
    typedef DynArray<GameObject> GameObjects;
private:

    //std::vector<GameObject> mGameObjects;
    GameObjects mGameObjects;
    uint32 mNumGameObjects;

public:
    ObjectSystem();
    ~ObjectSystem();

    GameObject* createObject(){
            GameObject* object = &mGameObjects[mNumGameObjects++];
            return object;
        /*
            GameObject* object = eng_new(GameObject, mGameObjects)(arguments...);
            object->mID = getElementIndex(object, mGameObjects);
            return object;
        */
    }

    void deleteObject(GameObjectID ID);

    GameObject* getObjectByID(GameObjectID ID);

    bool isTriggerArea(GameObjectID ID);

	bool createObjectsFromPackageSpec(PackageSpec* pkgSpec);

    inline uint32 getNumObjects() const { return mNumGameObjects; }
    inline GameObject* getFirstObject() { return mGameObjects.data(); }

    bool tick(float32 dt);

private:
    static void tickGameObjects(GameObject * gameObjects, uint32 numObjects, void * extraInfo);
    void tickGameObject(GameObject * go, float32 dt);
};

ENDNAMESPACE
