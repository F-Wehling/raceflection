#include "ObjectSystem/GameObject.h"


BEGINNAMESPACE

GameObject::GameObject():
    mID(-1), mPosition(glm::vec3(0.0)), mRotation(glm::quat()), mScaling(glm::vec3(1.0))
{

}

GameObject::GameObject(glm::vec3 Position, glm::quat Rotation, glm::vec3 Scaling ):
    mPosition(Position), mRotation(Rotation), mScaling(Scaling)

{

}

GameObject::~GameObject(){

}



ENDNAMESPACE
