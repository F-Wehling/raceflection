#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "MemorySystem.h"

#include "Container/Array.h"

#include "Utilities/Flags.h"
#include <GameObjectSpec.h>

BEGINNAMESPACE


class ObjectSystem;
class GameObjectComponent;
using GameObjectID = int32;

class GameObject {
friend class ObjectSystem;
public:
    static glm::vec3 FORWARD_DIRECTION;
    static glm::vec3 RIGHT_DIRECTION;
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

    GameObjectFlagsFlags mFlags;
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
    inline glm::vec3 getRight() const { return RIGHT_DIRECTION * mRotation; }
    inline glm::vec3 getLeft() const { return -(RIGHT_DIRECTION * mRotation); }
    inline glm::vec3 getUp() const { return UP_DIRECTION * mRotation; }
    inline glm::vec3 getDown() const { return -(UP_DIRECTION * mRotation); }
    inline GameObjectFlagsFlags getFlags() const { return mFlags; }
    inline ObjectSystem* getObjectSystem() { return mObjectSystem;}

    inline void setPosition(glm::vec3 newPosition){ this->mPosition = newPosition;}
    inline void setRotation(glm::quat newRotation){ this->mRotation = newRotation;}
    inline void setScaling(glm::vec3 newScaling){ this->mScaling = newScaling;}
    inline void setTriggerArea(bool isTrigger){if(isTrigger) mFlags.set(GameObjectFlags::TriggerArea); else mFlags.unset(GameObjectFlags::TriggerArea); }
    inline void setFlags(GameObjectFlagsFlags newFlags){ this->mFlags = newFlags;}

    inline bool hasPhysics() const {return mFlags.isSet(GameObjectFlags::Physics); }
    inline bool hasMesh() const {return mFlags.isSet(GameObjectFlags::Mesh); }
    inline bool isTriggerArea() const { return mFlags.isSet(GameObjectFlags::TriggerArea); }

	void lookAt(glm::vec3 position, GameObject* go, glm::vec3 upVector);
	void lookAt(glm::vec3 position, glm::vec3 whereToLookAt, glm::vec3 upVector);
	void lookInDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 upVector);
    void lookAt(GameObject* go, glm::vec3 upVector);
    void lookAt(glm::vec3 whereToLookAt, glm::vec3 upVector);
	void lookInDirection(glm::vec3 direction, glm::vec3 upVector);

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
