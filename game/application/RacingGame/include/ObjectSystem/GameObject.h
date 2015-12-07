#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

BEGINNAMESPACE

using GameObjectID = int;

class GameObject {
friend class ObjectSystem;
private:

    GameObjectID mID;

    glm::vec3 mPosition;

    glm::quat mRotation;

    glm::vec3 mScaling;


public:

    GameObject();
    GameObject(glm::vec3 Position, glm::quat Rotation, glm::vec3 Scaling );
    ~GameObject();

    inline GameObjectID getID(){ return mID; }
    inline glm::vec3 getPosition(){ return mPosition; }
    inline glm::quat getRotation(){ return mRotation; }
    inline glm::vec3 getScaling() { return mScaling; }

    inline void setPosition(glm::vec3 newPosition){ this->mPosition = newPosition;}
    inline void setRotation(glm::quat newRotation){ this->mRotation = newRotation;}
    inline void setSkaling(glm::vec3 newScaling){ this->mScaling = newScaling;}

};

ENDNAMESPACE
