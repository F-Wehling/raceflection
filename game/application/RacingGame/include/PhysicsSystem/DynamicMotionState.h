#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

BEGINNAMESPACE

using Vector3 = glm::vec3;
using Quaternion = glm::quat;

//Standard MotionState for our physical objects
//This MotionState gives the intial position to the physics and afterwards receives an updated position.
//The new position is then transformed correctly and delegated to the object for the rendering to update.
class DynamicMotionState : public btMotionState
{
private:
    int mEntity = -1.0;
    btTransform mInitialPosition;
    Vector3 *mPos, *mResetPos, *mParentPos; //Pointer to the required positions, access via entity subsystem too complicated for a subroutine
    Quaternion *mOri, *mResetOri, *mParentOri; //Pointer to the required orientation
    bool movable;

public:
    //Constructor, simply saves all arguments into the variables
    DynamicMotionState(const btTransform &initialPosition, int entity, bool movable, Vector3 &parentPos, Vector3 &pos, Vector3 &resetPos, Quaternion &parentOri, Quaternion &ori, Quaternion &resetOri);
    virtual ~DynamicMotionState(){}
    
    void setEntity(int entity);
    //Returns the initial position, called by Bullet once when the object is added
    virtual void getWorldTransform(btTransform &worldTrans) const;
    //Sets the world transform of the entity for the other subsystems wth regard to the parent offset. Called by Bullet after each position update.
    virtual void setWorldTransform(const btTransform &worldTrans);
    
    //Resets the entity's position back to its initial position. Required for deterministic reset of the physics
    void reset();
};

ENDNAMESPACE
