#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "MemorySystem.h"

#include "Container/Array.h"

BEGINNAMESPACE

class ObjectSystem;
class GameObjectComponent;
using GameObjectID = int32;

class GameObject {
friend class ObjectSystem;
public:
    static glm::vec3 FORWARD_DIRECTION;
    static glm::vec3 LEFT_DIRECTION;
    static glm::vec3 UP_DIRECTION;
private:
    typedef DynArray<GameObjectComponent*> Components;

private:

    GameObjectID mID;

    glm::vec3 mPosition;

    glm::quat mRotation;

    glm::vec3 mScaling;

    ObjectSystem* mObjectSystem;

    Components mComponents;
    uint32 mNumComponents;
public:

    GameObject();
    GameObject(glm::vec3 Position, glm::quat Rotation, glm::vec3 Scaling );
    ~GameObject();

    inline GameObjectID getID() const { return mID; }
    inline glm::vec3 getPosition() const { return mPosition; }
    inline glm::quat getRotation() const { return mRotation; }
    inline glm::vec3 getScaling() const { return mScaling; }
    inline glm::vec3 getForward() const { return FORWARD_DIRECTION * mRotation; }
    inline glm::vec3 getBackward() const { return -(FORWARD_DIRECTION * mRotation); }
    inline glm::vec3 getLeft() const { return LEFT_DIRECTION * mRotation; }
    inline glm::vec3 getRight() const { return -(LEFT_DIRECTION * mRotation); }
    inline glm::vec3 getUp() const { return UP_DIRECTION * mRotation; }
    inline glm::vec3 getDown() const { return -(UP_DIRECTION * mRotation); }
    inline ObjectSystem* getObjectSystem() { return mObjectSystem;}

    inline void setPosition(glm::vec3 newPosition){ this->mPosition = newPosition;}
    inline void setRotation(glm::quat newRotation){ this->mRotation = newRotation;}
    inline void setSkaling(glm::vec3 newScaling){ this->mScaling = newScaling;}

    void lookAt(GameObject* go, glm::vec3 upVector);
    void lookAt(glm::vec3 whereToLookAt, glm::vec3 upVector);

    template<typename ComponentType, typename... Args>
    inline ComponentType* addComponent(Args&&... args){
       ComponentType * c = eng_new(ComponentType, sComponentAllocator)(std::forward<Args>(args)...);
        mComponents[mNumComponents++] = c;
       return c;
    }

private:
    typedef ProxyNoCheckAllocator<LinearAllocator> ComponentAllocator_t;
    static ComponentAllocator_t sComponentAllocator;
};

ENDNAMESPACE
